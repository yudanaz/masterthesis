#include "rgb_nir_depth_capture.h"
#include "ui_rgbnird_mainwindow.h"

RGB_NIR_Depth_Capture::RGB_NIR_Depth_Capture(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::RGBNIRD_MainWindow)
{
	ui->setupUi(this);

	vimbaCamManager.detectCameras();
}

RGB_NIR_Depth_Capture::~RGB_NIR_Depth_Capture()
{
	delete ui;
}

void RGB_NIR_Depth_Capture::acquireImages()
{
	QList<Mat> images = vimbaCamManager.getCamImages();
	int imgNr = 0;
	foreach(Mat img, images)
	{
		QString windowName = "image " + QString::number(imgNr);
		Mat img8bit(img.rows, img.cols, CV_8UC1);
		img.convertTo(img8bit, CV_8UC1);
		imshow(windowName.toStdString(), img);
	}
}

/***********************************************
** GUI Methods:
************************************************/
void RGB_NIR_Depth_Capture::on_btn_acquireImage_released()
{
	acquireImages();
}

