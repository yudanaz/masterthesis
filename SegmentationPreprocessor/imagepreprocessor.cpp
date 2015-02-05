#include <QProgressDialog>
#include "imagepreprocessor.h"
#include "io.h"


//#################################################################################################
// PUBLIC:
//#################################################################################################

ImagePreprocessor::ImagePreprocessor()
{
    //try loading last camera parameter file
    try
    {
        FileStorage fs("config/config.txt", FileStorage::READ);
        std::string url;
        fs["lastCamParamURL"] >> url;
        fs.release();
        if(url != ""){ loadAll(QString::fromStdString(url)); }
    }
    catch(cv::Exception e)
    {
        QMessageBox::information(NULL, "Error", QString::fromStdString(e.msg), QMessageBox::Ok);
    }
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

void ImagePreprocessor::preproc(Mat& RGB, Mat& NIR, Mat& depth)
{
    //undistort
    Mat RGB_undist, NIR_undist, depth_undist;
    remap(RGB, RGB_undist, undistMapX_RGB, undistMapY_RGB, INTER_LINEAR, BORDER_CONSTANT, Scalar(0));
//    remap(NIR, NIR_undist, undistMapX_NIR, undistMapY_NIR, INTER_LINEAR, BORDER_CONSTANT, Scalar(0));
    remap(depth, depth_undist, undistMapX_IR, undistMapY_IR, INTER_LINEAR, BORDER_CONSTANT, Scalar(0));

    //TODO: resize / crop

    //map Kinect depth to RGB
    vector<Point3f> depth3D = projectDepthTo3DSpace(depth_undist);
    Mat depth2D = projectFrom3DSpaceToImage(depth3D, rotation_IR2RGB, transl_IR2RGB, cam_RGB,
                                            Size(depth_undist.cols, depth_undist.rows));

    //fill gaps in depth map
    Mat depth8bit, RGB_undist_gray;
    depth2D.convertTo(depth8bit, CV_8UC1, 255.0/2047.0);
    cvtColor(RGB_undist, RGB_undist_gray, CV_BGR2GRAY);
    Mat depth_refined = crossbilatFilter.filter(depth8bit, RGB_undist_gray, 16.0, 0.1);


    imshow("depth image", depth8bit);
    imshow("depth image refined", depth_refined);

//    imwrite("depth.png", depth8bit);
//    imwrite("rgb.png", RGB_undist);

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

    //also save URL of last saved parameter file
    FileStorage fs2("config/config.txt", FileStorage::WRITE);
    fs2 << "lastCamParamURL" << saveURL.toStdString();
    fs2.release();

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

    //also save URL of last loaded parameter file
    FileStorage fs2("config/config.txt", FileStorage::WRITE);
    fs2 << "lastCamParamURL" << loadURL.toStdString();
    fs2.release();

    qDebug() << "Loaded camera parameter file: " << loadURL;
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
    for (uint i = 0; i < imagePoints.size(); ++i)
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

vector<Point3f> ImagePreprocessor::projectDepthTo3DSpace(Mat depth)
{
    vector<Point3f> points3D(depth.rows * depth.cols);
    int cnt = 0;

    float fx = cam_IR.at<double>(0,0);
    float cx = cam_IR.at<double>(0,2);
    float fy = cam_IR.at<double>(1,1);
    float cy = cam_IR.at<double>(1,2);

    for (int y = 0; y < depth.rows; ++y)
    {
        for (int x = 0; x < depth.cols; ++x)
        {
            Point3f coord3D;
            ushort d = depth.at<ushort>(y,x);
            coord3D.x = (x - cx) * d / fx; //depth image is 16UC1 = ushort
            coord3D.y = (y - cy) * d / fy;
            coord3D.z = d;
            points3D[cnt++] = coord3D;
        }
    }
    return points3D;
}

Mat ImagePreprocessor::projectFrom3DSpaceToImage(std::vector<Point3f> points3D, Mat rot, Mat transl, Mat cam_Matrix, Size outImgSz)
{
//    qDebug() << IO::getOpenCVTypeName(img3D.type());

    vector<Point2f> points2D;
    projectPoints(points3D, rot, transl, cam_Matrix, noArray(), points2D);
//    projectPoints(points3D, rot, transl, Mat::zeros(3,3,CV_64F), noArray(), points2D);

    //transform vector holding 2D points into image matrix with depth values as intensity
    Mat img2D(outImgSz.height, outImgSz.width, CV_16UC1, Scalar(0));
    for (uint i = 0; i < points2D.size(); ++i)
    {
        int x = points2D[i].x;
        int y = points2D[i].y;
        int val = points3D[i].z;
        img2D.at<ushort>(y,x) = val;
    }

    return img2D;
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





