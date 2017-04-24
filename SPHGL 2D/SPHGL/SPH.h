#pragma once
#include "Particle.h"
#include <time.h>

class SPH
{
public:
	SPH();
	~SPH();

public:
	double GRAVITY;//重力系数
	double RANGE;//光滑核核半径
	double RANGE2;//光滑核半径的平方
	double DENSITY;//平静时的密度
	double PRESSURE;//压力系数
	double VISCOSITY;//粘稠度系数

	vector<Particle> particles;//粒子数组
	int numParticles;//粒子个数

	int group;//粒子发射组数
	int curGroup;
	/*瓶子的最近一次历史坐标*/
	double bottle_x;
	double bottle_y;
	/*瓶子的最近一次历史速度和目前的速度*/
	double bottle_vx1;
	double bottle_vx2;
	double bottle_vy1;
	double bottle_vy2;
	/*瓶子的加速度（受力）*/
	double bottle_fx;
	double bottle_fy;
	/*瓶子尺寸和位置(左下角)*/
	double xBottle;
	double yBottle;
	double wBottle;
	double hBottle;

public:
	/*一个帧周期执行一次*/
	void frame();
	/*检测瓶子的运动状态和外力受力状态并记录数据*/
	void bottleMove();

private:
	void initParameter();
	void initialize();
	void pour();
	void move();
};

