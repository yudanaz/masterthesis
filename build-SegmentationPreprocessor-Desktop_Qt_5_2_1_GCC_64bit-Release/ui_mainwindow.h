/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionStereoM_Params;
    QWidget *centralWidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton_calibCams;
    QPushButton *pushButton_calibCamRig;
    QPushButton *pushButton_preproc;
    QPushButton *pushButton_reproc;
    QSpacerItem *verticalSpacer;
    QPushButton *pushButton_save;
    QPushButton *pushButton_load;
    QMenuBar *menuBar;
    QMenu *menuParameters;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(400, 346);
        MainWindow->setStyleSheet(QStringLiteral("background-color: rgb(196, 201, 222)"));
        actionStereoM_Params = new QAction(MainWindow);
        actionStereoM_Params->setObjectName(QStringLiteral("actionStereoM_Params"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayoutWidget = new QWidget(centralWidget);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(30, 10, 341, 271));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        pushButton_calibCams = new QPushButton(verticalLayoutWidget);
        pushButton_calibCams->setObjectName(QStringLiteral("pushButton_calibCams"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButton_calibCams->sizePolicy().hasHeightForWidth());
        pushButton_calibCams->setSizePolicy(sizePolicy);
        pushButton_calibCams->setStyleSheet(QStringLiteral("background-color: rgb(255, 185, 84)"));

        verticalLayout->addWidget(pushButton_calibCams);

        pushButton_calibCamRig = new QPushButton(verticalLayoutWidget);
        pushButton_calibCamRig->setObjectName(QStringLiteral("pushButton_calibCamRig"));
        pushButton_calibCamRig->setStyleSheet(QStringLiteral("background-color: rgb(255, 185, 84)"));

        verticalLayout->addWidget(pushButton_calibCamRig);

        pushButton_preproc = new QPushButton(verticalLayoutWidget);
        pushButton_preproc->setObjectName(QStringLiteral("pushButton_preproc"));
        pushButton_preproc->setStyleSheet(QStringLiteral("background-color: rgb(255, 185, 84)"));

        verticalLayout->addWidget(pushButton_preproc);

        pushButton_reproc = new QPushButton(verticalLayoutWidget);
        pushButton_reproc->setObjectName(QStringLiteral("pushButton_reproc"));
        pushButton_reproc->setStyleSheet(QStringLiteral("background-color: rgb(255, 185, 84)"));

        verticalLayout->addWidget(pushButton_reproc);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        pushButton_save = new QPushButton(verticalLayoutWidget);
        pushButton_save->setObjectName(QStringLiteral("pushButton_save"));
        pushButton_save->setStyleSheet(QStringLiteral("background-color: rgb(100, 172, 200);"));

        verticalLayout->addWidget(pushButton_save);

        pushButton_load = new QPushButton(verticalLayoutWidget);
        pushButton_load->setObjectName(QStringLiteral("pushButton_load"));
        pushButton_load->setStyleSheet(QStringLiteral("background-color: rgb(100, 172, 200);"));

        verticalLayout->addWidget(pushButton_load);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 400, 25));
        menuParameters = new QMenu(menuBar);
        menuParameters->setObjectName(QStringLiteral("menuParameters"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuParameters->menuAction());
        menuParameters->addAction(actionStereoM_Params);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        actionStereoM_Params->setText(QApplication::translate("MainWindow", "StereoM. Params", 0));
        pushButton_calibCams->setText(QApplication::translate("MainWindow", "Calibrate Cameras", 0));
        pushButton_calibCamRig->setText(QApplication::translate("MainWindow", "Calibrate Cam Rig", 0));
        pushButton_preproc->setText(QApplication::translate("MainWindow", "Pre-Process", 0));
        pushButton_reproc->setText(QApplication::translate("MainWindow", "Re-Process", 0));
        pushButton_save->setText(QApplication::translate("MainWindow", "Save Camera Parameters", 0));
        pushButton_load->setText(QApplication::translate("MainWindow", "Load Camera Parameters", 0));
        menuParameters->setTitle(QApplication::translate("MainWindow", "Config", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H