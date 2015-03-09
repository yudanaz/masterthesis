#include "stereomatching_params.h"
#include "ui_stereomatching_params.h"

StereoMatching_params::StereoMatching_params(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::StereoMatching_params)
{
	ui->setupUi(this);
}

StereoMatching_params::~StereoMatching_params()
{
	delete ui;
}


std::vector<float> StereoMatching_params::getParams()
{
	std::vector<float> p;
	p.push_back(ui->lineEdit_optType->text().toFloat());
	p.push_back(ui->lineEdit_minDisparity->text().toFloat());
	p.push_back(ui->lineEdit_maxDisparity->text().toFloat());
	p.push_back(ui->lineEdit_wtaThresh->text().toFloat());
	p.push_back(ui->lineEdit_SADwin->text().toFloat());
	p.push_back(ui->lineEdit_preFilterCAP->text().toFloat());
	p.push_back(ui->lineEdit_uniqueness->text().toFloat());
	p.push_back(ui->lineEdit_speckleWin->text().toFloat());
	p.push_back(ui->lineEdit_speckleRange->text().toFloat());

	return p;
}
