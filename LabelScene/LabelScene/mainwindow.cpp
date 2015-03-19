#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <unistd.h>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_btn_load_released()
{
	imgOrig = imread("img.png", IMREAD_GRAYSCALE);
	equalizeHist(img, img);



	destroyAllWindows();
	namedWindow("image", WINDOW_AUTOSIZE);
	setMouseCallback("image", mouseCallback, NULL);
}

void MainWindow::drawAndShow()
{


	imshow("image", imgDraw);
}

void MainWindow::mouseCallback(int event, int x, int y, int flags, void* param)
{
	MainWindow *self = static_cast<MainWindow*>(param);
	self->doMouseCallback(event, x, y, flags);
}

void MainWindow::doMouseCallback(int event, int x, int y, int flags)
{
	 if  ( event == EVENT_LBUTTONDOWN )
	 {
//		  qDebug() << "Left button of the mouse is clicked - position (" << x << ", " << y << ")";
	 }
//	 else if  ( event == EVENT_RBUTTONDOWN )
//	 {
//		  qDebug() << "Right button of the mouse is clicked - position (" << x << ", " << y << ")";
//	 }
//	 else if  ( event == EVENT_MBUTTONDOWN )
//	 {
//		  qDebug() << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")";
//	 }
	 else if ( event == EVENT_MOUSEMOVE )
	 {
		  qDebug() << "Mouse move over the window - position (" << x << ", " << y << ")";

	 }
}
