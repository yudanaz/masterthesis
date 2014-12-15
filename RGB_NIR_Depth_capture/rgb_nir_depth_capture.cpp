#include <QDir>
#include <QTime>
#include "rgb_nir_depth_capture.h"
#include "ui_rgbnird_mainwindow.h"

RGB_NIR_Depth_Capture::RGB_NIR_Depth_Capture(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::RGBNIRD_MainWindow),
	triggerSave(false)
{
	ui->setupUi(this);

	qRegisterMetaType<RGBDNIR_MAP>();
	ImgAcquisitionWorker *imgAcqWorker = new ImgAcquisitionWorker();
	imgAcqWorker->moveToThread(&workerThread);
	connect(&workerThread, SIGNAL(finished()), imgAcqWorker, SLOT(deleteLater()));
	connect(this, SIGNAL(startImgAcquisition()), imgAcqWorker, SLOT(startAcquisition()));
	connect(imgAcqWorker, SIGNAL(imagesReady(RGBDNIR_MAP)), this, SLOT(imagesReady(RGBDNIR_MAP)));
	connect(this, SIGNAL(stopImgAcquisition()), imgAcqWorker, SLOT(stopAcquisition()));
	workerThread.start();

}

RGB_NIR_Depth_Capture::~RGB_NIR_Depth_Capture()
{
	workerThread.quit();
	workerThread.wait();
	destroyAllWindows();
	delete ui;
}

/***********************************************
** PUBLIC SLOTS:
************************************************/
void RGB_NIR_Depth_Capture::imagesReady(RGBDNIR_MAP images)
{
	QMapIterator<RGBDNIR_captureType, Mat> i(images);
	while(i.hasNext())
	{
		i.next();
		Mat img = i.value();
		QString windowName = VimbaCamManager::getRGBDNIR_captureTypeString( (RGBDNIR_captureType)i.key() );
		Mat img8bit(img.rows, img.cols, CV_8UC3);
		if(img.type() != CV_8UC3)
		{
			img.convertTo(img8bit, CV_8UC3);
		}
		else{ img8bit = img; }

		//show everything if user wants that
		if(ui->checkBox_showAllChannels->isChecked())
		{
			namedWindow(windowName.toStdString(), CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO | CV_GUI_EXPANDED);
			imshow(windowName.toStdString(), img);
		}
	}

	//save current image list if save button is clicked
	if(triggerSave)
	{
		i.toFront();
		QTime time;
		while(i.hasNext())
		{
			i.next();
			QString nm = QDir::currentPath() + "/" + time.currentTime().toString() + "_" +
						 VimbaCamManager::getRGBDNIR_captureTypeString( (RGBDNIR_captureType)i.key() ) + ".png";
			imwrite(nm.toStdString().c_str(), i.value());
		}
		triggerSave = false;
	}
}


/***********************************************
** GUI Methods:
************************************************/
void RGB_NIR_Depth_Capture::on_btn_startAcquisition_released()
{
	emit startImgAcquisition();
}

void RGB_NIR_Depth_Capture::on_btn_saveImgs_released()
{
	triggerSave = true;
}
