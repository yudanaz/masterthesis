#include "imagepreprocessor.h"
#include "io.h"

ImagePreprocessor::ImagePreprocessor()
{
}


void ImagePreprocessor::calib(QStringList calibImgs_RGB,
                              QStringList calibImgs_NIR,
                              QStringList calibImgs_IR, Size chessboardSize)
{
    chessboardSz = chessboardSize;

    //read calibration images from disk
    QList<Mat> imgsRGB = readImgs2List(calibImgs_RGB);
//    QList<Mat> imgsNIR = readImgs2List(calibImgs_NIR);
    QList<Mat> imgsIR = readImgs2List(calibImgs_IR);

    //compute intrinsics and undistortion map
    make_Intrinsics_and_undistMaps(imgsRGB, cam_RGB, undistMapX_RGB, undistMapY_RGB);
//    make_Intrinsics_and_undistMaps(imgsNIR, cam_NIR, undistMapX_NIR, undistMapY_NIR);
    make_Intrinsics_and_undistMaps(imgsIR, cam_IR, undistMapX_IR, undistMapY_IR);

    //undistort the calibration images
    QList<Mat> imgsRGB_undist = undistortImages(imgsRGB, undistMapX_RGB, undistMapY_RGB);
//    QList<Mat> imgsNIR_undist = undistortImages(imgsNIR, undistMapX_NIR, undistMapY_NIR);
    QList<Mat> imgsIR_undist = undistortImages(imgsIR, undistMapX_IR, undistMapY_IR);

    //TODO: make resize and crop parameters

    //TODO: resize calibration images

    //make rectify maps
    //[!] here temporaray intrinsics and dist.coeff. are computed AGAIN on undist.&resized/cropped calib images.
    //Tests have shown that the result does not suffer from this
    make_RectifyMaps(imgsIR_undist, imgsRGB_undist, rectifMapX_IR, rectifMapY_IR);
//    make_RectifyMaps(imgsNIR_undist, imgsRGB_undist, rectifMapX_NIR, rectifMapY_NIR);
}

void ImagePreprocessor::make_Intrinsics_and_undistMaps(QList<Mat> calibImgs, Mat &camIntrinsics, Mat &undistMapX, Mat &undistMapY)
{
    Size imgSize = calibImgs.first().size();
    int nrOfCorners = chessboardSz.width * chessboardSz.height;  //total number of inner corners

    //objectPoints should contain physical location of each corners but
    //since we don’t know that so we assign constant positions to all the corners
    vector<Point3f> obj;
    for (int i = 0; i < nrOfCorners; i++){ obj.push_back(Point3f(i / chessboardSz.width, i % chessboardSz.width, 0.0f)); }

    //get the object and image points
    vector<vector<Point3f> > objectPoints;
    vector<vector<Point2f> > imagePoints;
    getObjectAndImagePoints(calibImgs, chessboardSz.width, chessboardSz.height, obj, objectPoints, imagePoints);


    //get intrinsics
    Mat distCoeff;
    vector<Mat> rvecs, tvecs; //dummy
    calibrateCamera(objectPoints, imagePoints, imgSize, camIntrinsics, distCoeff, rvecs, tvecs);

    //get undistortion maps
    initUndistortRectifyMap(camIntrinsics, distCoeff, Mat(), camIntrinsics, imgSize, CV_32FC1, undistMapX, undistMapY);
}


void ImagePreprocessor::make_RectifyMaps(QList<Mat> srcImgs, QList<Mat> dstImgs, Mat &rectifMapX, Mat &rectifMapY)
{
    Size imgSize = srcImgs.first().size();
    int nrOfCorners = chessboardSz.width * chessboardSz.height;  //total number of inner corners
    vector<Point3f> obj;
    for (int i = 0; i < nrOfCorners; i++){ obj.push_back(Point3f(i / chessboardSz.width, i % chessboardSz.width, 0.0f)); }

    //get the object and image points
    vector<vector<Point3f> > objPoints_src;
    vector<vector<Point3f> > objPoints_dst;
    vector<vector<Point2f> > imgPoints_src;
    vector<vector<Point2f> > imgPoints_dst;

    getObjectAndImagePoints(srcImgs, chessboardSz.width, chessboardSz.height, obj, objPoints_src, imgPoints_src);
    getObjectAndImagePoints(dstImgs, chessboardSz.width, chessboardSz.height, obj, objPoints_dst, imgPoints_dst);

    //get intrinsics
    Mat distCoeff_src, distCoeff_dst;
    Mat cam_src, cam_dst;
    vector<Mat> rvecs_src, tvecs_src, rvecs_dst, tvecs_dst; //dummy
    calibrateCamera(objPoints_src, imgPoints_src, imgSize, cam_src, distCoeff_src, rvecs_src, tvecs_src);
    calibrateCamera(objPoints_dst, imgPoints_dst, imgSize, cam_dst, distCoeff_dst, rvecs_dst, tvecs_dst);

    //calibrate as stereo cameras
    Mat Rot, Transl, E, F, Rect_src, Rect_dst, Proj_src, Proj_dst, Q;
    stereoCalibrate(objPoints_src,
                    imgPoints_src, imgPoints_dst,
                    cam_src, distCoeff_src,
                    cam_dst, distCoeff_dst,
                    imgSize, Rot, Transl, E, F);

    stereoRectify(cam_src, distCoeff_src,
                  cam_dst, distCoeff_dst, imgSize, Rot, Transl,
                  Rect_src, Rect_dst, Proj_src, Proj_dst, Q, CALIB_ZERO_DISPARITY);

    //make the rectify maps (rgb is computed every time, because all other images are mapped to RGB)
    initUndistortRectifyMap(cam_src, distCoeff_src, Rect_src, Proj_src, imgSize, CV_32FC1, rectifMapX, rectifMapY);
    initUndistortRectifyMap(cam_dst, distCoeff_dst, Rect_dst, Proj_dst, imgSize, CV_32FC1, rectifMapX_RGB, rectifMapY_RGB);
}


QList<Mat> ImagePreprocessor::undistortImages(QList<Mat> imgs, Mat undistMapX, Mat undistMapY)
{
    QList<Mat> imgs_undist;
    foreach (Mat img, imgs)
    {
        Mat undist;
        remap(img, undist, undistMapX, undistMapY, INTER_LINEAR, BORDER_CONSTANT, Scalar(0));
        imgs_undist.append(undist);
    }
    return imgs_undist;
}



void ImagePreprocessor::getObjectAndImagePoints(QList<Mat> calibImgs, int width, int height,
                                                vector<Point3f> obj,
                                                vector<vector<Point3f> >& objectPoints,
                                                vector<vector<Point2f> >& imagePoints)
{
    Size chessboard_sz = Size(width, height);

    //get chessboard corners for all images
    getImagePoints(calibImgs, chessboard_sz, imagePoints);

    //put as many of the dummy object points in the list as there are image points
    for (int i = 0; i < imagePoints.size(); ++i)
    {
        objectPoints.push_back(obj);
    }
}

void ImagePreprocessor::getImagePoints(QList<Mat> calibImgs, Size chessboardSize, vector<vector<Point2f> > &imagePoints)
{
    //get chessboard corners for all images
    bool success = false;
    foreach(Mat img, calibImgs)
    {
        vector<Point2f> corners;
        success = findChessboardCorners(img, chessboardSize, corners,
                                        CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);

        if(success){ imagePoints.push_back(corners); }
    }
}


QList<Mat> ImagePreprocessor::readImgs2List(QStringList imgNames)
{
    QList<Mat> imgs;
    foreach (QString s, imgNames)
    {
        imgs.append(imread(s.toStdString(), IMREAD_GRAYSCALE));
    }
    return imgs;
}

