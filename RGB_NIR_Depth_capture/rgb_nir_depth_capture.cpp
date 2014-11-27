#include "rgb_nir_depth_capture.h"
#include "ui_rgbnird_mainwindow.h"

RGB_NIR_Depth_Capture::RGB_NIR_Depth_Capture(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::RGBNIRD_MainWindow)
{
	ui->setupUi(this);

	QStringList cams = vimbaCamManager.detectCameras();

	QList<Mat> images = vimbaCamManager.getCamImages();
	int imgNr = 0;
	foreach(Mat img, images)
	{
		QString windowName = "image " + QString::number(imgNr);
		imshow(windowName.toStdString(), img);
	}
}

RGB_NIR_Depth_Capture::~RGB_NIR_Depth_Capture()
{
	delete ui;
}
