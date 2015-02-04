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

using namespace cv;

class ImagePreprocessor
{
public:
    ImagePreprocessor();

    void calib(QStringList calibImgs_RGB, QStringList calibImgs_NIR, QStringList calibImgs_IR, Size chessboardSize);

    void preproc(Mat& RGB, Mat& NIR, Mat& Depth);

    void saveAll(QString saveURL);
    void loadAll(QString loadURL);

private:
    void make_Intrinsics_and_undistMaps(QList<Mat> calibImgs, Mat& camIntrinsics, Mat& undistMapX, Mat& undistMapY);
    void make_RectifyMaps(QList<Mat> srcImgs, QList<Mat> dstImgs, Mat& rectifMapX, Mat& rectifMapY, Mat &Rot, Mat &Transl);
    void getObjectAndImagePoints(QList<Mat> calibImgs, int width, int height,
                                                    vector<Point3f> obj,
                                                    vector<vector<Point3f> >& objectPoints,
                                                    vector<vector<Point2f> >& imagePoints);

    void getImagePoints(QList<Mat> calibImgs, Size chessboardSize, vector<vector<Point2f> > &imagePoints);

    QList<Mat> undistortImages(QList<Mat> imgs, Mat undistMapX, Mat undistMapY);

    Mat projectDepthTo3DSpace(Mat depth);
    Mat projectFrom3DSpaceToImage(Mat img3D, Mat rot, Mat transl);

    QList<Mat> readImgs2List(QStringList imgNames);


    //size of chessboard used for calibration
    Size chessboardSz;

    //camera instrinsics
    Mat cam_RGB;
    Mat cam_NIR;
    Mat cam_IR;

    //undistortion maps
    Mat undistMapX_RGB;
    Mat undistMapY_RGB;

    Mat undistMapX_NIR;
    Mat undistMapY_NIR;

    Mat undistMapX_IR;
    Mat undistMapY_IR;

    //resize and crop parameters
    double resizeFac_RGB;
    double resizeFac_NIR;
    double resizeFac_IR;

    Rect cropRect_RGB;
    Rect cropRect_NIR;
    Rect cropRect_IR;

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
};

#endif // IMAGEPREPROCESSOR_H
