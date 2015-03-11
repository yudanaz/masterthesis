#ifndef RGBNIRD_MAINWINDOW_H
#define RGBNIRD_MAINWINDOW_H

#include<QMainWindow>
#include<QDebug>
#include<QList>
#include<QThread>
#include<QGraphicsScene>
#include<QMutex>
#include<QTime>
#include<QSound>
#include<QKeyEvent>
#include<opencv2/opencv.hpp>
#include"imgacquisitionworker.h"
#include "prosilicaworker.h"
#include "goldeyeworker.h"
#include "kinectworker.h"
#include"saveimgsworker.h"
#include"vimbacammanager.h"

Q_DECLARE_METATYPE(RGBDNIR_MAP)

using namespace AVT::VmbAPI;
using namespace cv;


namespace Ui {
class RGBNIRD_MainWindow;
}

class RGB_NIR_Depth_Capture : public QMainWindow
{
	Q_OBJECT

public:
	explicit RGB_NIR_Depth_Capture(QWidget *parent = 0);
	~RGB_NIR_Depth_Capture();

public slots:
	void imagesReady(RGBDNIR_MAP capturedImgs);

signals:
	void startImgAcquisition();
	void toggleNIR_MultiChannelCapture(bool captureMulti);
	void saveImages(RGBDNIR_MAP imgs, bool RGBImg, bool NIR_DarkImg, bool NIR_channels, bool kinect_depth, bool kinect_ir, bool kinect_rgb);

private slots:
	void on_btn_startAcquisition_released();

	void on_btn_stopAcquisition_released();

	void on_btn_saveImgs_released();

	void on_checkBox_showAllChannels_clicked();

	void on_pushButton_saveSeries_released();

	void on_actionNIR_multi_channel_capture_changed();

	void on_checkBox_simulateRGBcalib_clicked();

	void on_checkBox_flipImgs_clicked();

	void on_checkBox_switchKinectRGB_IR_clicked();

	void on_btn_saveImgs_2_released();

protected:
	void keyPressEvent(QKeyEvent *event);

private:
	void startAcquisition();
	void stopAcquisition();
	bool acquiring;
	QString getUniquePrefixFromDateAndTime();
	void captureSeries();
	Mat drawCross(Mat& img);

	bool capturingSeries;
	bool countingDown;
	QTime myTimer;
	QTime myBeepTimer;
	int countdownSeconds;
	int countdownTime;
	int seriesCnt;
	int seriesMax;
	int seriesInterval;
	QSound sound_click;
	QSound sound_beep;
	QSound sound_beep2;

	RGBDNIR_MAP allCapturedImgs;

	Ui::RGBNIRD_MainWindow *ui;
	bool triggerSave;
	bool triggerSwitch_kinectRGB2IR;
	bool triggerSwitch_kinectIR2RGB;
	bool capturing_kinectRGB;
	QThread workerThread_Prosilica;
	QThread workerThread_Goldeye;
	QThread workerThread_Kinect;
	QThread workerThread_SaveImgs;
	ProsilicaWorker *myImgAcqWorker_Prosilica;
	GoldeyeWorker *myImgAcqWorker_Goldeye;
	KinectWorker *myImgAcqWorker_Kinect;
	SaveImgsWorker *mySaveImgsWorker;

	QMutex threadLock;

	QSharedPointer<QGraphicsScene> ptr_RGBScene;
	QSharedPointer<QGraphicsScene> ptr_NIRScene;
	QSharedPointer<QGraphicsScene> ptr_depthScene;


	int width_rgb, height_rgb, width_nir, height_nir, width_depth, height_depth;

	//params to show the RGB image the way it's gonna be after pre-processing
	Mat cam_RGB;
	Mat distCoeff_RGB;
	double resizeFac_RGB;
	Rect cropRect_RGB;
	Rect drawRect_RGB;
	bool simulatingRGBCalib;

	bool flipImgs;
};

#endif // RGBNIRD_MAINWINDOW_H
