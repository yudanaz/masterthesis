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
    ImagePreprocessor(QWidget* parent);

    void calibCams(QStringList calibImgs_RGB, QStringList calibImgs_NIR, QStringList calibImgs_IR, Size chessboardSize);
    void calibRig(QStringList calibImgs_RGB, QStringList calibImgs_NIR, QStringList calibImgs_IR, Size chessboardSize);

    void preproc(Mat RGB, Mat NIR, Mat depth, Mat &RGB_out, Mat &NIR_out, Mat &depth_stereo_out, Mat &depth_remapped_out);

    void saveAll(QString saveURL);
    void loadAll(QString loadURL);

    //get status
    bool cams_are_calibrated(){ return cams_are_calibrated_; }
    bool rig_is_calibrated(){ return rig_is_calibrated_; }


    /**************************************************************************
     * GETTER & SETTER:
     *************************************************************************/
    void OutputImageSize(int w, int h){ outputImgSz.width = w; outputImgSz.height = h; output_image_size_set_ = true;}



private:
    void make_Intrinsics_and_undistCoeffs(QList<Mat> calibImgs, Mat& camIntrinsics, Mat& distCoeff, QString imgType);
    void make_RectifyMaps(QList<Mat> srcImgs, QList<Mat> dstImgs, Mat distCoeff_src, Mat distCoeff_dst, Mat cam_src, Mat cam_dst,
                          Mat& out_rectifMapX_src, Mat& out_rectifMapY_src, Mat& out_rectifMapX_dst, Mat& out_rectifMapY_dst,
                          Mat &out_Rot, Mat &out_Transl, QString imgTypeSrc, QString imgTypeDst);

    bool fitRGBimgs2NIRimgs(QList<Mat> origNirs, QList<Mat> origRGBs);
    Mat resizeAndCropRGBImg(Mat rgbImg);

    vector<Point3f> projectDepthTo3DSpace(Mat depth);
    Mat projectFrom3DSpaceToImage(vector<Point3f> points3D, Mat rot, Mat transl, Mat cam_Matrix, Size outImgSz);

    void makeCrossSpectralStereo(Mat imgRGB_L, Mat imgNIR_R, Mat out_disp);

    /*!
     * \brief Pre-processes an image to zero mean and unit variance
     * (standard deviation) for local neighborhoods in order to make
     * it more machine-learning-friendly.
     * inspired by
     * http://bigwww.epfl.ch/demo/jlocalnormalization/
     */
    Mat NormalizeLocally(Mat img, int meanKernel, int stdDevKernel, bool outputAs8bit = true);



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
//    double resizeFac_NIR;
//    double resizeFac_IR;

    Rect cropRect_RGB;
//    Rect cropRect_NIR;
//    Rect cropRect_IR;

    //rectification maps
    Mat rectifyMapX_RGB;
    Mat rectifyMapY_RGB;

    Mat rectifyMapX_NIR;
    Mat rectifyMapY_NIR;

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
    bool output_image_size_set_;
};

#endif // IMAGEPREPROCESSOR_H
