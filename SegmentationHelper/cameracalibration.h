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

	/*!
	 * \brief Calibrates a single camera with a list of chessboard pattern images.
	 * \param calibImgs: A list of images with chessboard patterns from varied angles
	 * \param chessboard_width: Inner corners on the rows of the board.
	 * \param chessboard_height: Inner corners on the columns of the board.
	 * \param calibFileName: The path to the file in which the calibration matrices should be stored. If empty, calibration is not saved to disk
	 */
	void calibrateSingleCamera(QList<Mat> calibImgs, int chessboard_width, int chessboard_height, QString calibFileName = "");

	/*!
	 * \brief Calibrates a Goldeye custom multi-spectral camera (ISF-custom-made),
	 * i.e. calibrates the 4 wavebands independently
	 * \param calibImgPacks: A list of lists of images. Each inner list is one multi-spectral image made of 4 wavebands.
	 * \param chessboard_width: Inner corners on the rows of the board.
	 * \param chessboard_height: Inner corners on the columns of the board.
	 * \param calibFileName: The path to the file in which the calibration matrices should be stored. If empty, calibration is not saved to disk
	 */
	void calibrateGoldeyeMultiChannel(QList< QList<Mat> > calibImgPacks, int chessboard_width, int chessboard_height, QString calibFileName = "");

	/*!
	 * \brief Calibrates a pair of stereo cameras with a list of chessboard pattern images.
	 * \param calibImgsLeft: A list of images with chessboard patterns from varied angles for the left camera.
	 * \param calibImgsRight: A list of images with chessboard patterns from varied angles for the right camera.
	 * \param chessboard_width: Inner corners on the rows of the board.
	 * \param chessboard_height: Inner corners on the columns of the board.
	 * \param calibFileName: The path to the file in which the calibration matrices should be stored. If empty, calibration is not saved to disk
	 */
	void calibrateStereoCameras(QList<Mat> calibImgsLeft, QList<Mat> calibImgsRight,
								int chessboard_width, int chessboard_height, QString calibFileName = "");

	/*!
	 * \brief Loads and distignuishes between calibration files for the three camera types:
	 * single, goldeye and stereo cameras.
	 * \param calibFiles: List containing the calibration files (goldeye needs separate single-camera
	 * calibratedcalibration for each waveband).
	 */
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

	void makeAndSaveHomogeneityMatrices(QStringList calibImgTarFiles, QString folderURL);
	bool loadHomogeneityMatrices(QString loadFileURL);
	void applyHomogeneityMatrices(QString multiChImgtarFile);



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
	void makeRectifyMapsForStereo(Size leftImgSize, Size rightImgSize);

	QWidget *parentWidget;
	StereoBM sbm;
	bool cameraCalibrated;
	bool goldeyeCalibrated;
	bool stereoCalibrated;

	Mat camMatrix_single;
	Mat distCoeff_single;

	QVector<Mat> camMatrices_goldeye;
	QVector<Mat> distCoeffs_goldeye;
	int nrOfNIRChannels;

	Mat camMatrix_stereoL;
	Mat camMatrix_stereoR;
	Mat distCoeff_stereoL;
	Mat distCoeff_stereoR;
	Mat rotationMatrix;
	Mat translationVec;
	Mat essentialMat;
	Mat fundamentalMat;
	Mat rectificTransf_L;
	Mat rectificTransf_R;
	Mat projectionMat_L;
	Mat projectionMat_R;
	Mat disparity2DepthMat;
	Mat mapLx;
	Mat mapLy;
	Mat mapRx;
	Mat mapRy;
	bool rectifyMapsCreated;

	Mat homoGen935;
	Mat homoGen1060;
	Mat homoGen1300;
	Mat homoGen1550;
	bool homogeneityCalibrated;
};

#endif // CAMERACALIBRATION_H
