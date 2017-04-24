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
	isMouseDown = false;			//����Ƿ���
	frame = 0;
	rotX = 0;//��ʼ����ת�Ƕ�
	rotY = 0;
	zoom = -5.0;
	posX = 0;
	posY = 0;
	isLock = true;
	isBackground = false;

	//���ò���
	ratio = 3.6 / sph.wBottle;//���ű���
	ballRadius = 0.07;	//���Ӱ뾶
	timeStep = 10;		//֡���,ms

	lightPosition[0] = 0.0f;	//��Դλ��,xyz,dis
	lightPosition[1] = 1.5f;
	lightPosition[2] = 0.0f;
	lightPosition[3] = 0.0f;
	lightDirection[0] = 0.0f;	//���շ���
	lightDirection[0] = -1.0f;
	lightDirection[0] = 0.0f;

	//���ò���ȡ����
	double bgy = sph.yBottle * ratio - 0.1;
	setBackground(30, 30, bgy, 1.0);
	setFocusPolicy(Qt::StrongFocus);

	//��������֡
	timer.start(timeStep);
	time.start();
	QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(timeFrame()));
}

void ModelWidget::initializeGL()
{
	glClearColor(0, 0, 0, 1);//����ɫ

	glEnable(GL_DEPTH_TEST);//��Ȳ��ԣ����ֵ��Ч
	glEnable(GL_BLEND);		//��ɫ���
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);//������ɫ׷��
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);		//������ȷ�ΧΪ���

	//���ù��ղ���
	setLight();
}

void ModelWidget::setLight()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//GLfloat lightPosition[] = { 0.0, 1.2, -1.0, 1.0 };		//��Դλ��,xyz,dis
	GLfloat lightAmbient[] = { 0.6, 0.6, 0.6, 1.0 };		//ȫ�ֹ�����,��ǿ��
	GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//GLfloat lightDirection[] = { 0.0f, -1.0f, 0.0f };	//���շ���
	GLfloat lightExponent[] = { 1.0f };					//�۹�̶�
	GLfloat lightCutoff[] = { 60.0f };					//��Դ�н�

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);//ָ����Ҫ��ɫ׷�ٵĲ�������
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);//ָ����0�Ź�Դ��λ�� 
	//����
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse); //�������
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);//���淴���
	//�۹�
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

	//ͶӰ��ʽ��͸��ͶӰ
	gluPerspective(60, (GLfloat)iWinWidth / (GLfloat)iWinHeight, 0.1, 100);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
}

void ModelWidget::mouseMoveEvent(QMouseEvent *e)
{
	if (isMouseDown)
	{
		int deltX, deltY;
		// ��������ƶ����룬��תͼ��
		deltX = e->x() - mouseX;
		deltY = e->y() - mouseY;

		if (false == isLock)
		{
			// ��ת��  
			rotX += deltX / 2;
			rotY += deltY / 2;
			// ��ת�ǲ�����360�� 
			rotX = fmodf(rotX, 360.0);
			rotY = fmodf(rotY, 360.0);
		}
		else
		{
			sph.xBottle += double(deltX) / this->width() / ratio * 4;
			sph.yBottle -= double(deltY) / this->height() / ratio * 4;
		}

		//���µ�ǰ���λ�ã�ʹͼ��ʵʱ��ת
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
	int numDegrees = e->delta() / 8;//�����ĽǶȣ�*8�����������ľ���
	double numSteps = numDegrees / 120.0;//�����Ĳ�����*15�����������ĽǶ�
	zoom += numSteps / 2;
	e->accept();      //���ո��¼�
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

	sph.frame();//һ֡
	this->update();

	frame++;

	if (1000 <= time.elapsed())
	{
		ui->statusBar->showMessage(QString("FPS : %1 / s\t  sum: %2").arg(frame).arg(sph.curGroup * 5));
		time.restart();
		frame = 0;
	}

	e = clock();
	qDebug() << T_CHAR("֡�ڼ����") << e - s;
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

	//�ƶ���Դͬ���ƶ�
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);//ָ����0�Ź�Դ��λ�� 
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightDirection);

	//����ԭ��
	GLUquadricObj *quadricObj = gluNewQuadric();
	gluQuadricDrawStyle(quadricObj, GLU_SMOOTH);
	glColor4f(0.5, 0.5, 0.5, 0.5);
	gluSphere(quadricObj, 0.02, 8, 8);

	if (true == isBackground)
	{
		drawBackground();//���Ʊ���
	}

	drawBoundary();//���Ʊ߽�
	drawParticle();//��������
	glPopMatrix();

	ed = clock();
	qDebug() << T_CHAR("�����ڼ����") << ed - sd;
}

void ModelWidget::drawBoundary(GLenum mode)
{
	glPushMatrix();

	//�߽�λ�úʹ�С
	double xb = sph.xBottle * ratio - ballRadius;
	double yb = sph.yBottle * ratio - ballRadius;
	double wb = sph.wBottle * ratio + ballRadius*2;
	double hb = sph.hBottle * ratio + ballRadius;

	//���Ʊ߽�
	glLineWidth(3);
	glColor4f(0.7, 0.7, 0.8, 0.5);//�߽���ɫ
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
	glColor4f(0.9, 0.9, 1, 0.15);//�߽���ɫ
	glBegin(GL_QUADS);
	glVertex2d(xb, yb + hb);
	glVertex2d(xb, yb);
	glVertex2d(xb + wb, yb);
	glVertex2d(xb + wb, yb + hb);
	glEnd();

	glPopMatrix();
}

//��������
void ModelWidget::drawParticle()
{
	GLUquadricObj *quadricObj = gluNewQuadric();
	gluQuadricDrawStyle(quadricObj, GLU_SMOOTH);

	//������ɫ
	glColor4f(0.5, 0.55, 0.7, 0.9);

	glPushMatrix();

	//����ÿ������
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

	//���嶥������  
	glVertexPointer(3, GL_FLOAT, 0, bottomBackgroud.data());
	glColorPointer(4, GL_FLOAT, 0, colorBackgroud.data());

	//����
	glDrawArrays(GL_QUADS, 0, bottomBackgroud.size() / 3);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glPopMatrix();
}

void ModelWidget::setBackground(float xr, float zr, float yp, float step)
{
	bottomBackgroud.clear();
	colorBackgroud.clear();

	//�ܳ���,-xlen:xlen
	float xlen = xr;
	float zlen = zr;
	float ypos = yp;//y����

	//���񳤶�
	float steplen = step;

	//��������
	int xsum = xlen * 2 / steplen;
	int zsum = zlen * 2 / steplen;

	//����1����ɫ
	float r1 = 0.7;
	float g1 = 0.7;
	float b1 = 0.7;
	float a1 = 0.3;

	//����2����ɫ
	float r2 = 1;
	float g2 = 1;
	float b2 = 1;
	float a2 = 0.3;

	//��ǰ����λ��,�����һ���˵㣻
	float curx = -xlen;
	float curz = -zlen;
	float cury = ypos;
	//���㷽��λ��
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

