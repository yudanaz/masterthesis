/********************************************************************************
** Form generated from reading UI file 'stereomatching_params.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STEREOMATCHING_PARAMS_H
#define UI_STEREOMATCHING_PARAMS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_StereoMatching_params
{
public:
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *label_3;
    QLineEdit *lineEdit_optType;
    QLabel *label_2;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLineEdit *lineEdit_wtaThresh;
    QLineEdit *lineEdit_SADwin;
    QLineEdit *lineEdit_uniqueness;
    QLineEdit *lineEdit_speckleWin;
    QLineEdit *lineEdit_speckleRange;
    QLabel *label_8;
    QLineEdit *lineEdit_preFilterCAP;
    QLabel *label;
    QLabel *label_9;
    QLineEdit *lineEdit_maxDisparity;
    QLineEdit *lineEdit_minDisparity;

    void setupUi(QWidget *StereoMatching_params)
    {
        if (StereoMatching_params->objectName().isEmpty())
            StereoMatching_params->setObjectName(QStringLiteral("StereoMatching_params"));
        StereoMatching_params->resize(270, 320);
        StereoMatching_params->setStyleSheet(QStringLiteral("background-color: rgb(231, 203, 173);"));
        formLayoutWidget = new QWidget(StereoMatching_params);
        formLayoutWidget->setObjectName(QStringLiteral("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(10, 10, 238, 299));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        formLayout->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(formLayoutWidget);
        label_3->setObjectName(QStringLiteral("label_3"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label_3);

        lineEdit_optType = new QLineEdit(formLayoutWidget);
        lineEdit_optType->setObjectName(QStringLiteral("lineEdit_optType"));

        formLayout->setWidget(0, QFormLayout::FieldRole, lineEdit_optType);

        label_2 = new QLabel(formLayoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_2);

        label_4 = new QLabel(formLayoutWidget);
        label_4->setObjectName(QStringLiteral("label_4"));

        formLayout->setWidget(5, QFormLayout::LabelRole, label_4);

        label_5 = new QLabel(formLayoutWidget);
        label_5->setObjectName(QStringLiteral("label_5"));

        formLayout->setWidget(7, QFormLayout::LabelRole, label_5);

        label_6 = new QLabel(formLayoutWidget);
        label_6->setObjectName(QStringLiteral("label_6"));

        formLayout->setWidget(8, QFormLayout::LabelRole, label_6);

        label_7 = new QLabel(formLayoutWidget);
        label_7->setObjectName(QStringLiteral("label_7"));

        formLayout->setWidget(9, QFormLayout::LabelRole, label_7);

        lineEdit_wtaThresh = new QLineEdit(formLayoutWidget);
        lineEdit_wtaThresh->setObjectName(QStringLiteral("lineEdit_wtaThresh"));

        formLayout->setWidget(2, QFormLayout::FieldRole, lineEdit_wtaThresh);

        lineEdit_SADwin = new QLineEdit(formLayoutWidget);
        lineEdit_SADwin->setObjectName(QStringLiteral("lineEdit_SADwin"));

        formLayout->setWidget(5, QFormLayout::FieldRole, lineEdit_SADwin);

        lineEdit_uniqueness = new QLineEdit(formLayoutWidget);
        lineEdit_uniqueness->setObjectName(QStringLiteral("lineEdit_uniqueness"));

        formLayout->setWidget(7, QFormLayout::FieldRole, lineEdit_uniqueness);

        lineEdit_speckleWin = new QLineEdit(formLayoutWidget);
        lineEdit_speckleWin->setObjectName(QStringLiteral("lineEdit_speckleWin"));

        formLayout->setWidget(8, QFormLayout::FieldRole, lineEdit_speckleWin);

        lineEdit_speckleRange = new QLineEdit(formLayoutWidget);
        lineEdit_speckleRange->setObjectName(QStringLiteral("lineEdit_speckleRange"));

        formLayout->setWidget(9, QFormLayout::FieldRole, lineEdit_speckleRange);

        label_8 = new QLabel(formLayoutWidget);
        label_8->setObjectName(QStringLiteral("label_8"));

        formLayout->setWidget(6, QFormLayout::LabelRole, label_8);

        lineEdit_preFilterCAP = new QLineEdit(formLayoutWidget);
        lineEdit_preFilterCAP->setObjectName(QStringLiteral("lineEdit_preFilterCAP"));

        formLayout->setWidget(6, QFormLayout::FieldRole, lineEdit_preFilterCAP);

        label = new QLabel(formLayoutWidget);
        label->setObjectName(QStringLiteral("label"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label);

        label_9 = new QLabel(formLayoutWidget);
        label_9->setObjectName(QStringLiteral("label_9"));

        formLayout->setWidget(4, QFormLayout::LabelRole, label_9);

        lineEdit_maxDisparity = new QLineEdit(formLayoutWidget);
        lineEdit_maxDisparity->setObjectName(QStringLiteral("lineEdit_maxDisparity"));

        formLayout->setWidget(4, QFormLayout::FieldRole, lineEdit_maxDisparity);

        lineEdit_minDisparity = new QLineEdit(formLayoutWidget);
        lineEdit_minDisparity->setObjectName(QStringLiteral("lineEdit_minDisparity"));

        formLayout->setWidget(3, QFormLayout::FieldRole, lineEdit_minDisparity);


        retranslateUi(StereoMatching_params);

        QMetaObject::connectSlotsByName(StereoMatching_params);
    } // setupUi

    void retranslateUi(QWidget *StereoMatching_params)
    {
        StereoMatching_params->setWindowTitle(QApplication::translate("StereoMatching_params", "Form", 0));
        label_3->setText(QApplication::translate("StereoMatching_params", "Optimization (WTA/SGM)", 0));
        lineEdit_optType->setText(QApplication::translate("StereoMatching_params", "1", 0));
        label_2->setText(QApplication::translate("StereoMatching_params", "WTA  disp. thresh.:", 0));
        label_4->setText(QApplication::translate("StereoMatching_params", "SAD window", 0));
        label_5->setText(QApplication::translate("StereoMatching_params", "Uniqueness", 0));
        label_6->setText(QApplication::translate("StereoMatching_params", "Speckle Window", 0));
        label_7->setText(QApplication::translate("StereoMatching_params", "Speckle Range", 0));
        lineEdit_wtaThresh->setText(QApplication::translate("StereoMatching_params", "5.0", 0));
        lineEdit_SADwin->setText(QApplication::translate("StereoMatching_params", "3", 0));
        lineEdit_uniqueness->setText(QApplication::translate("StereoMatching_params", "1", 0));
        lineEdit_speckleWin->setText(QApplication::translate("StereoMatching_params", "7", 0));
        lineEdit_speckleRange->setText(QApplication::translate("StereoMatching_params", "4", 0));
        label_8->setText(QApplication::translate("StereoMatching_params", "Pre-Filter Cap", 0));
        lineEdit_preFilterCAP->setText(QApplication::translate("StereoMatching_params", "11", 0));
        label->setText(QApplication::translate("StereoMatching_params", "Minimal Disparity:", 0));
        label_9->setText(QApplication::translate("StereoMatching_params", "Maximal Disparity:", 0));
        lineEdit_maxDisparity->setText(QApplication::translate("StereoMatching_params", "64", 0));
        lineEdit_minDisparity->setText(QApplication::translate("StereoMatching_params", "8", 0));
    } // retranslateUi

};

namespace Ui {
    class StereoMatching_params: public Ui_StereoMatching_params {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STEREOMATCHING_PARAMS_H
