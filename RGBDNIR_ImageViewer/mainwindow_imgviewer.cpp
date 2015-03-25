#include "mainwindow_imgviewer.h"
#include "ui_mainwindow_imgviewer.h"

MainWindow_imgViewer::MainWindow_imgViewer(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow_imgViewer)
{
	ui->setupUi(this);
}

MainWindow_imgViewer::~MainWindow_imgViewer()
{
	delete ui;
}
