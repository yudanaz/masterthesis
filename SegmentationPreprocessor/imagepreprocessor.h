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

#include <opencv2/opencv.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/nonfree/features2d.hpp>

#include "crossbilateralfilterwrapper.h"

using namespace cv;
using namespace std;

class ImagePreprocessor
{
public:
    ImagePreprocessor();

    void calibCams(QStringList calibImgs_RGB, QStringList calibImgs_NIR, QStringList calibImgs_IR, Size chessboardSize);
    void calibRig(QStringList calibImgs_RGB, QStringList calibImgs_NIR, QStringList calibImgs_IR, Size chessboardSize);

    void preproc(Mat RGB, Mat NIR, Mat depth, Mat &RGB_out, Mat &NIR_out, Mat &depth_stereo_out, Mat &depth_remapped_out);

    void saveAll(QString saveURL);
    void loadAll(QString loadURL);

    //get status
    bool cams_are_calibrated(){ return cams_are_calibrated_; }
    bool rig_is_calibrated(){ return rig_is_calibrated_; }

private:
    void make_Intrinsics_and_undistCoeffs(QList<Mat> calibImgs, Mat& camIntrinsics, Mat& distCoeff);
    void make_RectifyMaps(QList<Mat> srcImgs, QList<Mat> dstImgs, Mat distCoeff_src, Mat distCoeff_dst, Mat cam_src, Mat cam_dst,
                          Mat& out_rectifMapX, Mat& out_rectifMapY, Mat &out_Rot, Mat &out_Transl);

    void fitRGBimgs2NIRimgs(QList<Mat> origNirs, QList<Mat> origRGBs);
    Mat resizeAndCropRGBImg(Mat rgbImg);

    vector<Point3f> projectDepthTo3DSpace(Mat depth);

    Mat projectFrom3DSpaceToImage(vector<Point3f> points3D, Mat rot, Mat transl, Mat cam_Matrix, Size outImgSz);

    //    QList<Mat> undistortImages(QList<Mat> imgs, Mat undistMapX, Mat undistMapY);

    void getObjectAndImagePoints(QList<Mat> calibImgs, int width, int height,
                                                    vector<Point3f> obj,
                                                    vector<vector<Point3f> >& objectPoints,
                                                    vector<vector<Point2f> >& imagePoints);

    void getImagePoints(QList<Mat> calibImgs, Size chessboardSize, vector<vector<Point2f> > &imagePoints);


    QList<Mat> readImgs2List(QStringList imgNames);

    //cross bilateral filter
    CrossBilateralFilterWRAPPER crossbilatFilter;

    //size of chessboard used for calibration
    Size chessboardSz;

    //camera instrinsics
    Mat cam_RGB;
    Mat cam_NIR;
    Mat cam_IR;

    //undistortion maps
    Mat distCoeff_RGB;
    Mat distCoeff_NIR;
    Mat distCoeff_IR;

    //resize and crop parameters
    double resizeFac_RGB;
//    double resizeFac_NIR;
//    double resizeFac_IR;

    Rect cropRect_RGB;
//    Rect cropRect_NIR;
//    Rect cropRect_IR;

    //rectification maps
    Mat rectifMapX_RGB;
    Mat rectifMapY_RGB;

    Mat rectifMapX_NIR;
    Mat rectifMapY_NIR;

    Mat rectifMapX_IR;
    Mat rectifMapY_IR;

    //translation and rotation
    Mat rotation_IR2RGB;
    Mat rotation_NIR2RGB;
    Mat transl_IR2RGB;
    Mat transl_NIR2RGB;

    //flags:
    bool cams_are_calibrated_;
    bool rig_is_calibrated_;
};

#endif // IMAGEPREPROCESSOR_H
