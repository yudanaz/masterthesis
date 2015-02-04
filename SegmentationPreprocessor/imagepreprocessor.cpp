#include <QProgressDialog>
#include "imagepreprocessor.h"
#include "io.h"


//#################################################################################################
// PUBLIC:
//#################################################################################################

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

//    qDebug() << (int)CV_32FC1;
//    qDebug() << (int)CV_64FC1;
//    qDebug() << cam_IR.type();

    //undistort the calibration images
    QList<Mat> imgsRGB_undist = undistortImages(imgsRGB, undistMapX_RGB, undistMapY_RGB);
//    QList<Mat> imgsNIR_undist = undistortImages(imgsNIR, undistMapX_NIR, undistMapY_NIR);
    QList<Mat> imgsIR_undist = undistortImages(imgsIR, undistMapX_IR, undistMapY_IR);

    //TODO: make resize and crop parameters

    //TODO: resize calibration images

    //make rectify maps
    //[!] here temporaray intrinsics and dist.coeff. are computed AGAIN on undist.&resized/cropped calib images.
    //Tests have shown that the result does not suffer from this
    make_RectifyMaps(imgsIR_undist, imgsRGB_undist, rectifMapX_IR, rectifMapY_IR, rotation_IR2RGB, transl_IR2RGB);
//    make_RectifyMaps(imgsNIR_undist, imgsRGB_undist, rectifMapX_NIR, rectifMapY_NIR, rotation_NIR2RGB, transl_NIR2RGB);
}

void ImagePreprocessor::preproc(Mat& RGB, Mat& NIR, Mat& Depth)
{
    //undistort
    Mat RGB_undist, NIR_undist, Depth_undist;
    remap(RGB, RGB_undist, undistMapX_RGB, undistMapY_RGB, INTER_LINEAR, BORDER_CONSTANT, Scalar(0));
//    remap(NIR, NIR_undist, undistMapX_NIR, undistMapY_NIR, INTER_LINEAR, BORDER_CONSTANT, Scalar(0));
    remap(Depth, Depth_undist, undistMapX_IR, undistMapY_IR, INTER_LINEAR, BORDER_CONSTANT, Scalar(0));

    //TODO: resize / crop

    //map Kinect depth to RGB


    //rectify

}

void ImagePreprocessor::saveAll(QString saveURL)
{
    FileStorage fs(saveURL.toStdString(), FileStorage::WRITE);
    fs << "cam_RGB" << cam_RGB;
    fs << "cam_NIR" << cam_NIR;
    fs << "cam_IR" << cam_IR;

    fs << "undistMapX_RGB" << undistMapX_RGB;
    fs << "undistMapY_RGB" << undistMapY_RGB;
    fs << "undistMapX_NIR" << undistMapX_NIR;
    fs << "undistMapY_NIR" << undistMapY_NIR;
    fs << "undistMapX_IR" << undistMapX_IR;
    fs << "undistMapY_IR" << undistMapY_IR;

    fs << "resizeFac_RGB" << resizeFac_RGB;
    fs << "resizeFac_NIR" << resizeFac_NIR;
    fs << "resizeFac_IR" << resizeFac_IR;

    fs << "cropRect_RGB" << cropRect_RGB;
    fs << "cropRect_NIR" << cropRect_NIR;
    fs << "cropRect_IR" << cropRect_IR;

    fs << "rectifMapX_RGB" << rectifMapX_RGB;
    fs << "rectifMapY_RGB" << rectifMapY_RGB;
    fs << "rectifMapX_NIR" << rectifMapX_NIR;
    fs << "rectifMapY_NIR" << rectifMapY_NIR;
    fs << "rectifMapX_IR" << rectifMapX_IR;
    fs << "rectifMapY_IR" << rectifMapY_IR;

    fs << "rotation_NIR2RGB" << rotation_NIR2RGB;
    fs << "rotation_IR2RGB" << rotation_IR2RGB;
    fs << "transl_NIR2RGB" << transl_NIR2RGB;
    fs << "transl_IR2RGB" << transl_IR2RGB;

    fs.release();
}

void ImagePreprocessor::loadAll(QString loadURL)
{
    FileStorage fs(loadURL.toStdString(), FileStorage::READ);
    fs["cam_RGB"] >> cam_RGB;
    fs["cam_NIR"] >> cam_NIR;
    fs["cam_IR"] >> cam_IR;

    fs["undistMapX_RGB"] >> undistMapX_RGB;
    fs["undistMapY_RGB"] >> undistMapY_RGB;
    fs["undistMapX_NIR"] >> undistMapX_NIR;
    fs["undistMapY_NIR"] >> undistMapY_NIR;
    fs["undistMapX_IR"] >> undistMapX_IR;
    fs["undistMapY_IR"] >> undistMapY_IR;

    fs["resizeFac_RGB"] >> resizeFac_RGB;
    fs["resizeFac_NIR"] >> resizeFac_NIR;
    fs["resizeFac_IR"] >> resizeFac_IR;

    fs["cropRect_RGB"] >> cropRect_RGB;
    fs["cropRect_NIR"] >> cropRect_NIR;
    fs["cropRect_IR"] >> cropRect_IR;

    fs["rectifMapX_RGB"] >> rectifMapX_RGB;
    fs["rectifMapY_RGB"] >> rectifMapY_RGB;
    fs["rectifMapX_NIR"] >> rectifMapX_NIR;
    fs["rectifMapY_NIR"] >> rectifMapY_NIR;
    fs["rectifMapX_IR"] >> rectifMapX_IR;
    fs["rectifMapY_IR"] >> rectifMapY_IR;

    fs["rotation_NIR2RGB"] >> rotation_NIR2RGB;
    fs["rotation_IR2RGB"] >> rotation_IR2RGB;
    fs["transl_NIR2RGB"] >> transl_NIR2RGB;
    fs["transl_IR2RGB"] >> transl_IR2RGB;

    fs.release();
}


//#################################################################################################
// PRIVATE:
//#################################################################################################

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


void ImagePreprocessor::make_RectifyMaps(QList<Mat> srcImgs, QList<Mat> dstImgs, Mat &rectifMapX, Mat &rectifMapY, Mat &Rot, Mat &Transl)
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
    Mat /*Rot, Transl,*/ E, F, Rect_src, Rect_dst, Proj_src, Proj_dst, Q;
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

Mat ImagePreprocessor::projectDepthTo3DSpace(Mat depth)
{
    Mat depth3D(depth.rows, depth.cols, CV_32FC3);

    float fx = cam_IR.at<double>(0,0);
    float cx = cam_IR.at<double>(0,2);
    float fy = cam_IR.at<double>(1,1);
    float cy = cam_IR.at<double>(1,2);

    for (int y = 0; y < depth.rows; ++y)
    {
        for (int x = 0; x < depth.cols; ++x)
        {
            Vec3f coord3D;
            ushort d = depth.at<ushort>(y,x);
            coord3D[0] = (x - cx) * d / fx; //depth image is 16UC1 = ushort
            coord3D[1] = (y - cy) * d / fy;
            coord3D[2] = d;
            depth3D.at<Vec3f>(y,x) = coord3D;
        }
    }
    return depth3D;
}

Mat ImagePreprocessor::projectFrom3DSpaceToImage(Mat img3D, Mat rot, Mat transl)
{
    for (int y = 0; y < img3D.rows; ++y)
    {
        for (int x = 0; x < img3D.cols; ++x)
        {
            //rotate and translate point
            Vec3f p = img3D.at<Vec3f>(y,x);

            //reproject from 3d to 2d image plain
        }
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





