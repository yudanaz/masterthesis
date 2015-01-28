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
#include<opencv2/opencv.hpp>
#include"imgacquisitionworker.h"
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

private slots:
	void on_btn_startAcquisition_released();

	void on_btn_saveImgs_released();

	void on_btn_stopAcquisition_released();

	void on_checkBox_showAllChannels_clicked();

	void on_pushButton_saveSeries_released();

	void on_btn_saveIR_RGB_pair_released();

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
	QThread workerThread1;
	QThread workerThread2;
	QThread workerThread3;
	ImgAcquisitionWorker *myImgAcqWorker1;
	ImgAcquisitionWorker *myImgAcqWorker2;
	ImgAcquisitionWorker *myImgAcqWorker3;
	QMutex threadLock;

	QSharedPointer<QGraphicsScene> ptr_RGBScene;
	QSharedPointer<QGraphicsScene> ptr_NIRScene;
	QSharedPointer<QGraphicsScene> ptr_depthScene;


	int width_rgb, height_rgb, width_nir, height_nir, width_depth, height_depth;
};

#endif // RGBNIRD_MAINWINDOW_H
