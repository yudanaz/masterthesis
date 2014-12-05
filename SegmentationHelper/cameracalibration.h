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
#include <QVector>

#include <opencv2/opencv.hpp>

#include"inout.h"

using namespace cv;

class CameraCalibration
{
public:
	CameraCalibration(QWidget *parent);
	void calibrateSingleCamera(QStringList calibImgFiles, int chessboard_width, int chessboard_height);
    void calibrateGoldeyeMultiChannel(QStringList calibImgTarFiles, int chessboard_width, int chessboard_height);
    void calibrateStereoCameras(QStringList calibImgsLeft, QStringList calibImgsRight,
                                int chessboard_width, int chessboard_height);

    void loadCalibrationFile(QStringList calibFiles);
    void undistortSingleImage(QString fileName);
	void undistortGoldeyeMultiChImg(QStringList tarFileNames);
    void undistortAndRemapStereoImages(Mat leftImage, Mat rightImage, Mat &leftImgOut, Mat &rightImgOut);
    Mat makeDisparityImage(Mat leftGrayImg, Mat rightGrayImg, int nrOfDisparities, int blockSize);

    bool isCalibrated_cam(){ return cameraCalibrated; }
    bool isCalibrated_goldeye(){ return goldeyeCalibrated; }
    bool isCalibrated_stereo(){ return stereoCalibrated; }

private:
    void calibrateCamFromImages(QList<Mat> calibImgs, int channelIndex,
                                int chessboard_width, int chessboard_height, bool isGrayScale = false);
    void getObjectAndImagePoints(QList<Mat> calibImgs, int width, int height, vector<Point3f> obj,
                                 vector<vector<Point3f> >& objectPoints, vector<vector<Point2f> >& imagePoints,
                                 bool isGrayScale = false);
    void saveCalibrationFile(QString calibFileName, int channelIndex);
    void saveStereoCalibrationFile(QString calibFileName);

	QWidget *parentWidget;
	bool cameraCalibrated;
    bool goldeyeCalibrated;
	bool stereoCalibrated;
    QVector<Mat> cameraMatrices;
    QVector<Mat> distCoefficients;
    Mat rotationMatrix;
    Mat translationVec;
    Mat essentialMat;
    Mat fundamentalMat;
    Mat rectificTransf_L;
    Mat rectificTransf_R;
    Mat projectionMat_L;
    Mat projectionMat_R;
    Mat disparity2DepthMat;
    int nrOfNIRChannels;
};

#endif // CAMERACALIBRATION_H
