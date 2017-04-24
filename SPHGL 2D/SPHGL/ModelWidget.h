#ifndef MODEL_WIDGET
#define MODEL_WIDGET

#include <QtOpenGL/QtOpenGL>
#include <QWheelEvent>
#include <qtimer.h>
#include <freeglut.h>
#include "SPH.h"
#include "ui_sphgl.h"

using namespace std;

#define T_CHAR(str)  QString::fromLocal8Bit(str)	//转换字符串

class ModelWidget : public QOpenGLWidget
{
	Q_OBJECT
public:
	ModelWidget(Ui::SPHGLClass *ui = NULL, int id = 0, QWidget* parent = 0);
	void initWidget();

private:
	GLint iWinWidth;			//窗口尺寸
	GLint iWinHeight;
	Ui::SPHGLClass *ui;

protected:
	/*
	*响应函数，opengl
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
	double ratio;//缩放比例
	

	/*
	*OpenGL
	*/
	void initializeGL();
	void paintGL();
	void resizeGL(int width, int height);
	void setLight();

	/*
	*环境变量
	*/
	int mouseX, mouseY;			//当前鼠标位置
	bool isMouseDown;			//鼠标是否按下
	float rotX = 0, rotY = 0;	//旋转角度
	double zoom;
	double posX, posY;
	int id;
	GLfloat lightDirection[3];		//光照方向
	GLfloat lightPosition[4];		//光源位置,xyz,dis

	/*
	*背景
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
	//底边背景
	void setBackground(float xr = 5, float zr = 5, float yp = -0.5, float step = 0.2);
};

#endif 
