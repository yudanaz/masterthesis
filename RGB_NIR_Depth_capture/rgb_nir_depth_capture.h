#ifndef RGBNIRD_MAINWINDOW_H
#define RGBNIRD_MAINWINDOW_H

#include<QMainWindow>
#include<QDebug>
#include<QList>
#include<opencv2/opencv.hpp>
#include"vimbacammanager.h"


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

private slots:
	void on_btn_acquireImage_released();

private:
	void acquireImages();

	Ui::RGBNIRD_MainWindow *ui;
	VimbaCamManager vimbaCamManager;
};

#endif // RGBNIRD_MAINWINDOW_H
