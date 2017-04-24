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
	GRAVITY = -0.5;//����ϵ��
	RANGE = 36;//�⻬�˺˰뾶
	RANGE2 = RANGE * RANGE;//�⻬�˰뾶��ƽ��
	DENSITY = 1;//ƽ��ʱ���ܶ�
	PRESSURE = 2;//ѹ��ϵ��
	VISCOSITY = 0.075;//ճ���ϵ��

	//����
	numParticles = 0;//���Ӹ���
	group = 150;//���ӷ�������
	curGroup = 0;
	/*ƿ�ӵ����һ����ʷ����*/
	bottle_x = 0;
	bottle_y = 0;
	/*ƿ�ӵ����һ����ʷ�ٶȺ�Ŀǰ���ٶ�*/
	bottle_vx1 = 0;
	bottle_vx2 = 0;
	bottle_vy1 = 0;
	bottle_vy2 = 0;
	/*ƿ�ӵļ��ٶȣ�������*/
	bottle_fx = 0;
	bottle_fy = 0;

	//ƿ�Ӵ�С��λ��
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

/*һ��֡����ִ��һ��*/
void SPH::frame()
{
	/*�涨����press�����ӣ����������Ͳ�������*/
	if (curGroup < group)
	{
		pour();//�������Ӻ���
		curGroup++;
	}
	bottleMove();
	move();//��ƿ�����ӽ��м���
}

/*����һ������*/
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

/*���ƿ�ӵ��˶�״̬����������״̬����¼����*/
void SPH::bottleMove()
{
	bottle_vx2 = xBottle - bottle_x;//��¼�ٶ�
	bottle_x = xBottle;//��λ�ñ��浽��ʷ
	bottle_fx = (bottle_vx2 - bottle_vx1) / 5;//��¼����
	bottle_vx1 = bottle_vx2;//������ʷ�ٶ�
	bottle_vy2 = yBottle - bottle_y;//����ͬ��
	bottle_y = yBottle;
	bottle_fy = (bottle_vy2 - bottle_vy1) / 5;
	bottle_vy1 = bottle_vy2;
}

/*ִ��һ�����ӵļ���*/
void SPH::move()
{
	int i;//��ѭ���м����
	int j;//��ѭ���м����
	double dist;//����
	Particle pi;//��ѭ��������
	Particle pj;//��ѭ��������
	double dx;//������x����
	double dy;//������y����
	double weight;//�⻬�˺���ֵ�����ֵ��
	double pressure;//ѹ����С
	double viscosity;//ճ��������С

	for (i = 0; i < numParticles; i++)
	{
		particles[i].neighbors.clear();
	}

	/*��ѭ�������ӷ������ڹ⻬�˰뾶��Χ�ڵ����ӻ�Ϊ�ھ�*/
	for (i = 0; i < numParticles; i++)
	{
		Particle &pi = particles[i];//ȡ��ѭ��Ҫ����������
		pi.numNeighbors = 0;//�������ھ�����
		pi.density = 0;//�������ܶ����Գ�ʼ��
		/*��ѭ���������ֹ������ѭ������*/
		for (j = 0; j < i; j++)
		{
			Particle &pj = particles[j];//ȡ��ѭ������
			dist = pow(pi.x - pj.x, 2) + pow(pi.y - pj.y, 2);//�����������ӵľ���ƽ��
			/*����������Ӿ����ڷ�Χ�� �����ж�*/
			if (dist < RANGE2)
			{
				dist = sqrt(dist);//�������
				weight = pow(1 - dist / RANGE, 2);//����⻬�˺�����ֵ
				//��ֵ�ۼӵ��������ӵ��ܶ���
				pi.density += weight;
				pj.density += weight;
				//����Ϊ�ھ�
				pi.numNeighbors++;
				pi.neighbors.push_back(&pj);
				pj.numNeighbors++;
				pj.neighbors.push_back(&pi);
			}
		}
	}
	/*������ӵ�ѹ��*/
	for (i = 0; i < numParticles; i++)
	{
		Particle &pi = particles[i];
		if (pi.density < DENSITY) pi.density = DENSITY;
		pi.pressure = pi.density - DENSITY;//����ѹ������K(�����ܶ�-��̬�ܶ�)��KĬ��Ϊ1

	}
	/*�����������ܱ�����ӵ�ѹ��������*/
	for (i = 0; i < numParticles; i++)
	{
		Particle &pi = particles[i];
		//��ʼ�����ӵ�����
		pi.fx = 0;
		pi.fy = 0;
		//ѭ���ڼ��������Ӹ�������ӵ�ѹ��������
		for (j = 0; j < pi.numNeighbors; j++)
		{
			pj = *pi.neighbors[j];
			//�����������ӵ������
			dx = pi.x - pj.x;
			dy = pi.y - pj.y;
			dist = sqrt(pow(dx, 2) + pow(dy, 2));//�����������ӵľ���
			weight = 1 - dist / RANGE;//�⻬�˺�����һ��΢�ֺ�
			pressure = weight * (pi.pressure + pj.pressure) / (2 * pj.density) * PRESSURE;//�������Ӷ����ӵ�ѹ������
			/*��ѹ���ֳ�����������ֵ�ۼƵ�������*/
			dist = 1 / dist;
			dx *= dist;
			dy *= dist;
			pi.fx += dx * pressure;
			pi.fy += dy * pressure;
			viscosity = weight / pj.density * VISCOSITY;//������ٶ��������ϵ��
			//������������ٶȵ�����
			dx = pi.vx - pj.vx;
			dy = pi.vy - pj.vy;
			//�����ٶ����� �ó����Ӷ����ӵ�ѹ�� �ۼ�������
			pi.fx -= dx * viscosity;
			pi.fy -= dy * viscosity;
		}
	}
	for (i = 0; i < numParticles; i++)
	{
		Particle &pi = particles[i];
		//�����ⲿƿ�����������䷴���ۼ�
		pi.fx -= bottle_fx;
		pi.fy -= bottle_fy;
		pi.fy += GRAVITY;
		//���������ó��ٶȺ�λ��
		pi.move(xBottle, yBottle, wBottle, hBottle);

	}

}
