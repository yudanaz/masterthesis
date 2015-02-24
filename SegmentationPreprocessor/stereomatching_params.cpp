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
