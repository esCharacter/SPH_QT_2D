#include "Particle.h"

Particle::Particle()
{
}

Particle::Particle(double x, double y)
{
	this->x = x;
	this->y = y;
	vx = vy = fx = fy = 0;
	neighbors.clear();

}

//将受力作用成速度和位移
void Particle::move(double xBottle, double yBottle, double wBottle, double hBottle)
{
	//力累计到速度
	vx += fx;
	vy += fy;
	//速度累计到位移
	x += vx;
	y += vy;
	//如果撞墙，速度为0，如果穿过墙，则受到和超过部分相同的力的反馈
	double d = 0.0001;
	if (x < d + xBottle)
	{
		vx += (d - x + xBottle);
	}

	if (y < d + yBottle)
	{
		vy += (d - y + yBottle);
	}

	if (x > wBottle - d + xBottle)
	{
		vx += (wBottle - d - x + xBottle);
	}

	if (y > hBottle - d + yBottle)
	{
		vy += (hBottle - d - y + yBottle);
	}
}