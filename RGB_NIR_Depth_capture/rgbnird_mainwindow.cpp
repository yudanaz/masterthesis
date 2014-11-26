#include "rgbnird_mainwindow.h"
#include "ui_rgbnird_mainwindow.h"

RGBNIRD_MainWindow::RGBNIRD_MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::RGBNIRD_MainWindow)
{
	ui->setupUi(this);
}

RGBNIRD_MainWindow::~RGBNIRD_MainWindow()
{
	delete ui;
}
