#include <QDir>
#include <QDateTime>
#include "rgb_nir_depth_capture.h"
#include "ui_rgbnird_mainwindow.h"
#include "prosilicaworker.h"
#include "goldeyeworker.h"
#include "kinectworker.h"

RGB_NIR_Depth_Capture::RGB_NIR_Depth_Capture(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::RGBNIRD_MainWindow),
	triggerSave(false), triggerSaveIR_RGB_pair(false),
	capturingSeries(false), countingDown(false), countdownSeconds(0), countdownTime(0), seriesCnt(0), seriesMax(0), seriesInterval(0),
	ptr_RGBScene(QSharedPointer<QGraphicsScene>(new QGraphicsScene)),
	ptr_NIRScene(QSharedPointer<QGraphicsScene>(new QGraphicsScene)),
	ptr_depthScene(QSharedPointer<QGraphicsScene>(new QGraphicsScene)),
	sound_click("sound/cameraClick.wav"), sound_beep("sound/beep.wav"), sound_beep2("sound/beep2.wav")
{
	ui->setupUi(this);

	//start the worker threads and connect signals:
	qRegisterMetaType<RGBDNIR_MAP>();

	//Prosilica worker
	myImgAcqWorker1 = new ProsilicaWorker();
	myImgAcqWorker1->moveToThread(&workerThread1);
	connect(&workerThread1, SIGNAL(finished()), myImgAcqWorker1, SLOT(deleteLater()));
	connect(this, SIGNAL(startImgAcquisition()), myImgAcqWorker1, SLOT(startAcquisition()));
	connect(myImgAcqWorker1, SIGNAL(imagesReady(RGBDNIR_MAP)), this, SLOT(imagesReady(RGBDNIR_MAP)));
	connect(&workerThread1, SIGNAL(finished()), &workerThread1, SLOT(deleteLater()));
	workerThread1.start(QThread::HighPriority);

	//Goldeye worker
	myImgAcqWorker2 = new GoldeyeWorker();
	myImgAcqWorker2->moveToThread(&workerThread2);
	connect(&workerThread2, SIGNAL(finished()), myImgAcqWorker2, SLOT(deleteLater()));
	connect(this, SIGNAL(startImgAcquisition()), myImgAcqWorker2, SLOT(startAcquisition()));
	connect(myImgAcqWorker2, SIGNAL(imagesReady(RGBDNIR_MAP)), this, SLOT(imagesReady(RGBDNIR_MAP)));
	connect(&workerThread2, SIGNAL(finished()), &workerThread2, SLOT(deleteLater()));
	workerThread2.start(QThread::HighPriority);

	//Kinect worker
	myImgAcqWorker3 = new KinectWorker();
	myImgAcqWorker3->moveToThread(&workerThread3);
	connect(&workerThread3, SIGNAL(finished()), myImgAcqWorker3, SLOT(deleteLater()));
	connect(this, SIGNAL(startImgAcquisition()), myImgAcqWorker3, SLOT(startAcquisition()));
	connect(myImgAcqWorker3, SIGNAL(imagesReady(RGBDNIR_MAP)), this, SLOT(imagesReady(RGBDNIR_MAP)));
	connect(&workerThread3, SIGNAL(finished()), &workerThread3, SLOT(deleteLater()));
	workerThread3.start(QThread::HighPriority);

	//get image widget sizes for display (-2 because of widget borders)
	width_rgb = ui->graphicsView_RGB->width()-2;
	height_rgb = ui->graphicsView_RGB->height()-2;
	width_nir = ui->graphicsView_NIR->width()-2;
	height_nir = ui->graphicsView_NIR->height()-2;
	width_depth = ui->graphicsView_Depth->width()-2;
	height_depth = ui->graphicsView_Depth->height()-2;
}


RGB_NIR_Depth_Capture::~RGB_NIR_Depth_Capture()
{
	if(!myImgAcqWorker1->isStopped()){ myImgAcqWorker1->setAcquiring(false); }
	if(!myImgAcqWorker2->isStopped()){ myImgAcqWorker2->setAcquiring(false); }
	if(!myImgAcqWorker3->isStopped()){ myImgAcqWorker3->setAcquiring(false); }

	workerThread1.quit();
	workerThread1.wait();

	workerThread2.quit();
	workerThread2.wait();

	workerThread3.quit();
	workerThread3.wait();

	delete ui;
}

/***********************************************
** PUBLIC SLOTS:
************************************************/
void RGB_NIR_Depth_Capture::imagesReady(RGBDNIR_MAP capturedImgs)
{
	threadLock.lock();

	//first join the images from sent by the thread to this slot with all images (by all threads)
	if(capturedImgs.contains(RGB)){ allCapturedImgs[RGB] = capturedImgs[RGB]; }
	if(capturedImgs.contains(Kinect_Depth)){ allCapturedImgs[Kinect_Depth] = capturedImgs[Kinect_Depth]; }
	if(capturedImgs.contains(Kinect_RGB)){ allCapturedImgs[Kinect_RGB] = capturedImgs[Kinect_RGB]; }
	if(capturedImgs.contains(NIR_Dark)){ allCapturedImgs[NIR_Dark] = capturedImgs[NIR_Dark]; }
	if(capturedImgs.contains(NIR_935)){ allCapturedImgs[NIR_935] = capturedImgs[NIR_935]; }
	if(capturedImgs.contains(NIR_1060)){ allCapturedImgs[NIR_1060] = capturedImgs[NIR_1060]; }
	if(capturedImgs.contains(NIR_1300)){ allCapturedImgs[NIR_1300] = capturedImgs[NIR_1300]; }
	if(capturedImgs.contains(NIR_1550)){ allCapturedImgs[NIR_1550] = capturedImgs[NIR_1550]; }
	if(triggerSaveIR_RGB_pair && capturedImgs.contains(Kinect_IR))
	{
		allCapturedImgs[Kinect_IR] = capturedImgs[Kinect_IR];
	}


	QMapIterator<RGBDNIR_captureType, Mat> i(allCapturedImgs);
	while(i.hasNext())
	{
		i.next();
		Mat img = i.value();
		RGBDNIR_captureType type = (RGBDNIR_captureType)i.key();
		QString windowName = VimbaCamManager::getRGBDNIR_captureTypeString( type );

		//show the "most important" images in the GUI
		Mat imgSmall;

		if(type == RGB)
		{
			//make a resized copy of the image according to graphic widget size
			cv::resize(img, imgSmall, Size(width_rgb,height_rgb));

			//show in widget width inverted channels (because Mat is BGR and QImage is RGB)
			QImage qimg(imgSmall.data, imgSmall.cols, imgSmall.rows, imgSmall.step, QImage::Format_RGB888);
			ptr_RGBScene = QSharedPointer<QGraphicsScene>(new QGraphicsScene);//drop last pointer to free memory
			ptr_RGBScene->addPixmap(QPixmap::fromImage(qimg.rgbSwapped()));
			ui->graphicsView_RGB->setScene(ptr_RGBScene.data());
		}
		else if(type == NIR_1300)
		{
			//make a resized copy of the image according to graphic widget size
			cv::resize(img, imgSmall, Size(width_nir,height_nir));

			//convert and scale from 16 to 8 bit so image can be displayed
			Mat img8bit(imgSmall.rows, imgSmall.cols, CV_8UC1);
			Mat imgRGB8(imgSmall.rows, imgSmall.cols, CV_8UC3);
//			double minVal, maxVal;
//			minMaxLoc(imgSmall, &minVal, &maxVal); //find minimum and maximum intensities
//			imgSmall.convertTo(img8bit, CV_8U, 255.0/(maxVal - minVal), -minVal * 255.0/(maxVal - minVal));
			imgSmall.convertTo(img8bit, CV_8U, 0.016, 0); // 0.016 = 255/16000, assuming 16k as max value
			cvtColor(img8bit, imgRGB8, CV_GRAY2RGB);

			//show in widget width inverted channels (because Mat is BGR and QImage is RGB)
			QImage qimg(imgRGB8.data, imgRGB8.cols, imgRGB8.rows, imgRGB8.step, QImage::Format_RGB888);
			ptr_NIRScene = QSharedPointer<QGraphicsScene>(new QGraphicsScene);//drop last pointer to free memory
			ptr_NIRScene->addPixmap(QPixmap::fromImage(qimg));
			ui->graphicsView_NIR->setScene(ptr_NIRScene.data());
		}
		else if(type == Kinect_Depth)
		{
			//make a resized copy of the image according to graphic widget size
			cv::resize(img, imgSmall, Size(width_depth,height_depth));
			cvtColor(imgSmall, imgSmall, CV_GRAY2RGB);

			//show in widget width inverted channels (because Mat is BGR and QImage is RGB)
			QImage qimg(imgSmall.data, imgSmall.cols, imgSmall.rows, imgSmall.step, QImage::Format_RGB888);
			ptr_depthScene = QSharedPointer<QGraphicsScene>(new QGraphicsScene);//drop last pointer to free memory
			ptr_depthScene->addPixmap(QPixmap::fromImage(qimg));
			ui->graphicsView_Depth->setScene(ptr_depthScene.data());
		}

		//show all channels in extra windows if user wants that
		if(ui->checkBox_showAllChannels->isChecked())
		{
			namedWindow(windowName.toStdString(), CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO | CV_GUI_EXPANDED);
			imshow(windowName.toStdString(), img);
		}
	}

	//if set, capture a series of images.
	if(capturingSeries){ captureSeries(); }

	//save current image list if save button is clicked or captureSeries() is triggered
	if(triggerSave)
	{
		sound_click.play();

		QDir dir;
		if(!dir.exists(QDir::currentPath()+"/out")){ dir.mkdir(QDir::currentPath()+"/out"); }
		i.toFront();
		QString dateTime_str = getUniquePrefixFromDateAndTime();

		while(i.hasNext())
		{
			i.next();
			QString nm = QDir::currentPath() + "/out/" +
						 dateTime_str + "_" +
						 VimbaCamManager::getRGBDNIR_captureTypeString( (RGBDNIR_captureType)i.key() ) + ".png";
			imwrite(nm.toStdString().c_str(), i.value());
		}
		triggerSave = false;
	}

	//save pair of kinect RGB and IR images if triggered
	if(triggerSaveIR_RGB_pair)
	{
		if(allCapturedImgs.contains(Kinect_IR) && allCapturedImgs.contains(Kinect_RGB))
		{
			sound_click.play();

			QString dateTime_str = getUniquePrefixFromDateAndTime();
			QString nmRGB = QDir::currentPath() + "/out/" + dateTime_str + "_kinectPairRGB.png";
			QString nmIR = QDir::currentPath() + "/out/" + dateTime_str + "_kinectPairIR.png";
			imwrite(nmRGB.toStdString(), allCapturedImgs[Kinect_RGB]);
			imwrite(nmIR.toStdString(), allCapturedImgs[Kinect_IR]);

			//remove ir image so it isn't saved twice
			allCapturedImgs.remove(Kinect_IR);
			triggerSaveIR_RGB_pair = false;
		}
	}

	threadLock.unlock();
}



void RGB_NIR_Depth_Capture::captureSeries()
{
	//beep every second
	if(countingDown && myBeepTimer.elapsed() > 1000)
	{
		if(--countdownSeconds == 1) { sound_beep2.play(); }
		else { sound_beep.play(); }
		myBeepTimer.restart();
	}

	//check if coundown is over
	if(countingDown &&  myTimer.elapsed() > countdownTime)
	{
		countingDown = false;
		triggerSave = true; //first image
		seriesCnt++;
		myTimer.restart();
	}

	//if all pictures have been taken, stop
	if(seriesCnt >= seriesMax)
	{
		capturingSeries = false;
		return;
	}

	//after countdown capture n images every x milliseconds
	if(!countingDown)
	{
		if(myTimer.elapsed() > seriesInterval)
		{
			triggerSave = true;
			seriesCnt++;
			myTimer.restart();
		}
	}
}


QString RGB_NIR_Depth_Capture::getUniquePrefixFromDateAndTime()
{
	QDateTime now = QDateTime::currentDateTime();
	return QString::number(now.date().year())
		   + QString("%1").arg(now.date().month(), 2, 10, QChar('0'))
		   + QString("%1").arg(now.date().day(), 2, 10, QChar('0')) + "_"
		   + QString("%1").arg(now.time().hour(), 2, 10, QChar('0'))
		   + QString("%1").arg(now.time().minute(), 2, 10, QChar('0'))
		   + QString("%1").arg(now.time().second(), 2, 10, QChar('0'))
		   + QString("%1").arg(now.time().msec()/10, 2, 10, QChar('0'));
}


/***********************************************
** GUI Methods:
************************************************/
void RGB_NIR_Depth_Capture::on_btn_startAcquisition_released()
{
	myImgAcqWorker1->setAcquiring(true);
	myImgAcqWorker2->setAcquiring(true);
	myImgAcqWorker3->setAcquiring(true);
	emit startImgAcquisition();
}

void RGB_NIR_Depth_Capture::on_btn_saveImgs_released()
{
	triggerSave = true;
}

void RGB_NIR_Depth_Capture::on_btn_stopAcquisition_released()
{
	myImgAcqWorker1->setAcquiring(false);
	myImgAcqWorker2->setAcquiring(false);
	myImgAcqWorker3->setAcquiring(false);
}

void RGB_NIR_Depth_Capture::on_checkBox_showAllChannels_clicked()
{
	//if unchecked, close all opencv windows
	if(!ui->checkBox_showAllChannels->isChecked())
	{
		destroyAllWindows();
	}
}

void RGB_NIR_Depth_Capture::on_pushButton_saveSeries_released()
{
	capturingSeries = true;
	countingDown = true;
	countdownSeconds = ui->lineEdit_countdown->text().toInt();
	countdownTime = countdownSeconds * 1000;
	seriesCnt = 0;
	seriesMax = ui->lineEdit_amount->text().toInt();
	seriesInterval = ui->lineEdit_interval->text().toDouble() * 1000.0;
	myTimer.start();
	myBeepTimer.start();
	sound_beep.play();
}

void RGB_NIR_Depth_Capture::on_btn_saveIR_RGB_pair_released()
{
	triggerSaveIR_RGB_pair = true;
	((KinectWorker*)myImgAcqWorker3)->triggerIRcapture();
}
