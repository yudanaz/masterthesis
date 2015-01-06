/********************************************************************************
** Form generated from reading UI file 'rgbnird_mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RGBNIRD_MAINWINDOW_H
#define UI_RGBNIRD_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RGBNIRD_MainWindow
{
public:
    QWidget *centralWidget;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *btn_startAcquisition;
    QPushButton *btn_stopAcquisition;
    QPushButton *btn_saveImgs;
    QCheckBox *checkBox_showAllChannels;
    QGraphicsView *graphicsView_RGB;
    QGraphicsView *graphicsView_NIR;
    QGraphicsView *graphicsView_Depth;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *RGBNIRD_MainWindow)
    {
        if (RGBNIRD_MainWindow->objectName().isEmpty())
            RGBNIRD_MainWindow->setObjectName(QStringLiteral("RGBNIRD_MainWindow"));
        RGBNIRD_MainWindow->resize(910, 688);
        centralWidget = new QWidget(RGBNIRD_MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayoutWidget = new QWidget(centralWidget);
        horizontalLayoutWidget->setObjectName(QStringLiteral("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(10, 10, 891, 41));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        btn_startAcquisition = new QPushButton(horizontalLayoutWidget);
        btn_startAcquisition->setObjectName(QStringLiteral("btn_startAcquisition"));

        horizontalLayout->addWidget(btn_startAcquisition);

        btn_stopAcquisition = new QPushButton(horizontalLayoutWidget);
        btn_stopAcquisition->setObjectName(QStringLiteral("btn_stopAcquisition"));

        horizontalLayout->addWidget(btn_stopAcquisition);

        btn_saveImgs = new QPushButton(horizontalLayoutWidget);
        btn_saveImgs->setObjectName(QStringLiteral("btn_saveImgs"));

        horizontalLayout->addWidget(btn_saveImgs);

        checkBox_showAllChannels = new QCheckBox(horizontalLayoutWidget);
        checkBox_showAllChannels->setObjectName(QStringLiteral("checkBox_showAllChannels"));

        horizontalLayout->addWidget(checkBox_showAllChannels);

        graphicsView_RGB = new QGraphicsView(centralWidget);
        graphicsView_RGB->setObjectName(QStringLiteral("graphicsView_RGB"));
        graphicsView_RGB->setGeometry(QRect(10, 60, 500, 500));
        graphicsView_NIR = new QGraphicsView(centralWidget);
        graphicsView_NIR->setObjectName(QStringLiteral("graphicsView_NIR"));
        graphicsView_NIR->setGeometry(QRect(530, 60, 300, 240));
        graphicsView_Depth = new QGraphicsView(centralWidget);
        graphicsView_Depth->setObjectName(QStringLiteral("graphicsView_Depth"));
        graphicsView_Depth->setGeometry(QRect(530, 320, 300, 240));
        RGBNIRD_MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(RGBNIRD_MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 910, 25));
        RGBNIRD_MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(RGBNIRD_MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        RGBNIRD_MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(RGBNIRD_MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        RGBNIRD_MainWindow->setStatusBar(statusBar);

        retranslateUi(RGBNIRD_MainWindow);

        QMetaObject::connectSlotsByName(RGBNIRD_MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *RGBNIRD_MainWindow)
    {
        RGBNIRD_MainWindow->setWindowTitle(QApplication::translate("RGBNIRD_MainWindow", "RGBNIRD_MainWindow", 0));
        btn_startAcquisition->setText(QApplication::translate("RGBNIRD_MainWindow", "Start Acquisition", 0));
        btn_stopAcquisition->setText(QApplication::translate("RGBNIRD_MainWindow", "Stop Acquisition", 0));
        btn_saveImgs->setText(QApplication::translate("RGBNIRD_MainWindow", "Save Images", 0));
        checkBox_showAllChannels->setText(QApplication::translate("RGBNIRD_MainWindow", "Show all Channels", 0));
    } // retranslateUi

};

namespace Ui {
    class RGBNIRD_MainWindow: public Ui_RGBNIRD_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RGBNIRD_MAINWINDOW_H
