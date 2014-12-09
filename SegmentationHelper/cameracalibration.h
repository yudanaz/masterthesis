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
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/nonfree/features2d.hpp>

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

    /*!
     * \brief Sets the parameters for the opencv blockmatching algorithm.
     * \param minDisparity: Minimal disparity in pixels
     * \param nrOfDisparities: Disparity range in pixels
     * \param SADWindowSize: Size of Sum-of-Absolute-Differences window
     * \param prefilterSize: Size of normalization pre-filter kernel (reduce lighting differences)
     * \param prefilterCap: Positive limit I_cap for nomralization by min(max(I-I_avg, -I_cap) , I_cap)
     * \param textureThresh: below is considered noise
     * \param uniquenessRatio: Filter out if [match_val - min_match < uniqueRatio * min_match]
     * \param specklewindowSize: handle speckels at object border
     * \param speckleRange: if min and max inside speckle window are inside this range, match is allowed
     */
    void setDisparityParameters(int minDisparity, int nrOfDisparities, int SADWindowSize,
                                int prefilterSize, int prefilterCap,
                                int textureThresh, float uniquenessRatio,
                                int specklewindowSize, int speckleRange);

    /*!
     * \brief Computes a disparity image using the Block Matching algorithm by [Konolige97]
     */
    Mat makeDisparityImage(Mat leftGrayImg, Mat rightGrayImg);
    Mat alignImageByFeatures(Mat imageL, Mat imageRtoBeAligned);

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
    StereoBM sbm;
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
