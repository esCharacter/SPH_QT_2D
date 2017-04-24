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

//���������ó��ٶȺ�λ��
void Particle::move(double xBottle, double yBottle, double wBottle, double hBottle)
{
	//���ۼƵ��ٶ�
	vx += fx;
	vy += fy;
	//�ٶ��ۼƵ�λ��
	x += vx;
	y += vy;
	//���ײǽ���ٶ�Ϊ0���������ǽ�����ܵ��ͳ���������ͬ�����ķ���
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