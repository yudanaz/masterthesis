#ifndef CAMERACALIBRATION_H
#define CAMERACALIBRATION_H

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
#include <QInputDialog>
#include <QProcess>

#include <opencv2/opencv.hpp>

#include"inout.h"

using namespace cv;

class CameraCalibration
{
public:
	CameraCalibration(QWidget *parent);
	void calibrateSingleCamera(QStringList calibImgFiles, int chessboard_width, int chessboard_height);
	void calibrateGoldeyeMultiChannel(QStringList calibImgTarFiles, int chessboard_width, int chessboard_height);
	bool liveCalibrateSingleCamera(int nrOfChessboards, int chessboard_width, int chessboard_height);

	void undistortSingleImage(QString fileName);
	void undistortGoldeyeMultiChImg(QString tarFileName);

	bool isCalibrated(){ return cameraCalibrated; }

private:
	void calibrateCamFromImages(QList<Mat> calibImgs, int chessboard_width, int chessboard_height, bool isGrayScale = false);
	void saveCalibrationMatrices(QString calibFileName);

	QWidget *parentWidget;
	bool cameraCalibrated;
	bool stereoCalibrated;
	Mat camMatrix;
	Mat distCoeff;
};

#endif // CAMERACALIBRATION_H
