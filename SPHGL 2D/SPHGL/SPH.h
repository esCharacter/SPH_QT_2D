#pragma once
#include "Particle.h"
#include <time.h>

class SPH
{
public:
	SPH();
	~SPH();

public:
	double GRAVITY;//����ϵ��
	double RANGE;//�⻬�˺˰뾶
	double RANGE2;//�⻬�˰뾶��ƽ��
	double DENSITY;//ƽ��ʱ���ܶ�
	double PRESSURE;//ѹ��ϵ��
	double VISCOSITY;//ճ���ϵ��

	vector<Particle> particles;//��������
	int numParticles;//���Ӹ���

	int group;//���ӷ�������
	int curGroup;
	/*ƿ�ӵ����һ����ʷ����*/
	double bottle_x;
	double bottle_y;
	/*ƿ�ӵ����һ����ʷ�ٶȺ�Ŀǰ���ٶ�*/
	double bottle_vx1;
	double bottle_vx2;
	double bottle_vy1;
	double bottle_vy2;
	/*ƿ�ӵļ��ٶȣ�������*/
	double bottle_fx;
	double bottle_fy;
	/*ƿ�ӳߴ��λ��(���½�)*/
	double xBottle;
	double yBottle;
	double wBottle;
	double hBottle;

public:
	/*һ��֡����ִ��һ��*/
	void frame();
	/*���ƿ�ӵ��˶�״̬����������״̬����¼����*/
	void bottleMove();

private:
	void initParameter();
	void initialize();
	void pour();
	void move();
};

