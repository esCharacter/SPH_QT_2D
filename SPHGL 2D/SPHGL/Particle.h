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
	//位置
	double x;
	double y;
	//速度
	double vx;
	double vy;
	//受力
	double fx;
	double fy;
	//密度
	double density;
	//内部压力
	double pressure;
	//邻居
	vector<Particle*> neighbors;
	int numNeighbors;


public:
	//将受力作用成速度和位移
	void move(double xBottle, double yBottle, double wBottle, double hBottle);

};

