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

	void on_btn_saveImgs_released();

	void on_btn_stopAcquisition_released();

	void on_checkBox_showAllChannels_clicked();

	void on_pushButton_saveSeries_released();

	void on_btn_saveIR_RGB_pair_released();

	void on_pushButton_switchRGB_IR_released();

	void on_actionNIR_multi_channel_capture_changed();

protected:
	void keyPressEvent(QKeyEvent *event);

private:
	QString getUniquePrefixFromDateAndTime();
	void captureSeries();
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
	bool triggerSaveIR_RGB_pair;
	bool triggerSwitchRGB2IR;
	bool triggerSwitchIR2RGB;
	bool capturingRGB;
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
};

#endif // RGBNIRD_MAINWINDOW_H
