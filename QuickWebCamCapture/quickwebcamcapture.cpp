#include "quickwebcamcapture.h"
#include "ui_quickwebcamcapture.h"

QuickWebCamCapture::QuickWebCamCapture(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::QuickWebCamCapture)
{
	ui->setupUi(this);

	VideoCapture capture0 = VideoCapture(0);
	VideoCapture capture1 = VideoCapture(1);

	Mat img0, img1, img0Display, img1Display;
	namedWindow("image0");
	namedWindow("image1");
	int key;

	while(true)
	{
		//capture images
		capture0 >> img0;
		capture1 >> img1;

//		QImage qimg1 = QImage((uchar*)img1.data, img1.cols, img1.rows, QImage::Format_RGB888);
//		QImage qimg2 = QImage((uchar*)img2.data, img2.cols, img2.rows, QImage::Format_RGB888);
//		QGraphicsScene *scene1 = new QGraphicsScene(this);
//		QGraphicsScene *scene2 = new QGraphicsScene(this);
//		scene1->addPixmap(QPixmap::fromImage(qimg1));
//		scene2->addPixmap(QPixmap::fromImage(qimg2));

//		ui->graphicsView1->setScene(scene1);

		//show images flipped around y-axis ("mirror" effect)
		flip(img0, img0Display, 1);
		flip(img1, img1Display, 1);
		imshow("image0", img0Display);
		imshow("image1", img1Display);

		//get user input for saving and terminating capture
		key = cvWaitKey(1);
		if(key == 1048608) //SPACE
		{
			QString nm = QTime::currentTime().toString().remove(":");
			QString nm0 = "0_" + nm + ".png";
			QString nm1 = "1_" + nm + ".png";
			imwrite((QDir::currentPath() +"/out/"+  nm0).toStdString().c_str(), img0);
			imwrite((QDir::currentPath() +"/out/"+  nm1).toStdString().c_str(), img1);

			qDebug() << "Saved images " << nm0 << " and " << nm1;
		}
		else if(key == 1048603 ) //ESC
		{
			//qDebug() << key;
			break;
		}

	}
}

QuickWebCamCapture::~QuickWebCamCapture()
{
	delete ui;
}
