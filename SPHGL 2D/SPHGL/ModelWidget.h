#ifndef MODEL_WIDGET
#define MODEL_WIDGET

#include <QtOpenGL/QtOpenGL>
#include <QWheelEvent>
#include <qtimer.h>
#include <freeglut.h>
#include "SPH.h"
#include "ui_sphgl.h"

using namespace std;

#define T_CHAR(str)  QString::fromLocal8Bit(str)	//ת���ַ���

class ModelWidget : public QOpenGLWidget
{
	Q_OBJECT
public:
	ModelWidget(Ui::SPHGLClass *ui = NULL, int id = 0, QWidget* parent = 0);
	void initWidget();

private:
	GLint iWinWidth;			//���ڳߴ�
	GLint iWinHeight;
	Ui::SPHGLClass *ui;

protected:
	/*
	*��Ӧ������opengl
	*/
	void keyPressEvent(QKeyEvent *e);
	void wheelEvent(QWheelEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void mouseDoubleClickEvent(QMouseEvent *e);

	private slots:
	void timeFrame();

private:
	/*
	*SPH
	*/
	SPH sph;
	QTimer timer;
	QTime time;
	int frame;
	double ratio;//���ű���
	

	/*
	*OpenGL
	*/
	void initializeGL();
	void paintGL();
	void resizeGL(int width, int height);
	void setLight();

	/*
	*��������
	*/
	int mouseX, mouseY;			//��ǰ���λ��
	bool isMouseDown;			//����Ƿ���
	float rotX = 0, rotY = 0;	//��ת�Ƕ�
	double zoom;
	double posX, posY;
	int id;
	GLfloat lightDirection[3];		//���շ���
	GLfloat lightPosition[4];		//��Դλ��,xyz,dis

	/*
	*����
	*/
	vector<float> bottomBackgroud;
	vector<float> colorBackgroud;
public:
	bool isLock;
	bool isBackground;
	float ballRadius;
	float timeStep;

public:
	void drawBoundary(GLenum mode = GL_RENDER);
	void drawParticle();
	void drawBackground();
	//�ױ߱���
	void setBackground(float xr = 5, float zr = 5, float yp = -0.5, float step = 0.2);
};

#endif 
