#include "mainwindow_imgviewer.h"
#include "ui_mainwindow_imgviewer.h"
#include "opencv2/opencv.hpp"
using namespace cv;

MainWindow_imgViewer::MainWindow_imgViewer(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow_imgViewer),
	imgIndex(0), imgMax(0),
	nm_rgb(""), nm_nir(""), nm_depth("")
{
	ui->setupUi(this);
}

MainWindow_imgViewer::~MainWindow_imgViewer()
{
	delete ui;
}

void MainWindow_imgViewer::on_pushButton_open_released()
{
	QString dirURL = QFileDialog::getExistingDirectory(this, "Select directory", QDir::homePath());
	if(dirURL == ""){ return; }
	qDebug() << "selected" << dirURL;
	QDirIterator dirIt(dirURL);
	imageURLs.clear();
	while(dirIt.hasNext())
	{
		QString imgURL = dirIt.next();
		if(imgURL.contains("NIR_MultiCh.png")){ imageURLs.append(imgURL); }
	}
	showImage();
}


void MainWindow_imgViewer::showImage()
{
	nm_nir = imageURLs[imgIndex];
	nm_rgb = nm_nir;
	nm_rgb.remove("NIR_MultiCh.png").append("RGB.png");
	nm_depth = nm_nir;
	nm_depth.remove("NIR_MultiCh.png").append("Kinect_Depth.png");
	nm_dark = nm_nir;
	nm_dark.remove("NIR_MultiCh.png").append("NIR_Dark.png");
	Mat nir = imread(nm_nir.toStdString(), IMREAD_COLOR);
	Mat rgb = imread(nm_rgb.toStdString(), IMREAD_COLOR);
	Mat depth16 = imread(nm_depth.toStdString(), IMREAD_ANYDEPTH);

	//show which image is shown
	QString showName = imageURLs[imgIndex];
	showName.remove("NIR_MultiCh.png");
	ui->label_imageName->setText(showName.split("/").last());

	//convert depth to mm
	Mat depth(depth16.size(), CV_8UC1);

	MatIterator_<ushort> it, end;
	MatIterator_<uchar> it8bit;
	for( it = depth16.begin<ushort>(), end = depth16.end<ushort>(), it8bit = depth.begin<uchar>();
		 it != end; ++it, ++it8bit)
	{
		float mm = 0;
		if (*it < 2047){ mm = 1.0 / (*it * -0.0030711016 + 3.3309495161) * 1000.0; }
		*it8bit = mm / 10000.0 * 255;
	}
	Mat depthColor;
	cvtColor(depth, depthColor, CV_GRAY2RGB);

	//resize
	Mat rgb2, depth2;
	cv::resize(rgb, rgb2, Size(300,300));
	cv::resize(depthColor, depth2, Size(300,225));

	QImage qimg_nir(nir.data, nir.cols, nir.rows, nir.step, QImage::Format_RGB888);
	QImage qimg_rgb(rgb2.data, rgb2.cols, rgb2.rows, rgb2.step, QImage::Format_RGB888);
	QImage qimg_depth(depth2.data, depth2.cols, depth2.rows, depth2.step, QImage::Format_RGB888);

	ptr_NIRScene = QSharedPointer<QGraphicsScene>(new QGraphicsScene);//drop last pointer to free memory
	ptr_RGBScene = QSharedPointer<QGraphicsScene>(new QGraphicsScene);//drop last pointer to free memory
	ptr_DepthScene = QSharedPointer<QGraphicsScene>(new QGraphicsScene);//drop last pointer to free memory

	ptr_NIRScene->addPixmap(QPixmap::fromImage(qimg_nir.rgbSwapped()));
	ptr_RGBScene->addPixmap(QPixmap::fromImage(qimg_rgb.rgbSwapped()));
	ptr_DepthScene->addPixmap(QPixmap::fromImage(qimg_depth));

	ui->graphicsView_NIR->setScene(ptr_NIRScene.data());
	ui->graphicsView_RGB->setScene(ptr_RGBScene.data());
	ui->graphicsView_depth->setScene(ptr_DepthScene.data());
}

QString MainWindow_imgViewer::makeFileURLInNewDir(QString url, QString dirFromHere)
{
	QStringList sl = url.split("/");
	QString newUrl = "";
	for (int i = 0; i < sl.size()-1; ++i)
	{
		newUrl += sl[i] + "/";
	}
	newUrl += dirFromHere + "/" + sl.last();
	return newUrl;
}

void MainWindow_imgViewer::on_pushButton_previous_released()
{
	if(--imgIndex < 0){ imgIndex = imageURLs.size() -1; }
	showImage();
}

void MainWindow_imgViewer::on_pushButton_next_released()
{
	imgIndex = (imgIndex+1) % imageURLs.size();
	showImage();
}

void MainWindow_imgViewer::on_pushButton_markAsBad_released()
{
	//move files
	QString nm_nir2 = makeFileURLInNewDir(nm_nir, "bad");
	QString nm_dark2 = makeFileURLInNewDir(nm_dark, "bad");
	QString nm_rgb2 = makeFileURLInNewDir(nm_rgb, "bad");
	QString nm_depth2 = makeFileURLInNewDir(nm_depth, "bad");
	QDir dir;
	bool success;
	success = dir.rename(nm_nir, nm_nir2);
	success = dir.rename(nm_dark, nm_dark2);
	success = dir.rename(nm_rgb, nm_rgb2);
	success = dir.rename(nm_depth, nm_depth2);

	//remove from list
	if(success)
	{
		imageURLs.removeAt(imgIndex);

		if(imageURLs.size() != 0)
		{
			if(--imgIndex < 0){ imgIndex = imageURLs.size() -1; }
			showImage();
		}
		else
		{
			ui->label_imageName->setText("DIRECTORY IS EMPTY!!!");
		}
	}
}

