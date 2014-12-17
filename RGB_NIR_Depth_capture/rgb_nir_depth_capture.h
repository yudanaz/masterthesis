#ifndef RGBNIRD_MAINWINDOW_H
#define RGBNIRD_MAINWINDOW_H

#include<QMainWindow>
#include<QDebug>
#include<QList>
#include<QThread>
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
	void imagesReady(RGBDNIR_MAP images);

signals:
	void startImgAcquisition();
	void stopImgAcquisition();

private slots:
	void on_btn_startAcquisition_released();

	void on_btn_saveImgs_released();

	void on_btn_stopAcquisition_released();

private:
	Ui::RGBNIRD_MainWindow *ui;
	QThread workerThread;
	bool triggerSave;
	ImgAcquisitionWorker *myImgAcqWorker;
};

#endif // RGBNIRD_MAINWINDOW_H
