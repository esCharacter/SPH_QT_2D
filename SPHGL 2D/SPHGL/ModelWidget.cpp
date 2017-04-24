#include "ModelWidget.h"

ModelWidget::ModelWidget(Ui::SPHGLClass *ui, int id, QWidget* parent)
:QOpenGLWidget(parent)
{
	this->id = id;
	this->ui = ui;
	initializeGL();
	initWidget();
}

void ModelWidget::initWidget()
{
	isMouseDown = false;			//鼠标是否按下
	frame = 0;
	rotX = 0;//初始化旋转角度
	rotY = 0;
	zoom = -5.0;
	posX = 0;
	posY = 0;
	isLock = true;
	isBackground = false;

	//设置参数
	ratio = 3.6 / sph.wBottle;//缩放比例
	ballRadius = 0.07;	//粒子半径
	timeStep = 10;		//帧间隔,ms

	lightPosition[0] = 0.0f;	//光源位置,xyz,dis
	lightPosition[1] = 1.5f;
	lightPosition[2] = 0.0f;
	lightPosition[3] = 0.0f;
	lightDirection[0] = 0.0f;	//光照方向
	lightDirection[0] = -1.0f;
	lightDirection[0] = 0.0f;

	//设置并获取背景
	double bgy = sph.yBottle * ratio - 0.1;
	setBackground(30, 30, bgy, 1.0);
	setFocusPolicy(Qt::StrongFocus);

	//启动动画帧
	timer.start(timeStep);
	time.start();
	QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(timeFrame()));
}

void ModelWidget::initializeGL()
{
	glClearColor(0, 0, 0, 1);//背景色

	glEnable(GL_DEPTH_TEST);//深度测试，深度值有效
	glEnable(GL_BLEND);		//颜色混合
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);//启用颜色追踪
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);		//设置深度范围为最大

	//设置光照参数
	setLight();
}

void ModelWidget::setLight()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//GLfloat lightPosition[] = { 0.0, 1.2, -1.0, 1.0 };		//光源位置,xyz,dis
	GLfloat lightAmbient[] = { 0.6, 0.6, 0.6, 1.0 };		//全局光属性,光强度
	GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//GLfloat lightDirection[] = { 0.0f, -1.0f, 0.0f };	//光照方向
	GLfloat lightExponent[] = { 1.0f };					//聚光程度
	GLfloat lightCutoff[] = { 60.0f };					//光源夹角

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);//指定需要颜色追踪的材料属性
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);//指定第0号光源的位置 
	//反射
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse); //漫反射后
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);//镜面反射后
	//聚光
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightDirection);
	glLightfv(GL_LIGHT0, GL_SPOT_EXPONENT, lightExponent);
	glLightfv(GL_LIGHT0, GL_SPOT_CUTOFF, lightCutoff);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightAmbient);
}

void ModelWidget::resizeGL(int width, int height)
{
	iWinWidth = (GLint)width;
	iWinHeight = (GLint)height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//投影方式：透视投影
	gluPerspective(60, (GLfloat)iWinWidth / (GLfloat)iWinHeight, 0.1, 100);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
}

void ModelWidget::mouseMoveEvent(QMouseEvent *e)
{
	if (isMouseDown)
	{
		int deltX, deltY;
		// 计算鼠标移动距离，旋转图像
		deltX = e->x() - mouseX;
		deltY = e->y() - mouseY;

		if (false == isLock)
		{
			// 旋转角  
			rotX += deltX / 2;
			rotY += deltY / 2;
			// 旋转角不超过360度 
			rotX = fmodf(rotX, 360.0);
			rotY = fmodf(rotY, 360.0);
		}
		else
		{
			sph.xBottle += double(deltX) / this->width() / ratio * 4;
			sph.yBottle -= double(deltY) / this->height() / ratio * 4;
		}

		//更新当前鼠标位置，使图像实时旋转
		mouseX = e->x();
		mouseY = e->y();
		//update();
	}
}
void ModelWidget::mousePressEvent(QMouseEvent *e)
{
	mouseX = e->x();
	mouseY = e->y();

	isMouseDown = true;


}
void ModelWidget::mouseReleaseEvent(QMouseEvent *e)
{
	isMouseDown = false;
}
void ModelWidget::mouseDoubleClickEvent(QMouseEvent *e)
{

}
void ModelWidget::wheelEvent(QWheelEvent *e)
{
	int numDegrees = e->delta() / 8;//滚动的角度，*8就是鼠标滚动的距离
	double numSteps = numDegrees / 120.0;//滚动的步数，*15就是鼠标滚动的角度
	zoom += numSteps / 2;
	e->accept();      //接收该事件
	update();
}
void ModelWidget::keyPressEvent(QKeyEvent *e)
{
	switch (e->key())
	{
	case Qt::Key_W:
		posY -= 0.05;
		//zoom += (GLfloat)0.05;
		break;
	case Qt::Key_S:
		posY += 0.05;
		//zoom -= (GLfloat)0.05;
		break;
	case Qt::Key_A:
		posX += 0.05;
		//rotX -= (GLfloat)5;
		break;
	case Qt::Key_D:
		posX -= 0.05;
		//rotX += (GLfloat)5;
		break;
	}
	update();
}
void ModelWidget::timeFrame()
{
	static int s = 0, e = 0;
	s = clock();

	sph.frame();//一帧
	this->update();

	frame++;

	if (1000 <= time.elapsed())
	{
		ui->statusBar->showMessage(QString("FPS : %1 / s\t  sum: %2").arg(frame).arg(sph.curGroup * 5));
		time.restart();
		frame = 0;
	}

	e = clock();
	qDebug() << T_CHAR("帧内间隔：") << e - s;
}

void ModelWidget::paintGL()
{
	static int sd = 0, ed = 0;
	sd = clock();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	glTranslatef(posX, posY, zoom);
	glRotatef(rotX, 0.0, 1.0, 0.0);
	glRotatef(rotY, 1.0, 0.0, 0.0);

	//移动光源同步移动
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);//指定第0号光源的位置 
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightDirection);

	//绘制原点
	GLUquadricObj *quadricObj = gluNewQuadric();
	gluQuadricDrawStyle(quadricObj, GLU_SMOOTH);
	glColor4f(0.5, 0.5, 0.5, 0.5);
	gluSphere(quadricObj, 0.02, 8, 8);

	if (true == isBackground)
	{
		drawBackground();//绘制背景
	}

	drawBoundary();//绘制边界
	drawParticle();//绘制粒子
	glPopMatrix();

	ed = clock();
	qDebug() << T_CHAR("绘制内间隔：") << ed - sd;
}

void ModelWidget::drawBoundary(GLenum mode)
{
	glPushMatrix();

	//边界位置和大小
	double xb = sph.xBottle * ratio - ballRadius;
	double yb = sph.yBottle * ratio - ballRadius;
	double wb = sph.wBottle * ratio + ballRadius*2;
	double hb = sph.hBottle * ratio + ballRadius;

	//绘制边界
	glLineWidth(3);
	glColor4f(0.7, 0.7, 0.8, 0.5);//边界颜色
	glBegin(GL_LINES);
	glVertex2d(xb, yb + hb);
	glVertex2d(xb, yb);

	glVertex2d(xb, yb);
	glVertex2d(xb + wb, yb);

	glVertex2d(xb + wb, yb);
	glVertex2d(xb + wb, yb + hb);

	glVertex2d(xb + wb, yb + hb);
	glVertex2d(xb, yb + hb);
	glEnd();

	if (mode == GL_SELECT)
	{
		glLoadName(1);
	}
	glColor4f(0.9, 0.9, 1, 0.15);//边界颜色
	glBegin(GL_QUADS);
	glVertex2d(xb, yb + hb);
	glVertex2d(xb, yb);
	glVertex2d(xb + wb, yb);
	glVertex2d(xb + wb, yb + hb);
	glEnd();

	glPopMatrix();
}

//绘制粒子
void ModelWidget::drawParticle()
{
	GLUquadricObj *quadricObj = gluNewQuadric();
	gluQuadricDrawStyle(quadricObj, GLU_SMOOTH);

	//粒子颜色
	glColor4f(0.5, 0.55, 0.7, 0.9);

	glPushMatrix();

	//绘制每个粒子
	for (int i = 0; i < sph.numParticles; i++)
	{
		Particle &p = sph.particles[i];
		glPushMatrix();
		glTranslatef(p.x * ratio, p.y * ratio, 0);
		gluSphere(quadricObj, ballRadius, 16, 16);
		glPopMatrix();
	}
	glPopMatrix();
}

void ModelWidget::drawBackground()
{
	glPushMatrix();

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	//定义顶点数组  
	glVertexPointer(3, GL_FLOAT, 0, bottomBackgroud.data());
	glColorPointer(4, GL_FLOAT, 0, colorBackgroud.data());

	//绘制
	glDrawArrays(GL_QUADS, 0, bottomBackgroud.size() / 3);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glPopMatrix();
}

void ModelWidget::setBackground(float xr, float zr, float yp, float step)
{
	bottomBackgroud.clear();
	colorBackgroud.clear();

	//总长度,-xlen:xlen
	float xlen = xr;
	float zlen = zr;
	float ypos = yp;//y坐标

	//单格长度
	float steplen = step;

	//方格数量
	int xsum = xlen * 2 / steplen;
	int zsum = zlen * 2 / steplen;

	//方格1的颜色
	float r1 = 0.7;
	float g1 = 0.7;
	float b1 = 0.7;
	float a1 = 0.3;

	//方格2的颜色
	float r2 = 1;
	float g2 = 1;
	float b2 = 1;
	float a2 = 0.3;

	//当前采样位置,方格的一个端点；
	float curx = -xlen;
	float curz = -zlen;
	float cury = ypos;
	//计算方格位置
	for (int i = 0; i < xsum; i++)
	{
		for (int j = 0; j < zsum; j++)
		{
			bottomBackgroud.push_back(curx);
			bottomBackgroud.push_back(cury);
			bottomBackgroud.push_back(curz);

			bottomBackgroud.push_back(curx + steplen);
			bottomBackgroud.push_back(cury);
			bottomBackgroud.push_back(curz);

			bottomBackgroud.push_back(curx + steplen);
			bottomBackgroud.push_back(cury);
			bottomBackgroud.push_back(curz + steplen);

			bottomBackgroud.push_back(curx);
			bottomBackgroud.push_back(cury);
			bottomBackgroud.push_back(curz + steplen);

			float r = r1;
			float g = g1;
			float b = b1;
			float a = a1;
			if (0 == (i + j) % 2)
			{
				r = r1;
				g = g1;
				b = b1;
				a = a1;
			}
			else
			{
				r = r2;
				g = g2;
				b = b2;
				a = a2;
			}
			for (int k = 0; k < 4; k++)
			{
				colorBackgroud.push_back(r);
				colorBackgroud.push_back(g);
				colorBackgroud.push_back(b);
				colorBackgroud.push_back(a);
			}

			curx += steplen;
		}
		curx = -xlen;
		curz += steplen;
	}
}

