#ifndef QUICKWEBCAMCAPTURE_H
#define QUICKWEBCAMCAPTURE_H

#include <QMainWindow>
#include<QDebug>
#include<QTime>
#include<QDir>
#include"opencv2/opencv.hpp"

using namespace cv;

namespace Ui {
class QuickWebCamCapture;
}

class QuickWebCamCapture : public QMainWindow
{
	Q_OBJECT

public:
	explicit QuickWebCamCapture(QWidget *parent = 0);
	~QuickWebCamCapture();

private:
	Ui::QuickWebCamCapture *ui;
};

#endif // QUICKWEBCAMCAPTURE_H
