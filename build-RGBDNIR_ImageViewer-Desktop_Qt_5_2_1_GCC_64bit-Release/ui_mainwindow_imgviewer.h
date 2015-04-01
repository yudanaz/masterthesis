/********************************************************************************
** Form generated from reading UI file 'mainwindow_imgviewer.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_IMGVIEWER_H
#define UI_MAINWINDOW_IMGVIEWER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow_imgViewer
{
public:
    QWidget *centralWidget;
    QGraphicsView *graphicsView_NIR;
    QGraphicsView *graphicsView_RGB;
    QGraphicsView *graphicsView_depth;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_open;
    QPushButton *pushButton_previous;
    QPushButton *pushButton_markAsBad;
    QPushButton *pushButton_next;
    QLabel *label_imageName;
    QCheckBox *checkBox_equalHist;
    QCheckBox *checkBox_showChannels;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow_imgViewer)
    {
        if (MainWindow_imgViewer->objectName().isEmpty())
            MainWindow_imgViewer->setObjectName(QStringLiteral("MainWindow_imgViewer"));
        MainWindow_imgViewer->resize(969, 650);
        centralWidget = new QWidget(MainWindow_imgViewer);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        graphicsView_NIR = new QGraphicsView(centralWidget);
        graphicsView_NIR->setObjectName(QStringLiteral("graphicsView_NIR"));
        graphicsView_NIR->setGeometry(QRect(10, 10, 638, 510));
        graphicsView_RGB = new QGraphicsView(centralWidget);
        graphicsView_RGB->setObjectName(QStringLiteral("graphicsView_RGB"));
        graphicsView_RGB->setGeometry(QRect(658, 10, 302, 302));
        graphicsView_depth = new QGraphicsView(centralWidget);
        graphicsView_depth->setObjectName(QStringLiteral("graphicsView_depth"));
        graphicsView_depth->setGeometry(QRect(656, 330, 302, 227));
        horizontalLayoutWidget = new QWidget(centralWidget);
        horizontalLayoutWidget->setObjectName(QStringLiteral("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(19, 560, 621, 31));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        pushButton_open = new QPushButton(horizontalLayoutWidget);
        pushButton_open->setObjectName(QStringLiteral("pushButton_open"));

        horizontalLayout->addWidget(pushButton_open);

        pushButton_previous = new QPushButton(horizontalLayoutWidget);
        pushButton_previous->setObjectName(QStringLiteral("pushButton_previous"));
        pushButton_previous->setMaximumSize(QSize(100, 16777215));

        horizontalLayout->addWidget(pushButton_previous);

        pushButton_markAsBad = new QPushButton(horizontalLayoutWidget);
        pushButton_markAsBad->setObjectName(QStringLiteral("pushButton_markAsBad"));

        horizontalLayout->addWidget(pushButton_markAsBad);

        pushButton_next = new QPushButton(horizontalLayoutWidget);
        pushButton_next->setObjectName(QStringLiteral("pushButton_next"));
        pushButton_next->setMaximumSize(QSize(100, 16777215));

        horizontalLayout->addWidget(pushButton_next);

        label_imageName = new QLabel(centralWidget);
        label_imageName->setObjectName(QStringLiteral("label_imageName"));
        label_imageName->setGeometry(QRect(20, 530, 411, 21));
        checkBox_equalHist = new QCheckBox(centralWidget);
        checkBox_equalHist->setObjectName(QStringLiteral("checkBox_equalHist"));
        checkBox_equalHist->setGeometry(QRect(520, 530, 121, 21));
        checkBox_showChannels = new QCheckBox(centralWidget);
        checkBox_showChannels->setObjectName(QStringLiteral("checkBox_showChannels"));
        checkBox_showChannels->setGeometry(QRect(380, 530, 121, 21));
        MainWindow_imgViewer->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow_imgViewer);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 969, 20));
        MainWindow_imgViewer->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow_imgViewer);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow_imgViewer->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow_imgViewer);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow_imgViewer->setStatusBar(statusBar);

        retranslateUi(MainWindow_imgViewer);

        QMetaObject::connectSlotsByName(MainWindow_imgViewer);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow_imgViewer)
    {
        MainWindow_imgViewer->setWindowTitle(QApplication::translate("MainWindow_imgViewer", "MainWindow_imgViewer", 0));
        pushButton_open->setText(QApplication::translate("MainWindow_imgViewer", "Open Dir", 0));
        pushButton_previous->setText(QApplication::translate("MainWindow_imgViewer", "<<", 0));
        pushButton_markAsBad->setText(QApplication::translate("MainWindow_imgViewer", "Mark as \"bad\"", 0));
        pushButton_next->setText(QApplication::translate("MainWindow_imgViewer", ">>", 0));
        label_imageName->setText(QApplication::translate("MainWindow_imgViewer", "Image name", 0));
        checkBox_equalHist->setText(QApplication::translate("MainWindow_imgViewer", "equalHist", 0));
        checkBox_showChannels->setText(QApplication::translate("MainWindow_imgViewer", "show Channels", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow_imgViewer: public Ui_MainWindow_imgViewer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_IMGVIEWER_H
