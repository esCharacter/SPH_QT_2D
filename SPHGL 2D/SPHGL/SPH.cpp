#include "SPH.h"


SPH::SPH()
{
	initParameter();
	initialize();
}

SPH::~SPH()
{
}

void SPH::initParameter()
{
	GRAVITY = -0.5;//重力系数
	RANGE = 36;//光滑核核半径
	RANGE2 = RANGE * RANGE;//光滑核半径的平方
	DENSITY = 1;//平静时的密度
	PRESSURE = 2;//压力系数
	VISCOSITY = 0.075;//粘稠度系数

	//粒子
	numParticles = 0;//粒子个数
	group = 150;//粒子发射组数
	curGroup = 0;
	/*瓶子的最近一次历史坐标*/
	bottle_x = 0;
	bottle_y = 0;
	/*瓶子的最近一次历史速度和目前的速度*/
	bottle_vx1 = 0;
	bottle_vx2 = 0;
	bottle_vy1 = 0;
	bottle_vy2 = 0;
	/*瓶子的加速度（受力）*/
	bottle_fx = 0;
	bottle_fy = 0;

	//瓶子大小和位置
	xBottle = -250;
	yBottle = -250;
	wBottle = 500;
	hBottle = 500;
}

void SPH::initialize()
{
	bottle_x = xBottle;
	bottle_y = yBottle;
}

/*一个帧周期执行一次*/
void SPH::frame()
{
	/*规定发射press组粒子，超过数量就不发射啦*/
	if (curGroup < group)
	{
		pour();//发射粒子函数
		curGroup++;
	}
	bottleMove();
	move();//对瓶内粒子进行计算
}

/*发射一组粒子*/
void SPH::pour()
{
	for (int i = -2; i <= 2; i++)
	{
		numParticles++;
		particles.push_back(Particle(xBottle + wBottle*0.7 - double(i * 36), yBottle + hBottle - 0.002));
		particles[numParticles - 1].vy = -5;
		particles[numParticles - 1].vx = -8;
	}
}

/*检测瓶子的运动状态和外力受力状态并记录数据*/
void SPH::bottleMove()
{
	bottle_vx2 = xBottle - bottle_x;//记录速度
	bottle_x = xBottle;//将位置保存到历史
	bottle_fx = (bottle_vx2 - bottle_vx1) / 5;//记录受力
	bottle_vx1 = bottle_vx2;//保存历史速度
	bottle_vy2 = yBottle - bottle_y;//以下同理
	bottle_y = yBottle;
	bottle_fy = (bottle_vy2 - bottle_vy1) / 5;
	bottle_vy1 = bottle_vy2;
}

/*执行一次粒子的计算*/
void SPH::move()
{
	int i;//外循环中间变量
	int j;//内循环中间变量
	double dist;//距离
	Particle pi;//外循环的粒子
	Particle pj;//内循环的粒子
	double dx;//向量的x分量
	double dy;//向量的y分量
	double weight;//光滑核函数值（或变值）
	double pressure;//压力大小
	double viscosity;//粘稠阻力大小

	for (i = 0; i < numParticles; i++)
	{
		particles[i].neighbors.clear();
	}

	/*该循环将粒子分区，在光滑核半径范围内的粒子互为邻居*/
	for (i = 0; i < numParticles; i++)
	{
		Particle &pi = particles[i];//取外循环要操作的粒子
		pi.numNeighbors = 0;//将粒子邻居至空
		pi.density = 0;//将粒子密度属性初始化
		/*内循环，用握手规则减少循环次数*/
		for (j = 0; j < i; j++)
		{
			Particle &pj = particles[j];//取内循环粒子
			dist = pow(pi.x - pj.x, 2) + pow(pi.y - pj.y, 2);//计算两个粒子的距离平方
			/*如果两个粒子距离在范围内 进入判断*/
			if (dist < RANGE2)
			{
				dist = sqrt(dist);//算出距离
				weight = pow(1 - dist / RANGE, 2);//算出光滑核函数的值
				//将值累加到两个粒子的密度里
				pi.density += weight;
				pj.density += weight;
				//互加为邻居
				pi.numNeighbors++;
				pi.neighbors.push_back(&pj);
				pj.numNeighbors++;
				pj.neighbors.push_back(&pi);
			}
		}
	}
	/*算出粒子的压力*/
	for (i = 0; i < numParticles; i++)
	{
		Particle &pi = particles[i];
		if (pi.density < DENSITY) pi.density = DENSITY;
		pi.pressure = pi.density - DENSITY;//粒子压力等于K(粒子密度-静态密度)，K默认为1

	}
	/*计算粒子所受别的粒子的压力和阻力*/
	for (i = 0; i < numParticles; i++)
	{
		Particle &pi = particles[i];
		//初始化粒子的受力
		pi.fx = 0;
		pi.fy = 0;
		//循环内计算别的粒子给这个粒子的压力和阻力
		for (j = 0; j < pi.numNeighbors; j++)
		{
			pj = *pi.neighbors[j];
			//计算两个粒子的坐标差
			dx = pi.x - pj.x;
			dy = pi.y - pj.y;
			dist = sqrt(pow(dx, 2) + pow(dy, 2));//计算两个粒子的距离
			weight = 1 - dist / RANGE;//光滑核函数的一阶微分和
			pressure = weight * (pi.pressure + pj.pressure) / (2 * pj.density) * PRESSURE;//计算粒子对粒子的压力标量
			/*将压力分成向量的两个值累计到受力中*/
			dist = 1 / dist;
			dx *= dist;
			dy *= dist;
			pi.fx += dx * pressure;
			pi.fy += dy * pressure;
			viscosity = weight / pj.density * VISCOSITY;//计算除速度外的阻力系数
			//计算粒子相对速度的向量
			dx = pi.vx - pj.vx;
			dy = pi.vy - pj.vy;
			//乘入速度因子 得出粒子对粒子的压力 累加入受力
			pi.fx -= dx * viscosity;
			pi.fy -= dy * viscosity;
		}
	}
	for (i = 0; i < numParticles; i++)
	{
		Particle &pi = particles[i];
		//考虑外部瓶子受力，将其反向累计
		pi.fx -= bottle_fx;
		pi.fy -= bottle_fy;
		pi.fy += GRAVITY;
		//将受力作用成速度和位移
		pi.move(xBottle, yBottle, wBottle, hBottle);

	}

}
