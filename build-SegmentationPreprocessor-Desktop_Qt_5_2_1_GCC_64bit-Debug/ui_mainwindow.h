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
    QAction *actionNormalize_Depth;
    QAction *actionMake_Skine_Binary_Image;
    QAction *actionMake_Cross_Spectral_Stereo;
    QAction *actionDistort;
    QAction *actionThin_plate_spline;
    QWidget *centralWidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton_calibCams;
    QPushButton *pushButton_calibCamRig;
    QPushButton *pushButton_preproc;
    QPushButton *pushButton_reproc;
    QSpacerItem *verticalSpacer_2;
    QPushButton *pushButton_openFomeFolder;
    QSpacerItem *verticalSpacer;
    QPushButton *pushButton_save;
    QPushButton *pushButton_load;
    QMenuBar *menuBar;
    QMenu *menuParameters;
    QMenu *menuPreproc_Options;
    QMenu *menuRGB_registration;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(400, 346);
        MainWindow->setStyleSheet(QStringLiteral("background-color: rgb(196, 201, 222)"));
        actionStereoM_Params = new QAction(MainWindow);
        actionStereoM_Params->setObjectName(QStringLiteral("actionStereoM_Params"));
        actionNormalize_Depth = new QAction(MainWindow);
        actionNormalize_Depth->setObjectName(QStringLiteral("actionNormalize_Depth"));
        actionNormalize_Depth->setCheckable(true);
        actionNormalize_Depth->setChecked(true);
        actionMake_Skine_Binary_Image = new QAction(MainWindow);
        actionMake_Skine_Binary_Image->setObjectName(QStringLiteral("actionMake_Skine_Binary_Image"));
        actionMake_Skine_Binary_Image->setCheckable(true);
        actionMake_Cross_Spectral_Stereo = new QAction(MainWindow);
        actionMake_Cross_Spectral_Stereo->setObjectName(QStringLiteral("actionMake_Cross_Spectral_Stereo"));
        actionMake_Cross_Spectral_Stereo->setCheckable(true);
        actionMake_Cross_Spectral_Stereo->setChecked(true);
        actionDistort = new QAction(MainWindow);
        actionDistort->setObjectName(QStringLiteral("actionDistort"));
        actionDistort->setCheckable(true);
        actionThin_plate_spline = new QAction(MainWindow);
        actionThin_plate_spline->setObjectName(QStringLiteral("actionThin_plate_spline"));
        actionThin_plate_spline->setCheckable(true);
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

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        pushButton_openFomeFolder = new QPushButton(verticalLayoutWidget);
        pushButton_openFomeFolder->setObjectName(QStringLiteral("pushButton_openFomeFolder"));

        verticalLayout->addWidget(pushButton_openFomeFolder);

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
        menuBar->setGeometry(QRect(0, 0, 400, 20));
        menuParameters = new QMenu(menuBar);
        menuParameters->setObjectName(QStringLiteral("menuParameters"));
        menuPreproc_Options = new QMenu(menuParameters);
        menuPreproc_Options->setObjectName(QStringLiteral("menuPreproc_Options"));
        menuRGB_registration = new QMenu(menuPreproc_Options);
        menuRGB_registration->setObjectName(QStringLiteral("menuRGB_registration"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuParameters->menuAction());
        menuParameters->addAction(actionStereoM_Params);
        menuParameters->addAction(menuPreproc_Options->menuAction());
        menuPreproc_Options->addAction(actionNormalize_Depth);
        menuPreproc_Options->addAction(actionMake_Skine_Binary_Image);
        menuPreproc_Options->addAction(actionMake_Cross_Spectral_Stereo);
        menuPreproc_Options->addAction(menuRGB_registration->menuAction());
        menuRGB_registration->addAction(actionDistort);
        menuRGB_registration->addAction(actionThin_plate_spline);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        actionStereoM_Params->setText(QApplication::translate("MainWindow", "StereoM. Params", 0));
        actionNormalize_Depth->setText(QApplication::translate("MainWindow", "normalize Depth", 0));
        actionMake_Skine_Binary_Image->setText(QApplication::translate("MainWindow", "make Skin Binary Image", 0));
        actionMake_Cross_Spectral_Stereo->setText(QApplication::translate("MainWindow", "make Cross-Spectral Stereo", 0));
        actionDistort->setText(QApplication::translate("MainWindow", "distort perspective", 0));
        actionThin_plate_spline->setText(QApplication::translate("MainWindow", "thin plate spline", 0));
        pushButton_calibCams->setText(QApplication::translate("MainWindow", "Calibrate Cameras", 0));
        pushButton_calibCamRig->setText(QApplication::translate("MainWindow", "Calibrate Cam Rig", 0));
        pushButton_preproc->setText(QApplication::translate("MainWindow", "Pre-Process", 0));
        pushButton_reproc->setText(QApplication::translate("MainWindow", "Re-Process", 0));
        pushButton_openFomeFolder->setText(QApplication::translate("MainWindow", "Open Home Folder", 0));
        pushButton_save->setText(QApplication::translate("MainWindow", "Save Camera Parameters", 0));
        pushButton_load->setText(QApplication::translate("MainWindow", "Load Camera Parameters", 0));
        menuParameters->setTitle(QApplication::translate("MainWindow", "Config", 0));
        menuPreproc_Options->setTitle(QApplication::translate("MainWindow", "Preproc. Options", 0));
        menuRGB_registration->setTitle(QApplication::translate("MainWindow", "RGB registration", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
