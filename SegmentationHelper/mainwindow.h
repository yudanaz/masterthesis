#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QtWidgets/QFileDialog>
#include <QDir>
#include <QList>
#include <QStringList>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QXmlStreamReader>
#include <QProgressDialog>
#include <QTime>

#include <opencv2/opencv.hpp>
//#include "opencv2/calib3d/calib3d.hpp"
//#include "opencv2/core/core.hpp"
//#include "opencv2/highgui/highgui.hpp"

using namespace cv;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void on_btn_makeLabelImgs_released();

	void on_btn_seed_released();

	void on_pushButton_seedAgain_released();


	void on_btn_slic_released();

	void on_pushButton_slicAgain_released();

	void on_btn_felsenzwalb_released();

	void on_pushButton_felsenzwalbAgain_released();

	void on_btn_stereoVision_released();

	void on_pushButton_stereoAgain_released();

private:
	void makeSeedsSuperpixels(QString fileName);
	void makeSlicSuperpixels(QString fileName);
	void makeFelsenzwalbSuperpixels(QString fileName);
	void makeDisparityImage(QString fileNameL, QString fileNameR);

	Ui::MainWindow *ui;
	vector< vector<int> > colorMap;
	vector< QString > objectMap;
	QString lastDir;
	QString lastSeedsFilename;
	QString lastSlicFilename;
	QString lastFelsenzwalbFilename;
	QString lastStereoFileNameL;
	QString lastStereoFileNameR;
};

#endif // MAINWINDOW_H
