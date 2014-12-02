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
#include <QMessageBox>

#include <opencv2/opencv.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/nonfree/features2d.hpp>
//#include "opencv2/calib3d/calib3d.hpp"
//#include "opencv2/core/core.hpp"
//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/features2d/features2d.hpp"

using namespace cv;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

	/*Just a test to see if the git commit and pull/push really works*/

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

	void on_btn_surf_released();

	void on_pushButton_surfAgain_released();

    void on_btn_calibSingle_released();

    void on_btn_undistSingle_released();

private:
    void makeLabelImages(QStringList fileNames);
	void makeSeedsSuperpixels(QString fileName);
	void makeSlicSuperpixels(QString fileName);
	void makeFelsenzwalbSuperpixels(QString fileName);
	void makeDisparityImage(QString fileNameL, QString fileNameR);
	void makeSurfFeatures(QString fileName);
    void calibrateSingleCamera(QStringList calibImgFiles, int chessboard_width, int chessboard_height);
    void undistortSingleImage(QString fileName);

	Ui::MainWindow *ui;
	vector< vector<int> > colorMap;
	vector< QString > objectMap;
	QString lastDir;
	QString lastSeedsFilename;
	QString lastSlicFilename;
	QString lastFelsenzwalbFilename;
	QString lastStereoFileNameL;
	QString lastStereoFileNameR;
    QString lastFeatureFileName;

    bool cameraCalibrated;
    bool stereoCalibrated;
    Mat camMatrix;
    Mat distCoeff;

};

#endif // MAINWINDOW_H
