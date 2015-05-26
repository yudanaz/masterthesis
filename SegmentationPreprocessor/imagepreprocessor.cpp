#include <QProgressDialog>
#include "imagepreprocessor.h"
#include "io.h"
#include "opencv2/objdetect/objdetect.hpp"
#include "helper.h"
#include "hog_crossspectralstereomatcher.h"
#include "locnorm_crossspectralstereomatcher.h"
#include "CThinPlateSpline.h"

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
	stereo_Type_(crossSpectrSt_HOG),
	normalizeDepth(false), makeSkinBinaryImage(false),
	RGBregist_distortPerspective(false), RGBregist_thinPlateSpline(false),
	xoffsetSUM(0), xoffsetCnt(0)
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
	QList<Mat> imgsNIR_all = readImgs2List(calibImgs_NIR, true);
	QList<Mat> imgsIR = readImgs2List(calibImgs_IR);

	//create separate lists for the NIR channels
	QList<Mat> imgsNIR_970, imgsNIR_1300, imgsNIR_1550;
	foreach(Mat nir, imgsNIR_all)
	{
		vector<Mat> nirChannels(3);
		split(nir, nirChannels);
		imgsNIR_970.push_back(nirChannels[0]);
		imgsNIR_1300.push_back(nirChannels[1]);
		imgsNIR_1550.push_back(nirChannels[2]);
	}

	//compute intrinsics and undistortion map for NIR channels
	make_Intrinsics_and_undistCoeffs(imgsNIR_970, cam_NIR_970, distCoeff_NIR_970, "NIR (970 nm)");
	make_Intrinsics_and_undistCoeffs(imgsNIR_1300, cam_NIR_1300, distCoeff_NIR_1300, "NIR (1300 nm)");
	make_Intrinsics_and_undistCoeffs(imgsNIR_1550, cam_NIR_1550, distCoeff_NIR_1550, "NIR (1550 nm)");

	//undistort images channel-wise
	QList<Mat> imgsNIR_all_undist = undistortNIRimagesChannelWise(imgsNIR_all);

	//compute intrinsics and undistortion maps for channel-wise undistorted NIR
	make_Intrinsics_and_undistCoeffs(imgsNIR_all_undist, cam_NIR_all, distCoeff_NIR_all, "NIR (all channels)");

	//compute intrinsics and undistortion map for RGB and IR
	make_Intrinsics_and_undistCoeffs(imgsRGB, cam_RGB, distCoeff_RGB, "RGB");
	make_Intrinsics_and_undistCoeffs(imgsIR, cam_IR, distCoeff_IR, "IR");

	cams_are_calibrated_ = true;
	makeMsg("Success!", "Cameras have been calibrated.");
}

void ImagePreprocessor::calibRig(QStringList calibImgs_RGB, QStringList calibImgs_NIR_rgb,
								 QStringList calibImgs_IR, QStringList calibImgsNIR_ir, Size chessboardSize)
{
	if(!cams_are_calibrated_){ return; }

	//Note: the calibration images used in this method should be recorded for the whole
	//camera rig, in order to make rectification possible and allow for alignment and stereo matching

	chessboardSz = chessboardSize;

	//read calibration images from disk
	QList<Mat> imgsRGB = readImgs2List(calibImgs_RGB);
	QList<Mat> imgsNIR_rgb = readImgs2List(calibImgs_NIR_rgb, true);
	QList<Mat> imgsIR = readImgs2List(calibImgs_IR);
	QList<Mat> imgsNIR_ir = readImgs2List(calibImgsNIR_ir, true);

	///////////////////////////////////////////////////////////////////////////
	/// UNDISTORT NIR IMAGES CHANNEL-WISE
	///////////////////////////////////////////////////////////////////////////
	QList<Mat> imgsNIR_ir_undist = undistortNIRimagesChannelWise(imgsNIR_ir);
	QList<Mat> imgsNir_rgb_undist = undistortNIRimagesChannelWise(imgsNIR_rgb);
	///////////////////////////////////////////////////////////////////////////
	/// endof UNDISTORT NIR IMAGES CHANNEL-WISE
	///////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////
	/// RESIZE RGB IMAGES (and make new intrinsics and undistort coeff.)
	///////////////////////////////////////////////////////////////////////////
	//undistort RGB and NIR and store as temp images
	QProgressDialog progress("Temporarily undistorting RGB and NIR images", "Cancel", 0, imgsRGB.size(), parent);
	progress.setMinimumWidth(300); progress.setMinimumDuration(50); progress.setValue(0);
	QList<Mat> imgsRGB_undist;
	QList<Mat> imgsNIR_undist;

	for (int i = 0; i < imgsRGB.size(); ++i)
	{
		Mat rgb_ = imgsRGB[i];
		Mat nir_ = imgsNir_rgb_undist[i];
		Mat rgb_undist, nir_undist;

		undistort(rgb_, rgb_undist, cam_RGB, distCoeff_RGB);
		undistort(nir_, nir_undist, cam_NIR_all, distCoeff_NIR_all);
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

	//resize RGB calibration images
	QList<Mat> imgsRGB_resized;
	foreach (Mat img, imgsRGB_undist)
	{
		imgsRGB_resized.append( resizeAndCropRGBImg(img) );
	}

	//calibrate RGB cam AGAIN with recized/cropped images
	//(TODO: should yet another set im chessboard images be used for this step??)
	make_Intrinsics_and_undistCoeffs(imgsRGB_resized, cam_RGB_resized, distCoeff_RGB_resized, "RGB_resized");
	///////////////////////////////////////////////////////////////////////////
	/// endof RESIZE RGB IMAGES
	///////////////////////////////////////////////////////////////////////////

	//make rectify maps for IR -> NIR mapping
	Mat dummy1, dummy2, dummy3; //use dummies because NIR should compose a stereo pair with RGB, and not with IR
	make_RectifyMaps(imgsIR, imgsNIR_ir_undist, distCoeff_IR, distCoeff_NIR_all, cam_IR, cam_NIR_all,
					 rectifyMapX_IR, rectifyMapY_IR, dummy1, dummy2,
					 rotation_IR2NIR, transl_IR2NIR, proj_IR2NIR, "IR (for IR->NIR)", "NIR (for IR->NIR)");

	//make rectify maps for RGB -> NIR mapping
	make_RectifyMaps(imgsRGB_resized, imgsNir_rgb_undist, distCoeff_RGB_resized, distCoeff_NIR_all, cam_RGB_resized, cam_NIR_all,
					 rectifyMapX_RGB, rectifyMapY_RGB, rectifyMapX_NIR, rectifyMapY_NIR,
					 rotation_RGB2NIR, transl_RGB2NIR, dummy3, "RGB (for RGB->NIR)", "NIR (for RGB->NIR)");

	rig_is_calibrated_ = true;
	makeMsg("Success!", "Camera rig has been calibrated.");
}

void ImagePreprocessor::preproc(Mat RGB, Mat NIR, Mat depth_kinect, Mat& RGB_out, Mat& NIR_out, Mat& depth_stereo_out, Mat& depth_remapped_out, Mat& skin_out)
{
	if(!rig_is_calibrated_){ return; }

	//undistort, resize and crop RGB img
	Mat RGB_undist;
	undistort(RGB, RGB_undist, cam_RGB, distCoeff_RGB);
	Mat RGB_resized = resizeAndCropRGBImg(RGB_undist);
//	Helper::debugImage(RGB_resized);

	//undistort NIR image channel-wise AND swap channels (so that the order is 1550, 1300, 970 in the BGR image)
	Mat NIR_undist = undistortNIRimgChannelWise(NIR);
    imwrite("NIR_undist.png", NIR_undist);

	//detect skin
	Mat skin = skinDetector.detect(NIR_undist)*255;

	//remap depth to NIR-camera space
	Mat depth_remapped = mapKinectDepth2NIR(depth_kinect, NIR_undist);

	//rectify
	Mat RGB_rect, NIR_rect, skin_rect;
	remap(RGB_resized, RGB_rect, rectifyMapX_RGB, rectifyMapY_RGB, INTER_LINEAR, BORDER_CONSTANT, Scalar(0));
	remap(NIR_undist, NIR_rect, rectifyMapX_NIR, rectifyMapY_NIR, INTER_LINEAR, BORDER_CONSTANT, Scalar(0));
	remap(skin, skin_rect, rectifyMapX_NIR, rectifyMapY_NIR, INTER_LINEAR, BORDER_CONSTANT, Scalar(0));
//	imwrite("RGB_rect.png", RGB_rect);
//	imwrite("NIR_rect.png", NIR_rect);

	//make disparity image from RGB and NIR images (cross/multi - spectral)
	Mat disp;
	if(makeCSStereo){ makeCrossSpectralStereo(NIR_rect, RGB_rect, disp); }
	else{ disp = Mat(NIR_rect.size(), CV_8UC1); } //empty img

	//register RGB to NIR image using HOG descriptors
	Mat RGB_registered = registerRGB2NIR(RGB_rect, NIR_rect);

	//crop images according to registered RGB and remapped kinect depth
	Rect finalCropRect = makeMinimalCrop(finalCropRect_byKinectDepth, finalCropRect_byRGB);
	Mat NIR_cropped = cropImage(NIR_rect, finalCropRect);
	Mat RGB_cropped = cropImage(RGB_registered, finalCropRect);
	Mat depth_cropped = cropImage(depth_remapped, finalCropRect);
	Mat disp_cropped = cropImage(disp, finalCropRect);
	Mat skin_cropped = cropImage(skin_rect, finalCropRect);

	//fill eventually remaining black spots close to borders in depth map
	depth_cropped = fixHolesInDepthMap(depth_cropped, 0);
	depth_cropped = fixHolesInDepthMap(depth_cropped, 1);
	depth_cropped = fixHolesInDepthMap(depth_cropped, 2);
	depth_cropped = fixHolesInDepthMap(depth_cropped, 3);

	//if set, resize the images to a new output (smaller) size (might be better/faster for CNN learning
	// and improves performance of cross-spectral stereo matching)
	Mat RGB_reg_small, NIR_small, depth_remapped_small, disp_small, skin_small;
	if(output_image_size_set_)
	{
		resize(NIR_cropped, NIR_small, outputImgSz, 0, 0, INTER_AREA);
		resize(RGB_cropped, RGB_reg_small, outputImgSz, 0, 0, INTER_AREA); //area averaging best for downsampling
		resize(depth_cropped, depth_remapped_small, outputImgSz, 0, 0, INTER_NEAREST); //for depth no in-between interpolation!
		resize(disp_cropped, disp_small, outputImgSz, 0, 0, INTER_NEAREST); //for depth no in-between interpolation!
		resize(skin_cropped, skin_small, outputImgSz, 0, 0, INTER_NEAREST); //for skin no in-between interpolation!
	}
	else
	{
		//put results in output imgs
		NIR_small = NIR_cropped;
		RGB_reg_small = RGB_cropped;
		depth_remapped_small = depth_cropped;
		disp_small = disp_cropped;
		skin_small = skin_cropped;
	}

	//set the final outputs
	NIR_out = NIR_small.clone();
	RGB_out = RGB_reg_small.clone();
	depth_remapped_out = depth_remapped_small.clone();
	depth_stereo_out = disp_small.clone();
	skin_out = skin_small.clone();
}



/**************************************************************************
 **************************************************************************
 **************************************************************************
 * GETTER & SETTER:
 **************************************************************************
 **************************************************************************
 *************************************************************************/
void ImagePreprocessor::setParameters(std::vector<float> params)
{
	CSstereoMatcher->setParams(params);
}

void ImagePreprocessor::setOptions(bool normDepth, bool makeSkinImg, bool makeCSStereo, bool rgbRegist_dist, bool rgbRegist_tpspline)
{
	this->normalizeDepth = normDepth;
	this->makeSkinBinaryImage = makeSkinImg;
	this->makeCSStereo = makeCSStereo;
	this->RGBregist_distortPerspective = rgbRegist_dist;
	this->RGBregist_thinPlateSpline = rgbRegist_tpspline;
}

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
										 Mat &out_Rot, Mat &out_Transl, Mat &Proj_dst,
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
	Mat /*Rot, Transl,*/ E, F, Rect_src, Rect_dst, Proj_src, Q;
	stereoCalibrate(objPoints_,
					imgPoints_src_, imgPoints_dst_,
					cam_src, distCoeff_src,
					cam_dst, distCoeff_dst,
					imgSize, out_Rot, out_Transl, E, F);

	stereoRectify(cam_src, distCoeff_src,
				  cam_dst, distCoeff_dst, imgSize, out_Rot, out_Transl,
				  Rect_src, Rect_dst, Proj_src, Proj_dst, Q, CALIB_ZERO_DISPARITY,
				  0); //set alpha = 0 ==> zoom and shift so no black areas remain after rectification
//				  1);

	//make the rectify maps
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
		makeMsg("Not all corners found while fitting RGB -> NIR", "OK only in "+QString::number(foundInRGB)+"/"+QString::number(origRGBs.size())
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

QList<Mat> ImagePreprocessor::undistortNIRimagesChannelWise(QList<Mat> imgsNIR, bool convert2grayscale)
{
	QList<Mat> imgsNIR_undist;
	foreach(Mat nir, imgsNIR)
	{
		Mat nir_undist = undistortNIRimgChannelWise(nir);
		if(convert2grayscale)
		{
			Mat nir_undist_gray;
			cvtColor(nir_undist, nir_undist_gray, CV_BGR2GRAY);
			imgsNIR_undist.push_back(nir_undist_gray);
		}
		else
		{
			imgsNIR_undist.push_back(nir_undist);
		}
	}
	return imgsNIR_undist;
}

Mat ImagePreprocessor::undistortNIRimgChannelWise(Mat imgNIR)
{
	Mat imgNIR_undist;
	vector<Mat> nirChannels(3);
	vector<Mat> nirChannels_undist(3);
	split(imgNIR, nirChannels);
	//undistort and also switch channels while we're at it...
	undistort(nirChannels[0], nirChannels_undist[2], cam_NIR_970, distCoeff_NIR_970);
	undistort(nirChannels[1], nirChannels_undist[1], cam_NIR_1300, distCoeff_NIR_1300);
	undistort(nirChannels[2], nirChannels_undist[0], cam_NIR_1550, distCoeff_NIR_1550);
	merge(nirChannels_undist, imgNIR_undist);
	return imgNIR_undist;
}

Mat ImagePreprocessor::registerImageByHorizontalShift(Mat img, vector<KeyPoint> k1, vector<KeyPoint> k2)
{
	//compute offset as half of median distance between matched points for horizontal shift
	QList<int> distances;
	for(uint i = 0; i < k1.size(); ++i)
	{
		Point2f p1 = k1[i].pt;
		Point2f p2 = k2[i].pt;
		distances.append( abs((int)p1.x - (int)p2.x) );
	}

	//make median
	qSort(distances);
	int xoffset;
	if(k1.size() % 2 == 0) //even
	{
		xoffset = (int)( (float)( distances[k1.size()/2] + distances[k1.size()/2-1] ) / 2.0 + 0.5);
	}
	else //uneven
	{
		xoffset = distances[k1.size()/2];
	}

	//compute average offset and print
	xoffsetSUM += xoffset;
	xoffsetCnt++;
	QString logStr = "RGB-NIR x-offset: sum: " + QString::number(xoffsetSUM) + "   imgs: " + QString::number(xoffsetCnt) + "   avg.: " + QString::number(xoffsetSUM / (double)xoffsetCnt);
	qDebug() << logStr;


	//shift image to the right and return
	return shiftImageToTheRight(img, xoffset);
}

Mat ImagePreprocessor::shiftImageToTheRight(Mat img, int xoffset)
{

	Mat imgShifted(img.rows, img.cols, img.type(), Scalar(0));
	img( Rect(0, 0, img.cols-xoffset, img.rows)).copyTo(imgShifted( Rect(xoffset, 0, img.cols-xoffset, img.rows) ));

	//define crop rectangle based on offset
	finalCropRect_byRGB = Rect(xoffset, 0, img.cols-xoffset, img.rows);
	return imgShifted;
}

Point ImagePreprocessor::warpOnePoint(Mat transfMat, Point p)
{
	Point p_;
	Mat M = transfMat;
	double X0 = M.at<double>(0,0) * p.x + M.at<double>(0,1) * p.y + M.at<double>(0,2);
	double Y0 = M.at<double>(1,0) * p.x + M.at<double>(1,1) * p.y + M.at<double>(1,2);
	double W = M.at<double>(2,0) * p.x + M.at<double>(2,1) * p.y + M.at<double>(2,2);
	W = W != 0.0f ? 1.f / W : 0.0f;
	p_.x = X0 * W;
	p_.y = Y0 * W;
	return p_;
}

Rect ImagePreprocessor::makeMinimalCrop(Rect r1, Rect r2)
{
	Rect r;
	int r1xx = r1.x + r1.width;
	int r2xx = r2.x + r2.width;
	int r1yy = r1.y + r1.height;
	int r2yy = r2.y + r2.height;

	r.x = r1.x > r2.x ? r1.x : r2.x;
	r.y = r1.y > r2.y ? r1.y : r2.y;
	int xx = r1xx < r2xx ? r1xx : r2xx;
	int yy = r1yy < r2yy ? r1yy : r2yy;
	r.width = xx - r.x;
	r.height = yy - r.y;
	return r;
}

Rect ImagePreprocessor::makeMinimalCrop(Point p1, Point p2, Point p3, Point p4, Mat& refImg)
{
	//make sure the points don't lie outside of the image boundaries
	assertPointInsideImage(p1, refImg);
	assertPointInsideImage(p2, refImg);
	assertPointInsideImage(p3, refImg);
	assertPointInsideImage(p4, refImg);

	//the two middle x- and y-coordinates define the minimal crop rectangle
	QList<int> xList = QList<int>() << p1.x << p2.x << p3.x << p4.x;
	QList<int> yList = QList<int>() << p1.y << p2.y << p3.y << p4.y;
	qSort(xList);
	qSort(yList);

	return Rect(xList[1], yList[1], xList[2]-xList[1], yList[2]-yList[1]);
}

void ImagePreprocessor::assertPointInsideImage(Point &p, Mat &refImg)
{
	p.x = p.x < 0 ? 0 : p.x;
	p.x = p.x > refImg.cols ? refImg.cols : p.x;
	p.y = p.y < 0 ? 0 : p.y;
	p.y = p.y > refImg.rows ? refImg.rows : p.y;
}

Mat ImagePreprocessor::cropImage(Mat img, Rect cropROI)
{
	Mat cropRef(img, cropROI);
	Mat cropCopy;
	cropRef.copyTo(cropCopy);
	return cropCopy;
}

Mat ImagePreprocessor::convertKinectDepthTo8Bit(Mat kinectDepth)
{
	Mat out(kinectDepth.size(), CV_8UC1);

	MatIterator_<ushort> it, end;
	MatIterator_<uchar> it8bit;
	for( it = kinectDepth.begin<ushort>(), end = kinectDepth.end<ushort>(), it8bit = out.begin<uchar>();
		 it != end; ++it, ++it8bit)
	{
		float mm = raw_depth_to_mm(*it);
		//throw away values closer than 80 mm
		mm = mm < 800 ? 0 : mm;
		*it8bit = mm / 10000.0 * 255;
	}

	return out;
}

float ImagePreprocessor::raw_depth_to_mm(int raw_depth)
{
  if (raw_depth < 2046)
  {
   return 1.0 / (raw_depth * -0.0030711016 + 3.3309495161) * 1000.0;
  }
  return 0;
}

Mat ImagePreprocessor::mapKinectDepth2NIR(Mat depth_kinect, Mat &NIR_img)
{
	//convert depth to 8 bit if necessary
	Mat depth_8bit;
	if(depth_kinect.type() == CV_16UC1)
	{
//		depth_kinect.convertTo(depth8bit, CV_8UC1, 255.0/2047.0);
		depth_8bit = convertKinectDepthTo8Bit(depth_kinect);
	}
	else{ depth_8bit = depth_kinect; }
    imwrite("depthReprojOrig.jpg", depth_8bit);

//	Helper::debugImage(depth_8bit);

	//invert colors so depth encoding is similar to disparity map
	Mat depth_inverted = depth_8bit;
//	Mat white(depth_kinect.size(), CV_8UC1, Scalar(255));
//	subtract(white, depth_8bit, depth_inverted);

	//fill holes in depth map by simple "shadow"-assumption
	Mat depth_fixed = fixHolesInDepthMap(depth_inverted, 1); //fix from right-to-left, i.e. shadows on right side of objects
	depth_fixed = fixHolesInDepthMap(depth_fixed, 0); //alos got left-to-right to fix some other holes caused by specular surfaces

//	Helper::debugImage(depth_fixed);
    imwrite("depthReprojfixed.jpg", depth_fixed);

	//undistort using intrinsics of depth camera
	Mat depth_kinect_undist;
	undistort(depth_fixed, depth_kinect_undist, cam_IR, distCoeff_IR);

	//map from depth map to 3d space
	vector<Point3f> depth3D = projectKinectDepthTo3DSpace(depth_kinect_undist);

	//mape from 3d space to 2d image in coordinate system of NIR camera
	double verticalShift = proj_IR2NIR.at<double>(1,3) / proj_IR2NIR.at<double>(0,0);
	double horizontalShift = proj_IR2NIR.at<double>(0,3) / proj_IR2NIR.at<double>(0,0);
	Mat depth2D = projectFrom3DSpaceToImage(depth3D, rotation_IR2NIR, transl_IR2NIR, cam_NIR_all, distCoeff_NIR_all,
											rectifyMapX_NIR, rectifyMapY_NIR,
											Size(NIR_img.cols, NIR_img.rows), horizontalShift, verticalShift,
											depth_kinect_undist);
//	Helper::debugImage(depth2D);
    imwrite("depthReproj3D2D.jpg", depth2D);


	//again fill holes in the resulting reprojected depth map, caused by reprojection
//	Mat depth2D_fixed = fixHolesInDepthMap(depth2D, 2); //fix top-down, i.e. shadows on upper side of objects
	Mat depth2D_fixed = fixHolesInDepthMap(depth2D, 3); //fix bottom-up, i.e. shadows on lower side of objects

//	Helper::debugImage(depth2D_fixed);
    imwrite("depthReproj3D2Dfixed.jpg", depth2D_fixed);

	//normalize image if flag is set
	if(normalizeDepth)
	{
		Mat depth2D_normed;
		double min, max;
		minMaxLoc(depth2D_fixed, &min, &max);
		int range = max - min;
		depth2D_fixed.convertTo(depth2D_normed, CV_8UC1, 255.0 / range, -min);
		return depth2D_normed;
	}

	return depth2D_fixed;

	//fill gaps in re-mapped depth map with crossbilateral filter
//	Mat NIR_gray, NIR_gray2;
//	cvtColor(NIR_img, NIR_gray, CV_RGB2GRAY);
//	undistort(NIR_gray, NIR_gray2, cam_NIR, distCoeff_NIR);
//	equalizeHist(NIR_gray2, NIR_gray);
//	Mat depth_refined = crossbilatFilter.filter(depth2D_fixed, NIR_gray, 4.0, 1.0);
//	Helper::debugImage(depth_refined);

//	return depth_refined;
}

Mat ImagePreprocessor::fixHolesInDepthMap(Mat depth, int direction, bool avoidBorders) //0 = L-R, 1 = R-L, 2 = Top-Down, 3 = Bottom-Up
{
	Mat depth_, depthBlur;

	//use "small-kernel" median filter against salt'n'pepper
	medianBlur(depth, depth, 3);
	//use "big-kernel" median filter values on black pixels only
	medianBlur(depth, depthBlur, 11);
	MatIterator_<uchar> it, itBlur, end, endBlur;
	for(it = depth.begin<uchar>(), itBlur = depthBlur.begin<uchar>(),
		end = depth.end<uchar>(), endBlur = depthBlur.end<uchar>();
		it != end; it++, itBlur++)
	{
		//if black pixel in original, use median blurred value
		*it = *it == 0 ? *it = *itBlur : *it;
	}

//	Helper::debugImage(depth);

	//define the direction of the depth-shadow fix
	if(direction == 0)//default: shawdows on left side of objects
	{
		depth_ = depth.clone();
	}
	else if(direction == 1) // shadows on right side of objects
	{
		flip(depth, depth_, 1); //flip vertically
	}
	else if(direction == 2) //shadows on upper side of objects
	{
		transpose(depth, depth_); //transpose
	}
	else if(direction == 3) //shadows on lower side of objects
	{
		transpose(depth, depth_); //transpose and flip vertically
		flip(depth_, depth_, 1);
	}

	for (int y = 0; y < depth_.rows; ++y)
	{
		uchar lastVal = 0; //init with 0 so black border around reprojected image won't be changed
		for (int x = 0; x < depth_.cols; ++x)
		{
			//if pixel value is black (0 = unknown depth), fill with last value in row
			uchar val = depth_.at<uchar>(y,x);
			if(val <= 0)
			{
				bool isBorder = false;

				//quick fix: also check if we're not targetting a black border caused by reprojection
				//do this by checking if the next n pixels are also black
				if(avoidBorders)
				{
					isBorder = true;
					for(int i = 0; i < 40; ++i)
					{
						if(depth_.at<uchar>(y,x+i) != 0) //if at least one of the next pixels isnt't black, it's not a border
						{
							isBorder = false;
							break;
						}
					}
				}

				if(!isBorder){ depth_.at<uchar>(y,x) = lastVal; }
			}
			else { lastVal = val; }
		}
	}
	if(direction == 1){ flip(depth_, depth_, 1); } //flip back
	else if(direction == 2){ transpose(depth_, depth_); } //transpose back
	else if(direction == 3){ flip(depth_, depth_, 1); transpose(depth_, depth_); } //transpose and flip back

//	Helper::debugImage(depth_);

	return depth_;
}

vector<Point3f> ImagePreprocessor::projectKinectDepthTo3DSpace(Mat depth)
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
			uchar d = depth.at<uchar>(y,x);
			coord3D.x = (x - cx) * d / fx;
			coord3D.y = (y - cy) * d / fy;
			coord3D.z = d;
			points3D[cnt++] = coord3D;
		}
	}
	return points3D;
}

Mat ImagePreprocessor::projectFrom3DSpaceToImage(std::vector<Point3f> points3D, Mat rot, Mat transl, Mat cam_Matrix, Mat distCoeff,
												 Mat rectifyMapX, Mat rectifyMapY,
												 Size outImgSz, double shiftX, double shiftY, Mat& refImg)
{
//    qDebug() << IO::getOpenCVTypeName(img3D.type());

	vector<Point2f> points2D;
	projectPoints(points3D, rot, transl, cam_Matrix, noArray(), points2D);

	//DEBUG//
	//test to see if projectPoints() does anything different from Burrus' method [http://nicolas.burrus.name/index.php/Research/KinectCalibration]
	//turns out it does exactly the same thing. reprojection errror is elsewhere.
//	float fx = cam_Matrix.at<double>(0,0);
//	float cx = cam_Matrix.at<double>(0,2);
//	float fy = cam_Matrix.at<double>(1,1);
//	float cy = cam_Matrix.at<double>(1,2);
//	for (uint j = 0; j < points3D.size(); ++j)
//	{
//		//project using camera intrinsics
//		Mat p3d = Mat(points3D[j]);
//		p3d.convertTo(p3d, rot.type()); //matrices have to be same type for matrix multiplication
//		Mat m3d_ =  rot * p3d + transl ;
//		const double *d = m3d_.ptr<double>(0);
//		Point3f p3d_((float)d[0], (float)d[1], (float)d[2]);
//		Point2f p2d;
//		p2d.x = ( (p3d_.x + shiftX) * fx / p3d_.z) + cx;
//		p2d.y = ( (p3d_.y + shiftY) * fy / p3d_.z) + cy;
//		points2D.push_back(p2d);
//	}

	//remember reprojected coordinates of corner points of original depth map
	int i1 = 0;
	int i2 = refImg.cols-1;
	int i3 = refImg.cols * (refImg.rows - 1);
	int i4 = refImg.cols * refImg.rows - 1;
	Point p1((int)points2D[i1].x, (int)points2D[i1].y); //indices for corner points
	Point p2((int)points2D[i2].x, (int)points2D[i2].y);
	Point p3((int)points2D[i3].x, (int)points2D[i3].y);
	Point p4((int)points2D[i4].x, (int)points2D[i4].y);

	//transform vector holding 2D points into image matrix with depth values as intensity
	Mat img2D(outImgSz.height, outImgSz.width, CV_8UC1, Scalar(0)); //white = farest OR unkown depth
	for (uint i = 0; i < points2D.size(); ++i)
	{
		int x = points2D[i].x;
		int y = points2D[i].y;
		int val = points3D[i].z;
		img2D.at<uchar>(y,x) = val;
	}

	//rectify image without interpolation, while doing that get rectified corner points
	Mat img2D_rect(img2D.size(), img2D.type());
	Point p1_rect(0, 0);
	Point p2_rect(img2D_rect.cols-1, 0);
	Point p3_rect(0, img2D_rect.rows-1);
	Point p4_rect(img2D_rect.cols-1, img2D_rect.rows-1);

	for (int y = 0; y < img2D.rows; ++y)
	{
		for (int x = 0; x < img2D.cols; ++x)
		{
			//for each pixel in rect. img get value from source image
			int yy = rectifyMapY.at<float>(y,x);
			int xx = rectifyMapX.at<float>(y,x);
			img2D_rect.at<uchar>(y,x) = img2D.at<uchar>(yy, xx);

			//if px in src img is corner point, remember position in rect. img
			if(p1.x == xx && p1.y == yy){ p1_rect.x = x; p1_rect.y = y; }
			if(p2.x == xx && p2.y == yy){ p2_rect.x = x; p2_rect.y = y; }
			if(p3.x == xx && p3.y == yy){ p3_rect.x = x; p3_rect.y = y; }
			if(p4.x == xx && p4.y == yy){ p4_rect.x = x; p4_rect.y = y; }
		}
	}

	//define minimal crop rectangle for kinect depth
	finalCropRect_byKinectDepth = makeMinimalCrop(p1_rect, p2_rect, p3_rect, p4_rect, img2D_rect);
	//assure that the rectangle respects original aspect ratio
	finalCropRect_byKinectDepth.height = finalCropRect_byKinectDepth.width * ((double)refImg.rows / (double)refImg.cols);

	return img2D_rect;
}

Mat ImagePreprocessor::registerRGB2NIR(Mat& RGB_img, Mat& NIR_img)
{
	Mat RGB_registered;

	if(makeCSStereo) //i.e. if HOG descriptors have been computed, we can use them for registration
	{
		vector<KeyPoint> matchedDescrL, matchedDescrR;
		vector<DMatch> dmatches;
		((HOG_crossSpectralStereoMatcher*)(CrossSpectralStereoMatcher*)CSstereoMatcher)->getBestDescriptors(matchedDescrL, matchedDescrR, dmatches, 4.0);

		vector<Point2f> pL, pR;
		for (int i = 0; i < matchedDescrL.size(); ++i)
		{
			pL.push_back(matchedDescrL[i].pt);
			pR.push_back(matchedDescrR[i].pt);
		}

		if(RGBregist_thinPlateSpline)
		{
			//register RGB using thin plate spline algorithm
			vector<Point> pLs, pRs;
			for (int i = 0; i < matchedDescrL.size(); ++i)
			{
				Point2f pL = matchedDescrL[i].pt;
				Point2f pR = matchedDescrR[i].pt;
				pLs.push_back(Point(pL.x, pL.y));
				pRs.push_back(Point(pR.x, pL.y));
			}
			CThinPlateSpline tps(pRs, pLs);
			tps.warpImage(RGB_img, RGB_registered);
		}
		else if(RGBregist_distortPerspective)
		{
			//register RGB to NIR using homography matrix computed from best HOG descriptor matches (uniformly spaced)
			Mat homography = findHomography(pR, pL, CV_RANSAC);
			warpPerspective(RGB_img, RGB_registered, homography, RGB_img.size());

			//also use homography matrix to get the 4 corner points of the warped image
			Point p1 = warpOnePoint(homography, Point(0,0));
			Point p2 = warpOnePoint(homography, Point(RGB_img.cols-1, 0));
			Point p3 = warpOnePoint(homography, Point(0,RGB_img.rows-1));
			Point p4 = warpOnePoint(homography, Point(RGB_img.cols-1, RGB_img.rows-1));

			//use these corner points to define the minimal crop rectangle
			finalCropRect_byRGB = makeMinimalCrop(p1, p2, p3, p4, RGB_img);
		}
		else //default
		{
			//register RGB to NIR using simple horizonzal shift
			RGB_registered = registerImageByHorizontalShift(RGB_img, matchedDescrL, matchedDescrR);
		}

		//DEBUG show matches//
		Mat matches;
		drawMatches(NIR_img, matchedDescrL, RGB_img, matchedDescrR, dmatches, matches);
//		imshow("descriptor matches", matches);
		imwrite("HOG_descriptors_matches.png", matches);
		//DEBUG show matches//

	}
	else //i.e. if HOG descriptors haven't been computed, then shift by pre-computed value
	{
		//register with horizontal shift using previously computed offset value in x direction
		RGB_registered = shiftImageToTheRight(RGB_img, 129);
	}

    imwrite("RGB_registered.png", RGB_registered);
	return RGB_registered;
}

void ImagePreprocessor::makeCrossSpectralStereo(Mat imgNIR_L, Mat imgRGB_R, Mat& out_disp)
{
//    Mat rgb_gray; //rgb to grayscale
//    cvtColor(imgRGB_R, rgb_gray, CV_BGR2GRAY);

	//get lowerst nir channel, 970 nm
	Mat nir_970(imgNIR_L.rows, imgNIR_L.cols, CV_8UC1);
	int from_to[] = {0, 0}; //970 nm is first channel
	mixChannels(&imgNIR_L, 1, &nir_970, 1, from_to, 1);

//    CSstereoMatcher->process(nir_970, rgb_gray, out_disp);

	//get red RGB channel (closest to NIR)
	Mat rgb_red(imgRGB_R.rows, imgRGB_R.cols, CV_8UC1);
	int from_to2[] = {2, 0}; //red is last channel (BGR format)
	mixChannels(&imgRGB_R, 1, &rgb_red, 1, from_to2, 1);


	//get left-to-right disparity
	CSstereoMatcher->process(nir_970, rgb_red, out_disp);

	//LR-CONSISTENCY CHECKING IS ALREADY DONE IN SGM, SO I DONT NEED TO DO THIS HERE...
}

Mat ImagePreprocessor::whiteBalance(Mat img)
{
	vector<Mat> ch(img.channels());
	split(img, ch);

	vector<Mat> ch2;
	foreach(Mat m, ch)
	{
		double min, max;
		minMaxLoc(m, &min, &max);
		Mat res = (m - min) * (255 / (max-min) );
		ch2.push_back(res);
	}

	Mat out;
	merge(ch2, out);
	return out;
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


QList<Mat> ImagePreprocessor::readImgs2List(QStringList imgNames, bool color)
{
	QProgressDialog progress("Loading images", "Cancel Load", 0, imgNames.size(), parent);
	progress.setMinimumWidth(300);
	progress.setMinimumDuration(100);
	progress.setValue(0);

	int colorMode;
	if(color){ colorMode = IMREAD_COLOR; }
	else{ colorMode = IMREAD_GRAYSCALE; }

	int cnt = 0;
	QList<Mat> imgs;
	foreach (QString s, imgNames)
	{
		imgs.append(imread(s.toStdString(), colorMode));

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
	fs << "cam_NIR" << cam_NIR_all;
	fs << "cam_NIR_970" << cam_NIR_970;
	fs << "cam_NIR_1300" << cam_NIR_1300;
	fs << "cam_NIR_1550" << cam_NIR_1550;
	fs << "cam_IR" << cam_IR;

	fs << "distCoeff_RGB" << distCoeff_RGB;
	fs << "distCoeff_RGB_resized" << distCoeff_RGB_resized;
	fs << "distCoeff_NIR" << distCoeff_NIR_all;
	fs << "distCoeff_NIR_970" << distCoeff_NIR_970;
	fs << "distCoeff_NIR_1300" << distCoeff_NIR_1300;
	fs << "distCoeff_NIR_1550" << distCoeff_NIR_1550;
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
	fs << "rectifMapX_IR" << rectifyMapX_IR;
	fs << "rectifMapY_IR" << rectifyMapY_IR;

	fs << "rotation_NIR2RGB" << rotation_RGB2NIR;
	fs << "rotation_IR2RGB" << rotation_IR2NIR;
	fs << "transl_NIR2RGB" << transl_RGB2NIR;
	fs << "transl_IR2RGB" << transl_IR2NIR;
	fs << "proj_IR2NIR" << proj_IR2NIR;

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
	fs["cam_NIR"] >> cam_NIR_all;
	fs["cam_NIR_970"] >> cam_NIR_970;
	fs["cam_NIR_1300"] >> cam_NIR_1300;
	fs["cam_NIR_1550"] >> cam_NIR_1550;
	fs["cam_IR"] >> cam_IR;

	fs["distCoeff_RGB"] >> distCoeff_RGB;
	fs["distCoeff_RGB_resized"] >> distCoeff_RGB_resized;
	fs["distCoeff_NIR"] >> distCoeff_NIR_all;
	fs["distCoeff_NIR_970"] >> distCoeff_NIR_970;
	fs["distCoeff_NIR_1300"] >> distCoeff_NIR_1300;
	fs["distCoeff_NIR_1550"] >> distCoeff_NIR_1550;
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
	fs["rectifMapX_IR"] >> rectifyMapX_IR;
	fs["rectifMapY_IR"] >> rectifyMapY_IR;

	fs["rotation_NIR2RGB"] >> rotation_RGB2NIR;
	fs["rotation_IR2RGB"] >> rotation_IR2NIR;
	fs["transl_NIR2RGB"] >> transl_RGB2NIR;
	fs["transl_IR2RGB"] >> transl_IR2NIR;
	fs["proj_IR2NIR"] >> proj_IR2NIR;

	fs["cams_are_calibrated_"] >> cams_are_calibrated_;
	fs["rig_is_calibrated_"] >> rig_is_calibrated_;

	fs.release();

	//also save URL of last loaded parameter file
	FileStorage fs2("config/config.txt", FileStorage::WRITE);
	fs2 << "lastCamParamURL" << loadURL.toStdString();
	fs2.release();

	makeMsg("Success!", "Calibration file \"" + loadURL.split("/").last() + "\" loaded");
}


Mat ImagePreprocessor::makeBrightGrayscale(Mat &img)
{
	//doesnt really work
	Mat img_;

	//check the brightes pixel value on all channels
	if(img.channels() == 3)
	{
		vector<Mat> chs(3);
		cvtColor(img, img_, CV_BGR2HSV);
		split(img_, chs);

//		equalizeHist(chs.at(0), chs.at(0));
//		equalizeHist(chs.at(1), chs.at(1));
		equalizeHist(chs.at(2), chs.at(2));
		img_ = chs.at(2).clone();
	}
	else if(img.channels() == 1)
	{
		equalizeHist(img, img_);
	}

	return img_.clone();
}

