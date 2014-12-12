#include "rgb_nir_depth_capture.h"
#include "ui_rgbnird_mainwindow.h"

RGB_NIR_Depth_Capture::RGB_NIR_Depth_Capture(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::RGBNIRD_MainWindow),
	acquiring(false)
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
	while(true)
	{
		QList<Mat> images = vimbaCamManager.getCamImages();
		int i;
		for(i = 0; i < images.length(); ++i)
		{
			Mat img = images[i];
			QString windowName = "image " + QString::number(i);
			Mat img8bit(img.rows, img.cols, CV_8UC3);
			if(img.type() != CV_8UC3)
			{
				img.convertTo(img8bit, CV_8UC3);
			}
			else{ img8bit = img; }
			namedWindow(windowName.toStdString(), CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO | CV_GUI_EXPANDED);
			imshow(windowName.toStdString(), img);
		}

		//if any key is pressed, stop
		if(waitKey(1) != -1){ return; }
	}
}

/***********************************************
** GUI Methods:
************************************************/
void RGB_NIR_Depth_Capture::on_btn_acquireImage_released()
{
	acquireImages();
}

