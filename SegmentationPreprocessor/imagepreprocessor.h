#ifndef IMAGEPREPROCESSOR_H
#define IMAGEPREPROCESSOR_H

#include <vector>

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
#include <QPointer>

#include <opencv2/opencv.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/nonfree/features2d.hpp>

#include "crossbilateralfilterwrapper.h"
#include "crossspectralstereomatcher.h"

using namespace cv;
using namespace std;

enum CrossSpectralStereoType { crossSpectrSt_HOG, crossSpectrSt_LocNorm };

class ImagePreprocessor
{
public:
	ImagePreprocessor(QWidget* parent);

	void calibCams(QStringList calibImgs_RGB, QStringList calibImgs_NIR, QStringList calibImgs_IR, Size chessboardSize);
	void calibRig(QStringList calibImgs_RGB, QStringList calibImgs_NIR_rgb, QStringList calibImgs_IR, QStringList calibImgsNIR_ir, Size chessboardSize);

	void preproc(Mat RGB, Mat NIR, Mat depth_kinect, Mat &RGB_out, Mat &NIR_out, Mat &depth_stereo_out, Mat &depth_remapped_out);
	void setParameters(std::vector<float> params);

	void saveAll(QString saveURL);
	void loadAll(QString loadURL);

	/**************************************************************************
	 * GETTER & SETTER:
	 *************************************************************************/
	//get status
	bool cams_are_calibrated(){ return cams_are_calibrated_; }
	bool rig_is_calibrated(){ return rig_is_calibrated_; }

	void OutputImageSize(int w, int h);
	void set_CSstereoType(CrossSpectralStereoType type);



private:
	void make_Intrinsics_and_undistCoeffs(QList<Mat> calibImgs, Mat& camIntrinsics, Mat& distCoeff, QString imgType);
	void make_RectifyMaps(QList<Mat> srcImgs, QList<Mat> dstImgs, Mat distCoeff_src, Mat distCoeff_dst, Mat cam_src, Mat cam_dst,
						  Mat& out_rectifMapX_src, Mat& out_rectifMapY_src, Mat& out_rectifMapX_dst, Mat& out_rectifMapY_dst,
						  Mat &out_Rot, Mat &out_Transl, Mat &Proj_dst, QString imgTypeSrc, QString imgTypeDst);

	bool fitRGBimgs2NIRimgs(QList<Mat> origNirs, QList<Mat> origRGBs);
	Mat resizeAndCropRGBImg(Mat rgbImg);
	Mat registerImageByHorizontalShift(Mat img, vector<KeyPoint> k1, vector<KeyPoint> k2);

	Mat mapKinectDepth2NIR(Mat depth_kinect, Mat &NIR_img);
	Mat fixHolesInDepthMap(Mat depth);
	vector<Point3f> projectKinectDepthTo3DSpace(Mat depth);
	Mat projectFrom3DSpaceToImage(vector<Point3f> points3D, Mat rot, Mat transl, Mat cam_Matrix, Mat distCoeff, Size outImgSz, double shiftX, double shiftY);

	void makeCrossSpectralStereo(Mat imgNIR_L, Mat imgRGB_R, Mat &out_disp);


	/**************************************************************************
	 * HELPER:
	 *************************************************************************/
	void getObjectAndImagePoints(QList<Mat> calibImgs, int width, int height,
													vector<Point3f> obj,
													vector<vector<Point3f> >& objectPoints,
													vector<vector<Point2f> >& imagePoints, QString imgType,
													QList<int> &blacklist, bool useBlacklist = false);

	void getImagePoints(QList<Mat> calibImgs, Size chessboardSize, vector<vector<Point2f> > &imagePoints, QString imgType,
						bool useBlacklist, QList<int> &blacklist);


	QList<Mat> readImgs2List(QStringList imgNames);

	void makeMsg(QString title, QString msg);
	void makeImgRelatedMsg(Mat img, QString title, QString msg);


	/**************************************************************************
	 * VARIABLES:
	 *************************************************************************/
	QWidget *parent; //ref to parent widget
	CrossBilateralFilterWRAPPER crossbilatFilter;
	QPointer<CrossSpectralStereoMatcher> CSstereoMatcher;
	Size chessboardSz; //size of chessboard used for calibration
	Size outputImgSz; //desired size of output image (e.g. for CNN sth. like 320x240 would be nice)

	//camera instrinsics
	Mat cam_RGB;
	Mat cam_RGB_resized;
	Mat cam_NIR;
	Mat cam_IR;

	//undistortion maps
	Mat distCoeff_RGB;
	Mat distCoeff_RGB_resized;
	Mat distCoeff_NIR;
	Mat distCoeff_IR;

	//resize and crop parameters
	double resizeFac_RGB;
	Rect cropRect_RGB;

	//rectification maps
	Mat rectifyMapX_RGB;
	Mat rectifyMapY_RGB;

	Mat rectifyMapX_NIR;
	Mat rectifyMapY_NIR;

	Mat rectifyMapX_IR;
	Mat rectifyMapY_IR;

	//translation and rotation to NIR camera (our "main" camera position)
	Mat rotation_IR2NIR;
	Mat proj_IR2NIR;
	Mat rotation_RGB2NIR;
	Mat transl_IR2NIR;
	Mat transl_RGB2NIR;

	//flags:
	bool cams_are_calibrated_;
	bool rig_is_calibrated_;
	bool output_image_size_set_;
	CrossSpectralStereoType stereo_Type_;
};

#endif // IMAGEPREPROCESSOR_H
