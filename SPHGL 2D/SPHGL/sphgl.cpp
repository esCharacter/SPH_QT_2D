#include "sphgl.h"

SPHGL::SPHGL(QWidget *parent)
: QMainWindow(parent)
{
	ui.setupUi(this);
	init();
}

SPHGL::~SPHGL()
{

}

void SPHGL::init()
{
	viewWidget = new ModelWidget(&ui);
	ui.baseLayout->addWidget(viewWidget);

}

void SPHGL::onLockView()
{
	if (ui.actionLock->isChecked() == true)
	{
		viewWidget->isLock = true;
	}
	else
	{
		viewWidget->isLock = false;
	}
}

void SPHGL::onSetBackground()
{
	if (ui.actionBackground->isChecked() == true)
	{
		viewWidget->isBackground = true;
	}
	else
	{
		viewWidget->isBackground = false;
	}
}