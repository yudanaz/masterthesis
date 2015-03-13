#include <QDir>
#include <QDateTime>
#include <QDesktopServices>
#include <QFile>
#include <QTextStream>
#include "rgb_nir_depth_capture.h"
#include "ui_rgbnird_mainwindow.h"

RGB_NIR_Depth_Capture::RGB_NIR_Depth_Capture(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::RGBNIRD_MainWindow),
	acquiring(false), triggerSave(false),
	triggerSwitch_kinectRGB2IR(false), triggerSwitch_kinectIR2RGB(false), capturing_kinectRGB(true),
	capturingSeries(false), countingDown(false), countdownSeconds(0), countdownTime(0), seriesCnt(0), seriesMax(0), seriesInterval(0),
	ptr_RGBScene(QSharedPointer<QGraphicsScene>(new QGraphicsScene)),
	ptr_NIRScene(QSharedPointer<QGraphicsScene>(new QGraphicsScene)),
	ptr_depthScene(QSharedPointer<QGraphicsScene>(new QGraphicsScene)),
	sound_click("misc/cameraClick.wav"), sound_beep("misc/beep.wav"), sound_beep2("misc/beep2.wav"),
	simulatingRGBCalib(true), flipImgs(false)
{
	ui->setupUi(this);

	//start the worker threads and connect signals:
	qRegisterMetaType<RGBDNIR_MAP>();

	//read correct USB port from file
	int usbport = 0;
	QFile f("misc/settings.config");
	if(f.open(QFile::ReadOnly))
	{
		QTextStream ts(&f);
		QStringList sl = ts.readLine().split(" ");
		if(sl.first() == "usbport:"){ usbport = sl.last().toInt(); }
		f.close();
	}
	else
	{
		QMessageBox::information(this, "Error", "No config file found!", QMessageBox::Ok);
	}


	//Prosilica worker
	myImgAcqWorker_Prosilica = new ProsilicaWorker();
	myImgAcqWorker_Prosilica->moveToThread(&workerThread_Prosilica);
	connect(&workerThread_Prosilica, SIGNAL(finished()), myImgAcqWorker_Prosilica, SLOT(deleteLater()));
	connect(this, SIGNAL(startImgAcquisition()), myImgAcqWorker_Prosilica, SLOT(startAcquisition()));
	connect(myImgAcqWorker_Prosilica, SIGNAL(imagesReady(RGBDNIR_MAP)), this, SLOT(imagesReady(RGBDNIR_MAP)));
	connect(&workerThread_Prosilica, SIGNAL(finished()), &workerThread_Prosilica, SLOT(deleteLater()));
	workerThread_Prosilica.start(QThread::HighPriority);

	//Goldeye worker
	myImgAcqWorker_Goldeye = new GoldeyeWorker(usbport);
	myImgAcqWorker_Goldeye->moveToThread(&workerThread_Goldeye);
	connect(&workerThread_Goldeye, SIGNAL(finished()), myImgAcqWorker_Goldeye, SLOT(deleteLater()));
	connect(this, SIGNAL(startImgAcquisition()), myImgAcqWorker_Goldeye, SLOT(startAcquisition()));
	connect(this, SIGNAL(toggleNIR_MultiChannelCapture(bool)), myImgAcqWorker_Goldeye, SLOT(toggleMultiChannelCapture(bool)));
	connect(myImgAcqWorker_Goldeye, SIGNAL(imagesReady(RGBDNIR_MAP)), this, SLOT(imagesReady(RGBDNIR_MAP)));
	connect(&workerThread_Goldeye, SIGNAL(finished()), &workerThread_Goldeye, SLOT(deleteLater()));
	workerThread_Goldeye.start(QThread::HighPriority);

	//Kinect worker
	myImgAcqWorker_Kinect = new KinectWorker();
	myImgAcqWorker_Kinect->moveToThread(&workerThread_Kinect);
	connect(&workerThread_Kinect, SIGNAL(finished()), myImgAcqWorker_Kinect, SLOT(deleteLater()));
	connect(this, SIGNAL(startImgAcquisition()), myImgAcqWorker_Kinect, SLOT(startAcquisition()));
	connect(myImgAcqWorker_Kinect, SIGNAL(imagesReady(RGBDNIR_MAP)), this, SLOT(imagesReady(RGBDNIR_MAP)));
	connect(&workerThread_Kinect, SIGNAL(finished()), &workerThread_Kinect, SLOT(deleteLater()));
	workerThread_Kinect.start(QThread::HighPriority);

	//Thread that saves images
	mySaveImgsWorker = new SaveImgsWorker();
	mySaveImgsWorker->moveToThread(&workerThread_SaveImgs);
	connect(&workerThread_SaveImgs, SIGNAL(finished()), mySaveImgsWorker, SLOT(deleteLater()));
	connect(this, SIGNAL(saveImages(RGBDNIR_MAP,bool,bool,bool,bool,bool,bool)), mySaveImgsWorker, SLOT(saveImgs(RGBDNIR_MAP,bool,bool,bool,bool,bool,bool)));
	connect(&workerThread_SaveImgs, SIGNAL(finished()), &workerThread_SaveImgs, SLOT(deleteLater()));
	workerThread_SaveImgs.start(QThread::HighPriority);

	//get image widget sizes for display (-2 because of widget borders)
	width_win1 = ui->graphicsView1->width()-2;
	height_win1 = ui->graphicsView1->height()-2;
	width_win2 = ui->graphicsView2->width()-2;
	height_win2 = ui->graphicsView2->height()-2;
	width_win3 = ui->graphicsView3->width()-2;
	height_win3 = ui->graphicsView3->height()-2;

	//load parameters for resizing / cropping RGB image
	FileStorage fs("misc/rgbCam.config", FileStorage::READ);
	fs["cam_RGB"] >> cam_RGB;
	fs["distCoeff_RGB"] >> distCoeff_RGB;
	fs["resizeFac_RGB"] >> resizeFac_RGB;
	fs["cropRect_RGB"] >> cropRect_RGB;
	fs.release();

	//compute the rectangle to be drawn on the rgb image, indicating the are which will remain after calibration
	drawRect_RGB.x = cropRect_RGB.x / resizeFac_RGB;
	drawRect_RGB.y = cropRect_RGB.y / resizeFac_RGB;
	drawRect_RGB.width = cropRect_RGB.width / resizeFac_RGB;
	drawRect_RGB.height = cropRect_RGB.height / resizeFac_RGB;
}


RGB_NIR_Depth_Capture::~RGB_NIR_Depth_Capture()
{
	if(!myImgAcqWorker_Prosilica->isStopped()){ myImgAcqWorker_Prosilica->setAcquiring(false); }
	if(!myImgAcqWorker_Goldeye->isStopped()){ myImgAcqWorker_Goldeye->setAcquiring(false); }
	if(!myImgAcqWorker_Kinect->isStopped()){ myImgAcqWorker_Kinect->setAcquiring(false); }

	workerThread_Prosilica.quit();
	workerThread_Prosilica.wait();

	workerThread_Goldeye.quit();
	workerThread_Goldeye.wait();

	workerThread_Kinect.quit();
	workerThread_Kinect.wait();

	delete ui;
}

/***********************************************
** PUBLIC SLOTS:
************************************************/
void RGB_NIR_Depth_Capture::imagesReady(RGBDNIR_MAP capturedImgs)
{
	threadLock.lock();

	//join the images sent by the thread to this slot with all images (by all threads)
	//also remember which images have been sent now, to repaint only those
	vector<RGBDNIR_captureType> sentNow;

	if(capturedImgs.contains(RGB)){ allCapturedImgs[RGB] = capturedImgs[RGB]; sentNow.push_back(RGB); }

	if(capturedImgs.contains(NIR_Dark)){ allCapturedImgs[NIR_Dark] = capturedImgs[NIR_Dark]; sentNow.push_back(NIR_Dark); }
	if(capturedImgs.contains(NIR_970)){ allCapturedImgs[NIR_970] = capturedImgs[NIR_970]; sentNow.push_back(NIR_970); }
	if(capturedImgs.contains(NIR_1300)){ allCapturedImgs[NIR_1300] = capturedImgs[NIR_1300]; sentNow.push_back(NIR_1300); }
	if(capturedImgs.contains(NIR_1550)){ allCapturedImgs[NIR_1550] = capturedImgs[NIR_1550]; sentNow.push_back(NIR_1550); }

	if(capturedImgs.contains(Kinect_Depth)){ allCapturedImgs[Kinect_Depth] = capturedImgs[Kinect_Depth]; sentNow.push_back(Kinect_Depth); }
	if(capturedImgs.contains(Kinect_RGB))
	{
		if(triggerSwitch_kinectIR2RGB)
		{
			allCapturedImgs.clear();
			capturing_kinectRGB = true;
			triggerSwitch_kinectIR2RGB = false;
		}
		allCapturedImgs[Kinect_RGB] = capturedImgs[Kinect_RGB];
		sentNow.push_back(Kinect_RGB);
	}
	if(capturedImgs.contains(Kinect_IR))
	{
		if(triggerSwitch_kinectRGB2IR)
		{
			allCapturedImgs.clear();
			capturing_kinectRGB = false;
			triggerSwitch_kinectRGB2IR = false;
		}
		allCapturedImgs[Kinect_IR] = capturedImgs[Kinect_IR];
		sentNow.push_back(Kinect_IR);
	}

	//repaint the channels that have been sent now
	for (int i = 0; i < sentNow.size(); ++i)
	{
		RGBDNIR_captureType type = (RGBDNIR_captureType)sentNow[i];
		Mat img = allCapturedImgs[type];
		QString windowName = VimbaCamManager::getRGBDNIR_captureTypeString( type );

		//show the "most important" images in the GUI
		Mat imgSmall;

		if(type == RGB)
		{
			//make target cross
			Mat rgbWithCross;
			if(ui->checkBox_drawTargetCross->isChecked())
			{
				rgbWithCross = drawCross(img, 6);
			}
			else{ rgbWithCross = img; }

			//make undistored copy of RGB image and draw rectangle indicating effective area after calibration
			Mat rgbWithRect;
			if(simulatingRGBCalib)
			{
				undistort(rgbWithCross, rgbWithRect, cam_RGB, distCoeff_RGB);
				cv::rectangle(rgbWithRect, drawRect_RGB, Scalar(255,0,0), 3);
			}
			else
			{
				rgbWithRect = rgbWithCross;
			}

			//make a resized copy of the image according to graphic widget size
			cv::resize(rgbWithRect, imgSmall, Size(width_win2,height_win2));
			if(flipImgs){ flip(imgSmall, imgSmall, 1); }

			//show in widget width inverted channels (because Mat is BGR and QImage is RGB)
			QImage qimg(imgSmall.data, imgSmall.cols, imgSmall.rows, imgSmall.step, QImage::Format_RGB888);
			ptr_RGBScene = QSharedPointer<QGraphicsScene>(new QGraphicsScene);//drop last pointer to free memory
			ptr_RGBScene->addPixmap(QPixmap::fromImage(qimg.rgbSwapped()));
			ui->graphicsView2->setScene(ptr_RGBScene.data());
		}
        else if(type == NIR_1300)
		{
			//make target cross
			Mat imgColor;
			cvtColor(img, imgColor, CV_GRAY2RGB);
			Mat imgCross;
			if(ui->checkBox_drawTargetCross->isChecked()){ imgCross = drawCross(imgColor); }
			else{ imgCross = imgColor; }

			//make a resized copy of the image according to graphic widget size
			cv::resize(imgCross, imgSmall, Size(width_win1,height_win1));
			if(flipImgs){ flip(imgSmall, imgSmall, 1); }

			//show in widget width inverted channels (because Mat is BGR and QImage is RGB)
			QImage qimg(imgSmall.data, imgSmall.cols, imgSmall.rows, imgSmall.step, QImage::Format_RGB888);
			ptr_NIRScene = QSharedPointer<QGraphicsScene>(new QGraphicsScene);//drop last pointer to free memory
			ptr_NIRScene->addPixmap(QPixmap::fromImage(qimg));
			ui->graphicsView1->setScene(ptr_NIRScene.data());
		}
		else if(capturing_kinectRGB && type == Kinect_Depth || !capturing_kinectRGB && type == Kinect_IR)
		{
			//make cross
			Mat imgColor;
			cvtColor(img, imgColor, CV_GRAY2RGB);
			Mat imgCross;
			if(ui->checkBox_drawTargetCross->isChecked()){ imgCross = drawCross(imgColor); }
			else{ imgCross = imgColor; }

			//make a resized copy of the image according to graphic widget size
			if(img.cols != 0) //work-around for not understood bug... when switching back from IR to RGB, depth image is empty. TODO: look into it
			{
				cv::resize(imgCross, imgSmall, Size(width_win3, height_win3));
				if(flipImgs){ flip(imgSmall, imgSmall, 1); }
			}

			//show in widget width inverted channels (because Mat is BGR and QImage is RGB)
			QImage qimg(imgSmall.data, imgSmall.cols, imgSmall.rows, imgSmall.step, QImage::Format_RGB888);
			ptr_depthScene = QSharedPointer<QGraphicsScene>(new QGraphicsScene);//drop last pointer to free memory
			ptr_depthScene->addPixmap(QPixmap::fromImage(qimg));
			ui->graphicsView3->setScene(ptr_depthScene.data());
		}

		//show the other channels in extra windows if user wants that
		else if(ui->checkBox_showAllChannels->isChecked())
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

		emit saveImages(allCapturedImgs, ui->actionRGB->isChecked(), ui->actionNIR_Dark->isChecked(), ui->actionNIR_channels->isChecked(),
						ui->actionKinect_Depth->isChecked(), ui->actionKinect_IR->isChecked(), ui->actionKinect_RGB->isChecked());

		triggerSave = false;
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

Mat RGB_NIR_Depth_Capture::drawCross(Mat &img, int lineStrength)
{
	Mat img2 = img.clone();
	Point p1(img.cols/2, 0);
	Point p2(img.cols/2, img.rows);
	Point p3(0, img.rows/2);
	Point p4(img.cols, img.rows/2);
	cv::line(img2, p1, p2, Scalar(0,255,0), lineStrength);
	cv::line(img2, p3, p4, Scalar(0,255,0), lineStrength);
	return img2;
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


void RGB_NIR_Depth_Capture::startAcquisition()
{
	myImgAcqWorker_Prosilica->setAcquiring(true);
	myImgAcqWorker_Goldeye->setAcquiring(true);
	myImgAcqWorker_Kinect->setAcquiring(true);
	emit startImgAcquisition();
	acquiring = true;
}

void RGB_NIR_Depth_Capture::stopAcquisition()
{
	myImgAcqWorker_Prosilica->setAcquiring(false);
	myImgAcqWorker_Goldeye->setAcquiring(false);
	myImgAcqWorker_Kinect->setAcquiring(false);
	acquiring = false;
}

/***********************************************
** Keyboard events:
************************************************/
void RGB_NIR_Depth_Capture::keyPressEvent(QKeyEvent *event)
{
	//save image when "s" is pressed, ascii[83] = s
	if(event->key() == 83){ triggerSave = true; }

	//start/stop acquiring images when "a" is pressed),, ascii[65] = a
	if(event->key() == 65)
	{
		if(acquiring){ stopAcquisition(); }
		else{ startAcquisition(); }
	}

	//flip images if "f" is pressed
	if(event->key() == 70)
	{
		flipImgs = !flipImgs;
		ui->checkBox_flipImgs->setChecked(flipImgs);
	}

	//open output folder if "o" is pressed
	if(event->key() == 79)
	{
		QDir d;
		QDesktopServices::openUrl(d.absolutePath() + "/out");
	}
}


/***********************************************
** GUI Methods:
************************************************/
void RGB_NIR_Depth_Capture::on_btn_startAcquisition_released()
{
	startAcquisition();
}

void RGB_NIR_Depth_Capture::on_btn_stopAcquisition_released()
{
	stopAcquisition();
}

void RGB_NIR_Depth_Capture::on_btn_saveImgs_released()
{
	triggerSave = true;
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

void RGB_NIR_Depth_Capture::on_checkBox_switchKinectRGB_IR_clicked()
{
	bool capturingRGB = ((KinectWorker*)myImgAcqWorker_Kinect)->isCapturingRGB();
	triggerSwitch_kinectRGB2IR = capturingRGB;
	triggerSwitch_kinectIR2RGB = !capturingRGB;
	((KinectWorker*)myImgAcqWorker_Kinect)->switch_RGB_IR(!capturingRGB);
}

void RGB_NIR_Depth_Capture::on_actionNIR_multi_channel_capture_changed()
{
	emit toggleNIR_MultiChannelCapture(ui->actionNIR_multi_channel_capture->isChecked());
}

void RGB_NIR_Depth_Capture::on_checkBox_simulateRGBcalib_clicked()
{
	simulatingRGBCalib = ui->checkBox_simulateRGBcalib->isChecked();
}

void RGB_NIR_Depth_Capture::on_checkBox_flipImgs_clicked()
{
	flipImgs = ui->checkBox_flipImgs->isChecked();
}


void RGB_NIR_Depth_Capture::on_btn_saveImgs_2_released()
{
	QDir d;
	QDesktopServices::openUrl(d.absolutePath() + "/out");
}
