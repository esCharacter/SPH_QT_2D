#pragma once
#include <vector>
#include "Particle.h"

using namespace std;

class Particle
{
public:
	Particle();
	Particle(double x, double y);

public:
	//λ��
	double x;
	double y;
	//�ٶ�
	double vx;
	double vy;
	//����
	double fx;
	double fy;
	//�ܶ�
	double density;
	//�ڲ�ѹ��
	double pressure;
	//�ھ�
	vector<Particle*> neighbors;
	int numNeighbors;


public:
	//���������ó��ٶȺ�λ��
	void move(double xBottle, double yBottle, double wBottle, double hBottle);

};

