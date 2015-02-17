#include <QProgressDialog>
#include "imagepreprocessor.h"
#include "io.h"
#include "opencv2/objdetect/objdetect.hpp"
#include "helper.h"
#include "hog_crossspectralstereomatcher.h"
#include "locnorm_crossspectralstereomatcher.h"

/**************************************************************************
 **************************************************************************
 **************************************************************************
 * PUBLIC:
 **************************************************************************
 **************************************************************************
 *************************************************************************/

ImagePreprocessor::ImagePreprocessor(QWidget *parent):
    resizeFac_RGB(0.0),
    cams_are_calibrated_(false),
    rig_is_calibrated_(false),
    output_image_size_set_(false),
    stereo_Type_(crossSpectrSt_HOG)
{
    this->parent = parent;
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
        QMessageBox::information(parent, "Error", QString::fromStdString(e.msg), QMessageBox::Ok);
    }

    CSstereoMatcher = new HOG_crossSpectralStereoMatcher;
}


void ImagePreprocessor::calibCams(QStringList calibImgs_RGB,
                                  QStringList calibImgs_NIR,
                                  QStringList calibImgs_IR, Size chessboardSize)
{
    //Note: the calibration images used here should be recorded for each cam separately
    //in order to provide max accuracy for inidividual cams. the whole cam rig is calibrated
    //separately in calibRig()

    chessboardSz = chessboardSize;

    //read calibration images from disk
    QList<Mat> imgsRGB = readImgs2List(calibImgs_RGB);
    QList<Mat> imgsNIR = readImgs2List(calibImgs_NIR);
//    QList<Mat> imgsIR = readImgs2List(calibImgs_IR);

    //compute intrinsics and undistortion map
    make_Intrinsics_and_undistCoeffs(imgsRGB, cam_RGB, distCoeff_RGB, "RGB");
    make_Intrinsics_and_undistCoeffs(imgsNIR, cam_NIR, distCoeff_NIR, "NIR");
//    make_Intrinsics_and_undistCoeffs(imgsIR, cam_IR, distCoeff_IR, "IR");

    cams_are_calibrated_ = true;
    makeMsg("Success!", "Cameras have been calibrated.");
}

void ImagePreprocessor::calibRig(QStringList calibImgs_RGB, QStringList calibImgs_NIR, QStringList calibImgs_IR, Size chessboardSize)
{
    if(!cams_are_calibrated_){ return; }

    //Note: the calibration images used in this method should be recorded for the whole
    //camera rig, in order to make rectification possible and allow for alignment and stereo matching

    chessboardSz = chessboardSize;

    //read calibration images from disk
    QList<Mat> imgsRGB = readImgs2List(calibImgs_RGB);
    QList<Mat> imgsNIR = readImgs2List(calibImgs_NIR);
//    QList<Mat> imgsIR = readImgs2List(calibImgs_IR);

    //undistort RGB and NIR and store as temp images
    QProgressDialog progress("Temporarily undistorting RGB and NIR images", "Cancel", 0, imgsRGB.size(), parent);
    progress.setMinimumWidth(300); progress.setMinimumDuration(50); progress.setValue(0);
    QList<Mat> imgsRGB_undist;
    QList<Mat> imgsNIR_undist;

    for (int i = 0; i < imgsRGB.size(); ++i)
    {
        Mat rgb_ = imgsRGB[i];
        Mat nir_ = imgsNIR[i];
        Mat rgb_undist, nir_undist;

        undistort(rgb_, rgb_undist, cam_RGB, distCoeff_RGB);
        undistort(nir_, nir_undist, cam_NIR, distCoeff_NIR);
        imgsRGB_undist.append(rgb_undist);
        imgsNIR_undist.append(nir_undist);

        QCoreApplication::processEvents(); //make qt app responsive
        progress.setValue(i);
    }
    progress.setValue(imgsRGB.size());

    //make resize and crop parameters based on undistorted temp images
    if(!fitRGBimgs2NIRimgs(imgsNIR_undist, imgsRGB_undist))
    {
        return;
    }

    //resize calibration images
    QList<Mat> imgsRGB_resized;
    foreach (Mat img, imgsRGB_undist)
    {
        imgsRGB_resized.append( resizeAndCropRGBImg(img) );
    }

    //calibrate RGB cam AGAIN with recized/cropped images
    //TODO: should yet another set im chessboard images be used for this step??
    make_Intrinsics_and_undistCoeffs(imgsRGB_resized, cam_RGB_resized, distCoeff_RGB_resized, "RGB_resized");
//    foreach (Mat img, imgsRGB_resized)
//    {
//        Mat img2;
//        undistort(img, img2, cam_RGB_resized, distCoeff_RGB_resized);
//        imshow("", img2); cvWaitKey(0);
//    }

    //make rectify maps
//    make_RectifyMaps(imgsIR, imgsRGB, distCoeff_IR, distCoeff_RGB, cam_IR, cam_RGB,
//                     rectifyMapX_IR, rectifyMapY_IR, rectifyMapX_RGB, rectifyMapY_RGB,
//                     rotation_IR2RGB, transl_IR2RGB, "IR", "RGB");
    make_RectifyMaps(imgsNIR, imgsRGB_resized, distCoeff_NIR, distCoeff_RGB_resized, cam_NIR, cam_RGB_resized,
                     rectifyMapX_NIR, rectifyMapY_NIR, rectifyMapX_RGB, rectifyMapY_RGB,
                     rotation_NIR2RGB, transl_NIR2RGB, "NIR_rectify", "RGB_rectify");


    rig_is_calibrated_ = true;
    makeMsg("Success!", "Camera rig has been calibrated.");
}

void ImagePreprocessor::preproc(Mat RGB, Mat NIR, Mat depth_kinect, Mat& RGB_out, Mat& NIR_out, Mat& depth_stereo_out, Mat& depth_remapped_out)
{
    ////////////////////////////////////////////////////////////////////////////////////////
    /// TEST CROSS SPECTRAL STEREO MATCHING ON TEST STEREO IMAGES WITHOUT RECTIFICATION
    //make disparity image from RGB and NIR images (cross/multi - spectral)
    Mat disp2;
    makeCrossSpectralStereo(RGB, NIR, disp2);

    //put results in output imgs
    RGB_out = RGB;
    NIR_out = NIR;
//    depth_remapped_out = depth_rect;
    depth_stereo_out = disp2;
    return;
    ///endof TEST CROSS SPECTRAL STEREO MATCHING ON TEST STEREO IMAGES WITHOUT RECTIFICATION
    /////////////////////////////////////////////////////////////////////////////////////////

    if(!rig_is_calibrated_){ return; }

    //undistort, resize and crop RGB img
    Mat RGB_undist;
    undistort(RGB, RGB_undist, cam_RGB, distCoeff_RGB);
    Mat RGB_resized = resizeAndCropRGBImg(RGB_undist);

//    //map Kinect depth to RGB
//    vector<Point3f> depth3D = projectDepthTo3DSpace(depth);
//    Mat depth2D = projectFrom3DSpaceToImage(depth3D, rotation_IR2RGB, transl_IR2RGB, cam_RGB,
//                                            Size(depth.cols, depth.rows));

//    //fill gaps in depth map
//    Mat depth8bit, RGB_undist_gray;
//    depth2D.convertTo(depth8bit, CV_8UC1, 255.0/2047.0);
//    cvtColor(RGB, RGB_undist_gray, CV_BGR2GRAY);
//    Mat depth_refined = crossbilatFilter.filter(depth8bit, RGB_undist_gray, 16.0, 0.1);

    //rectify
    Mat RGB_rect, NIR_rect, depth_rect;
    remap(RGB_resized, RGB_rect, rectifyMapX_RGB, rectifyMapY_RGB, INTER_LINEAR, BORDER_CONSTANT, Scalar(0));
    remap(NIR, NIR_rect, rectifyMapX_NIR, rectifyMapY_NIR, INTER_LINEAR, BORDER_CONSTANT, Scalar(0));
//    remap(depth_refined, depth_rect, rectifMapX_RGB, rectifMapY_RGB, INTER_LINEAR, BORDER_CONSTANT, Scalar(0));
        //(rectify kinect depth  with RGB params, because it was mapped to RGB)

    //if set, resize the images to a new output (smaller) size (might be better/faster for CNN learning
    // and improves performance of cross-spectral stereo matching)
    if(output_image_size_set_)
    {
        resize(RGB_rect, RGB_out, outputImgSz, 0, 0, INTER_AREA); //area averaging best for downsampling
        resize(NIR_rect, NIR_out, outputImgSz, 0, 0, INTER_AREA);
//        resize(depth_rect, depth_remapped_out, outputImgSz, 0, 0, INTER_NEAREST); //for depth no in-between interpolation!
    }
    else
    {
        //put results in output imgs
        RGB_out = RGB_rect;
        NIR_out = NIR_rect;
//        depth_remapped_out = depth_rect;
    }

    //make disparity image from resized RGB and NIR images (cross/multi - spectral)
    Mat disp;
    makeCrossSpectralStereo(RGB_out, NIR_out, disp);
    depth_stereo_out = disp;
}

/**************************************************************************
 **************************************************************************
 **************************************************************************
 * GETTER & SETTER:
 **************************************************************************
 **************************************************************************
 *************************************************************************/
void ImagePreprocessor::OutputImageSize(int w, int h)
{
    outputImgSz.width = w;
    outputImgSz.height = h;
    output_image_size_set_ = true;
}

void ImagePreprocessor::set_CSstereoType(CrossSpectralStereoType type)
{
    stereo_Type_ = type;
    if(type == crossSpectrSt_HOG)
    {
        CSstereoMatcher = new HOG_crossSpectralStereoMatcher;
    }
    else if(type == crossSpectrSt_LocNorm)
    {
        CSstereoMatcher = new LocNorm_crossSpectralStereoMatcher;
    }
}


/**************************************************************************
 **************************************************************************
 **************************************************************************
 * PRIVATE:
 **************************************************************************
 **************************************************************************
 *************************************************************************/

void ImagePreprocessor::make_Intrinsics_and_undistCoeffs(QList<Mat> calibImgs, Mat &camIntrinsics, Mat &distCoeff, QString imgType)
{
    //if set, use output image size, else use size of input image
    Size imgSize = calibImgs.first().size();

    int nrOfCorners = chessboardSz.width * chessboardSz.height;  //total number of inner corners

    //objectPoints should contain physical location of each corners but
    //since we don’t know that so we assign constant positions to all the corners
    vector<Point3f> obj;
    for (int i = 0; i < nrOfCorners; i++){ obj.push_back(Point3f(i / chessboardSz.width, i % chessboardSz.width, 0.0f)); }

    //get the object and image points
    vector<vector<Point3f> > objectPoints;
    vector<vector<Point2f> > imagePoints;
    QList<int> blackList; //will contain image indices for which chessboard corners couldn't be found
    getObjectAndImagePoints(calibImgs, chessboardSz.width, chessboardSz.height, obj, objectPoints, imagePoints,
                            imgType, blackList, false);

    //get intrinsics
    vector<Mat> rvecs, tvecs; //dummy
    calibrateCamera(objectPoints, imagePoints, imgSize, camIntrinsics, distCoeff, rvecs, tvecs);
}


void ImagePreprocessor::make_RectifyMaps(QList<Mat> srcImgs, QList<Mat> dstImgs,
                                         Mat distCoeff_src, Mat distCoeff_dst, Mat cam_src, Mat cam_dst,
                                         Mat &out_rectifMapX_src, Mat &out_rectifMapY_src, Mat &out_rectifMapX_dst, Mat &out_rectifMapY_dst,
                                         Mat &out_Rot, Mat &out_Transl,
                                         QString imgTypeSrc, QString imgTypeDst)
{
    //if set, use output image size, else use size of input image
    Size imgSize = srcImgs.first().size();

    int nrOfCorners = chessboardSz.width * chessboardSz.height;  //total number of inner corners
    vector<Point3f> obj;
    for (int i = 0; i < nrOfCorners; i++){ obj.push_back(Point3f(i / chessboardSz.width, i % chessboardSz.width, 0.0f)); }

    //get the object and image points
    vector<vector<Point3f> > objPoints_src;
    vector<vector<Point3f> > objPoints_dst;
    vector<vector<Point2f> > imgPoints_src;
    vector<vector<Point2f> > imgPoints_dst;

    QList<int> blackList; //will contain image indices for which chessboard corners couldn't be found
    getObjectAndImagePoints(srcImgs, chessboardSz.width, chessboardSz.height, obj, objPoints_src, imgPoints_src,
                            imgTypeSrc, blackList, true);
    getObjectAndImagePoints(dstImgs, chessboardSz.width, chessboardSz.height, obj, objPoints_dst, imgPoints_dst,
                            imgTypeDst, blackList, true);


    //check wether all points have been found for left and right images, eventually create new "good" lists
    vector<vector<Point3f> > objPoints_;
    vector<vector<Point2f> > imgPoints_src_;
    vector<vector<Point2f> > imgPoints_dst_;

    if(blackList.size() != 0) //=there have been cases where no corners could be found
    {
        QMessageBox::information(parent, "Problem making rectify maps", "Couldn't find all corners, using only good subset", QMessageBox::Ok);

        //create new lists without the problematic entries, i.e. where for one of the image types no corners could be found
        for (int i = 0; i < objPoints_src.size(); ++i)
        {
            if(!blackList.contains(i))
            {
                objPoints_.push_back(objPoints_src[i]);
                imgPoints_src_.push_back(imgPoints_src[i]);
                imgPoints_dst_.push_back(imgPoints_dst[i]);
            }
        }
    }
    else
    {
        objPoints_ = objPoints_src;
        imgPoints_src_ = imgPoints_src;
        imgPoints_dst_ = imgPoints_dst;
    }

    //calibrate as stereo cameras
    Mat /*Rot, Transl,*/ E, F, Rect_src, Rect_dst, Proj_src, Proj_dst, Q;
    stereoCalibrate(objPoints_,
                    imgPoints_src_, imgPoints_dst_,
                    cam_src, distCoeff_src,
                    cam_dst, distCoeff_dst,
                    imgSize, out_Rot, out_Transl, E, F);

    stereoRectify(cam_src, distCoeff_src,
                  cam_dst, distCoeff_dst, imgSize, out_Rot, out_Transl,
                  Rect_src, Rect_dst, Proj_src, Proj_dst, Q, CALIB_ZERO_DISPARITY,
                  0); //set alpha to zero -> no black areas

    //make the rectify maps (rgb is computed every time, because all other images are mapped to RGB)
    initUndistortRectifyMap(cam_src, distCoeff_src, Rect_src, Proj_src, imgSize, CV_32FC1, out_rectifMapX_src, out_rectifMapY_src);
    initUndistortRectifyMap(cam_dst, distCoeff_dst, Rect_dst, Proj_dst, imgSize, CV_32FC1, out_rectifMapX_dst, out_rectifMapY_dst);
}

bool ImagePreprocessor::fitRGBimgs2NIRimgs(QList<Mat> origNirs, QList<Mat> origRGBs)
{
    if(origNirs.count() != origRGBs.count()) //amount of both lists must be equal
    {
        QMessageBox::information(parent, "Error while fitting RGB to NIR image", "Not same amount of RGB and NIR images.", QMessageBox::Ok);
        return false;
    }

    QProgressDialog progress("Fitting RGB to NIR image", "Cancel", 0, origNirs.size(), parent);
    progress.setMinimumWidth(300);
    progress.setMinimumDuration(100);
    progress.setValue(0);

    // 1) find chessboard corners
    vector<vector<Point2f> > cornersNIR;
    vector<vector<Point2f> > cornersRGB;
    bool all_is_well = true;
    int foundInNIR = origNirs.count();
    int foundInRGB = origRGBs.count();
    QList<int> badImgs; //images in which the chessboard corners couldnt be found either in RGB or NIR

    for (int i = 0; i < origNirs.count(); ++i)
    {
        Mat imgNIR, imgNIR_gray, imgRGB, imgRGB_gray;
        vector<Point2f> imgCornersNIR, imgCornersRGB;

        //NIR:
        imgNIR = origNirs.at(i);
        if(imgNIR.channels() > 1){ cvtColor(imgNIR, imgNIR_gray, CV_BGR2GRAY); }//must be gray scale
        else{ imgNIR_gray = imgNIR; }

        bool success = findChessboardCorners(imgNIR_gray, chessboardSz, imgCornersNIR, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
        if(!success)
        {
            all_is_well = false;
            badImgs.append(i);
            foundInNIR--;
        }
        cornersNIR.push_back(imgCornersNIR);//store cornery anyways, if not found, this index will be jumped later because of badImgs list

        //RGB:
        imgRGB = origRGBs.at(i);
        if(imgRGB.channels() > 1){ cvtColor(imgRGB, imgRGB_gray, CV_BGR2GRAY); }//must be gray scale
        else{ imgRGB_gray = imgRGB; }

        success = findChessboardCorners(imgRGB_gray, chessboardSz, imgCornersRGB, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
        if(!success)
        {
            all_is_well = false;
            if(!badImgs.contains(i)){ badImgs.append(i); }
            foundInRGB--;
        }
        cornersRGB.push_back(imgCornersRGB);//store corners anyways, ...

        QCoreApplication::processEvents(); //make qt app responsive
        progress.setValue(i);
        if(progress.wasCanceled()){ return false; }
    }
    progress.setValue(origNirs.size());
    if(!all_is_well)
    {
        makeMsg("Not all corners found", "OK only in "+QString::number(foundInRGB)+"/"+QString::number(origRGBs.size())
                +" RGB and "+QString::number(foundInNIR)+"/"+QString::number(origNirs.size())+" NIR images");
    }

    // 2) get average resize factor by comparing corner distances for NIR and RGB images
    int nrOfImages = cornersNIR.size();
    int nrOfCorners = chessboardSz.height * chessboardSz.width;//cornersNIR[0].size();
    double avgResizeFactor = 0.0;
    for(int i = 0; i < nrOfImages; ++i)
    {
        //images in which corners couldn't be found must be skipped
        if(badImgs.contains(i)){ continue; }

        //get distances between chessboard corners for each image type (NIR and RGB)
        //and compute the relationship / resize factor
        for (int j = 0; j < nrOfCorners-1; ++j)
        {
            Point2f diff_NIR = cornersNIR[i][j] - cornersNIR[i][j+1];
            double euclDist_NIR = sqrt(diff_NIR.x * diff_NIR.x + diff_NIR.y * diff_NIR.y);

            Point2f diff_RGB = cornersRGB[i][j] - cornersRGB[i][j+1];
            double euclDist_RGB = sqrt(diff_RGB.x * diff_RGB.x + diff_RGB.y * diff_RGB.y);

            double resizeFactor = euclDist_NIR / euclDist_RGB;
            avgResizeFactor += resizeFactor;
        }
    }
    //get average
    int nrOfGoodImgs = nrOfImages - badImgs.size();
    avgResizeFactor /= (nrOfGoodImgs * nrOfCorners); //divide by all corners in all (good) imgs

    // 3) resize on of the RGB images
    Mat resized;
    resize(origRGBs.at(0), resized, Size(), avgResizeFactor, avgResizeFactor, INTER_AREA);

    // 4) with the resized RGB image, compute the crop rectangle
    int nirw = origNirs.at(0).cols;
    int nirh = origNirs.at(0).rows;
    int rgbw = resized.cols;
    int rgbh = resized.rows;
    Rect cropRect( (rgbw-nirw)/2, (rgbh-nirh)/2, nirw, nirh );

    //save resize and drop params for later and return resized rgb image
    resizeFac_RGB = avgResizeFactor;
    cropRect_RGB = cropRect;
    return true;
}

Mat ImagePreprocessor::resizeAndCropRGBImg(Mat rgbImg)
{
    Mat resized, cropped;
    resize(rgbImg, resized, Size(), resizeFac_RGB, resizeFac_RGB, INTER_AREA); //for downsampling, area averaging is best
    resized(cropRect_RGB).copyTo(cropped);
    return cropped;
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
            ushort d = depth.at<ushort>(y,x); //depth image is 16UC1 = ushort
            coord3D.x = (x - cx) * d / fx;
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

void ImagePreprocessor::makeCrossSpectralStereo(Mat imgRGB_L, Mat imgNIR_R, Mat& out_disp)
{
    Mat rgb_gray; //rgb to grayscale
    cvtColor(imgRGB_L, rgb_gray, CV_BGR2GRAY);

    CSstereoMatcher->process(rgb_gray, imgNIR_R, out_disp);
}



/**************************************************************************
 **************************************************************************
 **************************************************************************
 * HELPER:
 **************************************************************************
 **************************************************************************
 *************************************************************************/
void ImagePreprocessor::getObjectAndImagePoints(QList<Mat> calibImgs, int width, int height,
                                                vector<Point3f> obj,
                                                vector<vector<Point3f> >& objectPoints,
                                                vector<vector<Point2f> >& imagePoints,
                                                QString imgType, QList<int>& blacklist, bool useBlacklist)
{
    Size chessboard_sz = Size(width, height);

    //get chessboard corners for all images
    getImagePoints(calibImgs, chessboard_sz, imagePoints, imgType, useBlacklist, blacklist);

    //put as many of the dummy object points in the list as there are image points
    for (uint i = 0; i < imagePoints.size(); ++i)
    {
        objectPoints.push_back(obj);
    }
}

void ImagePreprocessor::getImagePoints(QList<Mat> calibImgs, Size chessboardSize, vector<vector<Point2f> > &imagePoints,
                                       QString imgType, bool useBlacklist, QList<int> &blacklist)
{
    QProgressDialog progress("Getting image points for " + imgType + " image", "Cancel", 0, calibImgs.size(), parent);
    progress.setMinimumWidth(300);
    progress.setMinimumDuration(100);
    progress.setValue(0);

    //get chessboard corners for all images
    uint cnt = 0;
    bool success = false;
    foreach(Mat img, calibImgs)
    {
        vector<Point2f> corners;
        success = findChessboardCorners(img, chessboardSize, corners,
                                        CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
        if(success){ imagePoints.push_back(corners); }
        else
        {
            makeImgRelatedMsg(img, "Error getting image points", "Couldn't get image points in " + imgType + " img " + QString::number(cnt+1));
            if(useBlacklist)
            {
                if(!blacklist.contains(cnt)){ blacklist.append(cnt); }
                imagePoints.push_back(corners); //push back anyways, entry is gonna be skipped using blacklist later
            }
        }

        QCoreApplication::processEvents(); //make qt app responsive
        progress.setValue(cnt++);
        if(progress.wasCanceled()){ return; }
    }
    progress.setValue(calibImgs.size());
}


QList<Mat> ImagePreprocessor::readImgs2List(QStringList imgNames)
{
    QProgressDialog progress("Loading images", "Cancel Load", 0, imgNames.size(), parent);
    progress.setMinimumWidth(300);
    progress.setMinimumDuration(100);
    progress.setValue(0);

    int cnt = 0;
    QList<Mat> imgs;
    foreach (QString s, imgNames)
    {
        imgs.append(imread(s.toStdString(), IMREAD_GRAYSCALE));

        QCoreApplication::processEvents(); //make qt app responsive
        progress.setValue(cnt++);
        if(progress.wasCanceled()){ return imgs; }
    }
    progress.setValue(imgNames.size());
    return imgs;
}

void ImagePreprocessor::makeMsg(QString title, QString msg)
{
    QMessageBox::information(parent, title, msg, QMessageBox::Ok);
}

void ImagePreprocessor::makeImgRelatedMsg(Mat img, QString title, QString msg)
{
    namedWindow(title.toStdString(), CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO | CV_GUI_EXPANDED);
    imshow(title.toStdString(), img);
    QMessageBox::information(parent, title, msg, QMessageBox::Ok);
    destroyWindow(title.toStdString());
}



/**************************************************************************
 **************************************************************************
 **************************************************************************
 * PUBLIC LOAD & SAVE METHODS:
 **************************************************************************
 **************************************************************************
 *************************************************************************/
void ImagePreprocessor::saveAll(QString saveURL)
{
    FileStorage fs(saveURL.toStdString(), FileStorage::WRITE);
    fs << "cam_RGB" << cam_RGB;
    fs << "cam_RGB_resized" << cam_RGB_resized;
    fs << "cam_NIR" << cam_NIR;
    fs << "cam_IR" << cam_IR;

    fs << "distCoeff_RGB" << distCoeff_RGB;
    fs << "distCoeff_RGB_resized" << distCoeff_RGB_resized;
    fs << "distCoeff_NIR" << distCoeff_NIR;
    fs << "distCoeff_IR" << distCoeff_IR;

    fs << "resizeFac_RGB" << resizeFac_RGB;
//    fs << "resizeFac_NIR" << resizeFac_NIR;
//    fs << "resizeFac_IR" << resizeFac_IR;

    fs << "cropRect_RGB" << cropRect_RGB;
//    fs << "cropRect_NIR" << cropRect_NIR;
//    fs << "cropRect_IR" << cropRect_IR;

    fs << "rectifMapX_RGB" << rectifyMapX_RGB;
    fs << "rectifMapY_RGB" << rectifyMapY_RGB;
    fs << "rectifMapX_NIR" << rectifyMapX_NIR;
    fs << "rectifMapY_NIR" << rectifyMapY_NIR;
    fs << "rectifMapX_IR" << rectifMapX_IR;
    fs << "rectifMapY_IR" << rectifMapY_IR;

    fs << "rotation_NIR2RGB" << rotation_NIR2RGB;
    fs << "rotation_IR2RGB" << rotation_IR2RGB;
    fs << "transl_NIR2RGB" << transl_NIR2RGB;
    fs << "transl_IR2RGB" << transl_IR2RGB;

    fs << "cams_are_calibrated_" << cams_are_calibrated_;
    fs << "rig_is_calibrated_" << rig_is_calibrated_;

    fs.release();

    //also save URL of last saved parameter file
    FileStorage fs2("config/config.txt", FileStorage::WRITE);
    fs2 << "lastCamParamURL" << saveURL.toStdString();
    fs2.release();

    makeMsg("Success!", "Calibration file saved");
}

void ImagePreprocessor::loadAll(QString loadURL)
{
    FileStorage fs(loadURL.toStdString(), FileStorage::READ);
    fs["cam_RGB"] >> cam_RGB;
    fs["cam_RGB_resized"] >> cam_RGB_resized;
    fs["cam_NIR"] >> cam_NIR;
    fs["cam_IR"] >> cam_IR;

    fs["distCoeff_RGB"] >> distCoeff_RGB;
    fs["distCoeff_RGB_resized"] >> distCoeff_RGB_resized;
    fs["distCoeff_NIR"] >> distCoeff_NIR;
    fs["distCoeff_IR"] >> distCoeff_IR;

    fs["resizeFac_RGB"] >> resizeFac_RGB;
//    fs["resizeFac_NIR"] >> resizeFac_NIR;
//    fs["resizeFac_IR"] >> resizeFac_IR;

    fs["cropRect_RGB"] >> cropRect_RGB;
//    fs["cropRect_NIR"] >> cropRect_NIR;
//    fs["cropRect_IR"] >> cropRect_IR;

    fs["rectifMapX_RGB"] >> rectifyMapX_RGB;
    fs["rectifMapY_RGB"] >> rectifyMapY_RGB;
    fs["rectifMapX_NIR"] >> rectifyMapX_NIR;
    fs["rectifMapY_NIR"] >> rectifyMapY_NIR;
    fs["rectifMapX_IR"] >> rectifMapX_IR;
    fs["rectifMapY_IR"] >> rectifMapY_IR;

    fs["rotation_NIR2RGB"] >> rotation_NIR2RGB;
    fs["rotation_IR2RGB"] >> rotation_IR2RGB;
    fs["transl_NIR2RGB"] >> transl_NIR2RGB;
    fs["transl_IR2RGB"] >> transl_IR2RGB;

    fs["cams_are_calibrated_"] >> cams_are_calibrated_;
    fs["rig_is_calibrated_"] >> rig_is_calibrated_;

    fs.release();

    //also save URL of last loaded parameter file
    FileStorage fs2("config/config.txt", FileStorage::WRITE);
    fs2 << "lastCamParamURL" << loadURL.toStdString();
    fs2.release();

    makeMsg("Success!", "Calibration file loaded");
}



