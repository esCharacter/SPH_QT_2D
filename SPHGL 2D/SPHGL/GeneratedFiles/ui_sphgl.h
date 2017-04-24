/********************************************************************************
** Form generated from reading UI file 'sphgl.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SPHGL_H
#define UI_SPHGL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SPHGLClass
{
public:
    QAction *actionLock;
    QAction *actionAbout;
    QAction *actionBackground;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QVBoxLayout *baseLayout;
    QMenuBar *menuBar;
    QMenu *menu;
    QMenu *menu_2;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *SPHGLClass)
    {
        if (SPHGLClass->objectName().isEmpty())
            SPHGLClass->setObjectName(QStringLiteral("SPHGLClass"));
        SPHGLClass->resize(900, 600);
        actionLock = new QAction(SPHGLClass);
        actionLock->setObjectName(QStringLiteral("actionLock"));
        actionLock->setCheckable(true);
        actionLock->setChecked(true);
        QIcon icon;
        icon.addFile(QStringLiteral(":/SPHGL/Resources/lock_view.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionLock->setIcon(icon);
        actionAbout = new QAction(SPHGLClass);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        actionBackground = new QAction(SPHGLClass);
        actionBackground->setObjectName(QStringLiteral("actionBackground"));
        actionBackground->setCheckable(true);
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/SPHGL/Resources/action_background.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionBackground->setIcon(icon1);
        centralWidget = new QWidget(SPHGLClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        baseLayout = new QVBoxLayout();
        baseLayout->setSpacing(6);
        baseLayout->setObjectName(QStringLiteral("baseLayout"));

        gridLayout->addLayout(baseLayout, 0, 0, 1, 1);

        SPHGLClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(SPHGLClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 900, 23));
        menu = new QMenu(menuBar);
        menu->setObjectName(QStringLiteral("menu"));
        menu_2 = new QMenu(menuBar);
        menu_2->setObjectName(QStringLiteral("menu_2"));
        SPHGLClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(SPHGLClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        SPHGLClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(SPHGLClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        SPHGLClass->setStatusBar(statusBar);

        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menu_2->menuAction());
        menu->addAction(actionLock);
        menu->addAction(actionBackground);
        menu_2->addAction(actionAbout);
        mainToolBar->addAction(actionLock);
        mainToolBar->addAction(actionBackground);

        retranslateUi(SPHGLClass);
        QObject::connect(actionLock, SIGNAL(triggered()), SPHGLClass, SLOT(onLockView()));
        QObject::connect(actionBackground, SIGNAL(triggered()), SPHGLClass, SLOT(onSetBackground()));

        QMetaObject::connectSlotsByName(SPHGLClass);
    } // setupUi

    void retranslateUi(QMainWindow *SPHGLClass)
    {
        SPHGLClass->setWindowTitle(QApplication::translate("SPHGLClass", "SPHGL", 0));
        actionLock->setText(QApplication::translate("SPHGLClass", "\351\224\201\345\256\232", 0));
        actionAbout->setText(QApplication::translate("SPHGLClass", "About", 0));
        actionBackground->setText(QApplication::translate("SPHGLClass", "\350\203\214\346\231\257", 0));
        menu->setTitle(QApplication::translate("SPHGLClass", "\345\274\200\345\247\213", 0));
        menu_2->setTitle(QApplication::translate("SPHGLClass", "\345\205\263\344\272\216", 0));
    } // retranslateUi

};

namespace Ui {
    class SPHGLClass: public Ui_SPHGLClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SPHGL_H
