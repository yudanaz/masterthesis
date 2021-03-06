#include "cameracalibration.h"

CameraCalibration::CameraCalibration(QWidget *parent):
	cameraCalibrated(false),
	stereoCalibrated(false),
	rectifyMapsCreated(false),
	homogeneityCalibrated(false),
	nrOfNIRChannels(4), //4 NIR channels max right now (will change with new flashlight)
	camMatrices_goldeye(4),
	distCoeffs_goldeye(4),
	RGB2NIR_resizeFactor(0),
	RGB2NIR_fittingComputed(false)
{
	parentWidget = parent;
}

void CameraCalibration::calibrateSingleCamera(QList<Mat> calibImgs, int chessboard_width, int chessboard_height, QString calibFileName)
{
	calibrateCamFromImages(calibImgs, 0, chessboard_width, chessboard_height);
	QMessageBox::information(parentWidget, "Calibration successful", "Camera has been calibrated", QMessageBox::Ok);
	cameraCalibrated = true;
	goldeyeCalibrated = false;
	stereoCalibrated = false;

	if(calibFileName != ""){ saveCalibrationFile(calibFileName, 0); }
}

void CameraCalibration::calibrateGoldeyeMultiChannel(QList<QList<Mat> > calibImgPacks, int chessboard_width, int chessboard_height, QString calibFileName)
{
	//open each tar file and get band images
	QList<Mat> b935, b1060, b1300, b1550;

	foreach(QList<Mat> imgs, calibImgPacks)
	{
		b935.append(imgs.at(0));
		b1060.append(imgs.at(1));
		b1300.append(imgs.at(2));
		b1550.append(imgs.at(3));
	}

	//make calibration for each waveband
	calibrateCamFromImages(b935, 0, chessboard_width, chessboard_height, true);
	calibrateCamFromImages(b1060, 1, chessboard_width, chessboard_height, true);
	calibrateCamFromImages(b1300, 2, chessboard_width, chessboard_height, true);
	calibrateCamFromImages(b1550, 3, chessboard_width, chessboard_height, true);

	//save calibration matrix
	if(calibFileName != "")
	{
		saveCalibrationFile(calibFileName + "_935", 0);
		saveCalibrationFile(calibFileName + "_1060", 1);
		saveCalibrationFile(calibFileName + "_1300", 2);
		saveCalibrationFile(calibFileName + "_1550", 3);
	}

	goldeyeCalibrated = true;
	cameraCalibrated = false;
	stereoCalibrated = false;
}

void CameraCalibration::calibrateStereoCameras(QList<Mat> calibImgsLeft, QList<Mat> calibImgsRight,
											   int chessboard_width, int chessboard_height, QString calibFileName)
{
	doStereoCalibration(calibImgsLeft, calibImgsRight, chessboard_width, chessboard_height, calibFileName);

	//save the calibration file
	if(calibFileName != ""){ saveStereoCalibrationFile(calibFileName, false); }

	QMessageBox::information(parentWidget, "Calibration successful", "Stereo Camera Pair has been calibrated", QMessageBox::Ok);
	goldeyeCalibrated = false;
	cameraCalibrated = false;
	stereoCalibrated = true;
}

void CameraCalibration::calibrateRGBNIRStereoCameras(QList<Mat> calibImgsNIR_L, QList<Mat> calibImgsRGB_R, int chessboard_width, int chessboard_height, QString calibFileName)
{
	//resize and crop RGB images to fit NIR images:
	QList<Mat> calibImgsRGB_R_fitted = fitRGBimgs2NIRimgs(calibImgsNIR_L, calibImgsRGB_R, chessboard_width, chessboard_height);

	//calibrate stereo images
	doStereoCalibration(calibImgsNIR_L, calibImgsRGB_R_fitted, chessboard_width, chessboard_height, calibFileName);

	//save the calibration file
	if(calibFileName != ""){ saveStereoCalibrationFile(calibFileName, true); }

	QMessageBox::information(parentWidget, "Calibration successful", "RGN & NIR Stereo Pair has been calibrated", QMessageBox::Ok);
	goldeyeCalibrated = false;
	cameraCalibrated = false;
	stereoCalibrated = true;
}

void CameraCalibration::doStereoCalibration(QList<Mat> calibImgsLeft, QList<Mat> calibImgsRight,
											int chessboard_width, int chessboard_height, QString calibFileName)
{
	Size imgSize = calibImgsLeft.first().size();

	//get object and image points
	vector<Point3f> obj;
	for (int j = 0; j < chessboard_width * chessboard_height; j++)
	{
		obj.push_back(Point3f(j / chessboard_width, j % chessboard_width, 0.0f));
	}
	vector<vector<Point3f> > objectPoints_L;
	vector<vector<Point3f> > objectPoints_R;
	vector<vector<Point2f> > imagePoints_L;
	vector<vector<Point2f> > imagePoints_R;

	getObjectAndImagePoints(calibImgsLeft, chessboard_width, chessboard_height, obj, objectPoints_L, imagePoints_L);
	getObjectAndImagePoints(calibImgsRight, chessboard_width, chessboard_height, obj, objectPoints_R, imagePoints_R);

	//check wether all points have been found for left and right images
	if(imagePoints_L.size() != imagePoints_R.size())
	{
		QMessageBox::information(parentWidget, "Error", "Couldnt find all chessboard corners", QMessageBox::Ok);
		return;
	}

	//start stereo calibration
	Mat CM_L = Mat(3, 3, CV_64FC1);
	Mat CM_R = Mat(3, 3, CV_64FC1);
	Mat D_L, D_R;
	Mat rotMat, translVec, E, F;

	stereoCalibrate(objectPoints_L, imagePoints_L, imagePoints_R,
					CM_L, D_L, CM_R, D_R, imgSize, rotMat, translVec, E, F,
					cvTermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 100, 1e-5),
					CV_CALIB_SAME_FOCAL_LENGTH | CV_CALIB_ZERO_TANGENT_DIST);

	//start stereo rectification and trigger making the rectify maps (in undist and remap method)
	Mat R1, R2, P1, P2, Q;
	stereoRectify(CM_L, D_L, CM_R, D_R, imgSize, rotMat, translVec, R1, R2, P1, P2, Q);
	rectifyMapsCreated = false;

	//store everythin in class members for later use
	camMatrices_goldeye[0] = CM_L.clone();
	camMatrices_goldeye[1] = CM_R.clone();
	distCoeffs_goldeye[0] = D_L.clone();
	distCoeffs_goldeye[1] = D_R.clone();
	rotationMatrix = rotMat.clone();
	translationVec = translVec.clone();
	essentialMat = E.clone();
	fundamentalMat = F.clone();
	rectificTransf_L = R1.clone();
	rectificTransf_R = R2.clone();
	projectionMat_L = P1.clone();
	projectionMat_R = P2.clone();
	disparity2DepthMat = Q.clone();
}

Mat CameraCalibration::resizeAndCropRGBImg(Mat rgbImg)
{
	Mat resized, cropped;
	if(!RGB2NIR_fittingComputed){ return cropped; } //return empty
	resize(rgbImg, resized, Size(), RGB2NIR_resizeFactor, RGB2NIR_resizeFactor, INTER_CUBIC);
	resized(RGB2NIR_cropRect).copyTo(cropped);
	return cropped;
}

QList<Mat> CameraCalibration::fitRGBimgs2NIRimgs(QList<Mat> origNirs, QList<Mat> origRGBs,
												 int chessboard_width, int chessboard_height)
{
	QList<Mat> resizedRGBs, fittedRGBs;

	// 1) find chessboard corners
	if(origNirs.count() != origRGBs.count()){ return fittedRGBs; } //amount of both lists must be equal
	vector<vector<Point2f> > cornersNIR_L;
	vector<vector<Point2f> > cornersRGB_R;
	Size size = Size(chessboard_width, chessboard_height);

	for (int i = 0; i < origNirs.count(); ++i)
	{
		Mat imgGrayL, imgGrayR;
		vector<Point2f> cornersL, cornersR;

		//NIR, left:
		cvtColor(origNirs.at(i), imgGrayL, CV_BGR2GRAY); //must be gray scale
		bool success = findChessboardCorners(imgGrayL, size, cornersL, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
		if(success){ cornersNIR_L.push_back(cornersL); }

		//RGB, right:
		cvtColor(origRGBs.at(i), imgGrayR, CV_BGR2GRAY); //must be gray scale
		success = findChessboardCorners(imgGrayR, size, cornersR, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
		if(success){ cornersRGB_R.push_back(cornersR); }
	}
	//check wether all points have been found for left and right images
	if(cornersRGB_R.size() != cornersNIR_L.size())
	{
		QMessageBox::information(parentWidget, "Error", "Couldnt find all chessboard corners", QMessageBox::Ok);
		return fittedRGBs;
	}


	// 2) get average resize factor by comparing corner distances for NIR and RGB images
	int nrOfImages = cornersNIR_L.size();
	int nrOfCorners = cornersNIR_L[0].size();
	double avgResizeFactor = 0.0;
	for(int i = 0; i < nrOfImages; ++i)
	{
		//get distances between chessboard corners for each image type (NIR and RGB)
		//and compute the relationship / resize factor
		for (int j = 0; j < nrOfCorners-1; ++j)
		{
			Point2f diff_NIR = cornersNIR_L[i][j] - cornersNIR_L[i][j+1];
			double euclDist_NIR = sqrt(diff_NIR.x * diff_NIR.x + diff_NIR.y * diff_NIR.y);

			Point2f diff_RGB = cornersRGB_R[i][j] - cornersRGB_R[i][j+1];
			double euclDist_RGB = sqrt(diff_RGB.x * diff_RGB.x + diff_RGB.y * diff_RGB.y);

			double resizeFactor = euclDist_NIR / euclDist_RGB;
			avgResizeFactor += resizeFactor;
//            qDebug() << "resize factor: " << resizeFactor;
//            qDebug() << "NIR: " << euclDist_NIR << "   RGB: " << euclDist_RGB;
		}
	}
	//get average
	avgResizeFactor /= (nrOfImages * nrOfCorners);
	qDebug() << "average resize factor: " << avgResizeFactor;

	// 3) resize the RGB images
	foreach(Mat rgbImg, origRGBs)
	{
		Mat resized;
		resize(rgbImg, resized, Size(), avgResizeFactor, avgResizeFactor, INTER_CUBIC);
		resizedRGBs.append(resized);
//        imshow("orig", rgbImg);
//        imshow("resized", resized);
	}

	// 4) crop the rgb images
	int nirw = origNirs.at(0).cols;
	int nirh = origNirs.at(0).rows;
	int rgbw = resizedRGBs.at(0).cols;
	int rgbh = resizedRGBs.at(0).rows;
	Rect cropRect( (rgbw-nirw)/2, (rgbh-nirh)/2, nirw, nirh );

	foreach(Mat rgbImg, resizedRGBs)
	{
		Mat cropped;
		rgbImg(cropRect).copyTo(cropped);
		fittedRGBs.append(cropped);
//        imshow("cropped", cropped);
	}

	RGB2NIR_resizeFactor = avgResizeFactor;
	RGB2NIR_cropRect = cropRect;
	RGB2NIR_fittingComputed = true;
	return fittedRGBs;
}

void CameraCalibration::calibrateCamFromImages(QList<Mat> calibImgs, int channelIndex,
											   int chessboard_width, int chessboard_height, //nr of inner corners in row & column
											   bool isGrayScale)
{
	Size imgSize = calibImgs.first().size();
	int nrOfCorners = chessboard_width * chessboard_height;  //total number of inner corners

	//objectPoints should contain physical location of each corners but
	//since we don’t know that so we assign constant positions to all the corners
	vector<Point3f> obj;
	for (int j = 0; j < nrOfCorners; j++)
	{
		obj.push_back(Point3f(j / chessboard_width, j % chessboard_width, 0.0f));
	}

	vector<vector<Point3f> > objectPoints;
	vector<vector<Point2f> > imagePoints;

	getObjectAndImagePoints(calibImgs, chessboard_width, chessboard_height, obj, objectPoints, imagePoints, isGrayScale);


	//calibrate the camera and store matrizes
	Mat camMatrix(3, 3, CV_32FC1);
	camMatrix.at<float>(0, 0) = 1;
	camMatrix.at<float>(1, 1) = 1;
	Mat distCoeff;
	vector<Mat> rvecs, tvecs;

	calibrateCamera(objectPoints, imagePoints, imgSize, camMatrix, distCoeff, rvecs, tvecs);
	camMatrices_goldeye[channelIndex] = camMatrix.clone();
	distCoeffs_goldeye[channelIndex] = distCoeff.clone();
}

void CameraCalibration::getObjectAndImagePoints(QList<Mat> calibImgs, int width, int height,
												vector<Point3f> obj,
												vector<vector<Point3f> >& objectPoints,
												vector<vector<Point2f> >& imagePoints,
												bool isGrayScale)
{
	Size chessboard_sz = Size(width, height);

	//get chessboard corners for all images
	getImagePoints(calibImgs, chessboard_sz, imagePoints, isGrayScale);

	//put as many of the dummy object points in the list as there are image points
	for (int i = 0; i < imagePoints.size(); ++i)
	{
		objectPoints.push_back(obj);
	}
}

void CameraCalibration::getImagePoints(QList<Mat> calibImgs, Size chessboardSize, vector<vector<Point2f> > &imagePoints, bool isGrayScale)
{
	//get chessboard corners for all images
	bool success = false;
	foreach(Mat img, calibImgs)
	{
		vector<Point2f> corners;
		Mat imgGray;
		if(!isGrayScale){ cvtColor(img, imgGray, CV_BGR2GRAY); }
		else{ imgGray = img; }
		success = findChessboardCorners(imgGray, chessboardSize, corners,
										CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);

		if(success)
		{
			imagePoints.push_back(corners);
		}

		//show found corners in image
//		drawChessboardCorners(img, chessboard_sz, corners, success);
//		namedWindow("found corners", CV_WINDOW_KEEPRATIO);
//		imshow("found corners", img);
//		cvWaitKey(2);
	}
}

void CameraCalibration::undistortSingleImage(QString fileName)
{
	Mat img, imgUndist;
	img = imread(fileName.toStdString().c_str());
	undistort(img, imgUndist, camMatrices_goldeye[0], distCoeffs_goldeye[0]);

	namedWindow("original", CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO);
	namedWindow("undistorted", CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO);
	imshow("original", img);
	imshow("undistorted", imgUndist);
}

void CameraCalibration::undistortGoldeyeMultiChImg(QStringList tarFileNames)
{
//	QString basicFileName = QFileDialog::getSaveFileName(parentWidget, "Select file to store original and undistorted channel images",
//														 QDir::homePath(), "*.png");
//	basicFileName.remove(".png");

	QStringList wavebands;
	wavebands << "935" << "1060" << "1300" << "1550";

	//undistort all channels of multichannel image
	foreach(QString tarFileName, tarFileNames)
	{
		QList<Mat> imgs = InOut::getImagesFromTarFile(tarFileName);
		QString newName = tarFileName.remove(".tar");
		for(int i = 0; i< 4/*imgs.length()*/; ++i)
		{
			Mat imgU;
			undistort(imgs.at(i), imgU, camMatrices_goldeye[i], distCoeffs_goldeye[i]);

			//save orig and undistorted for all channels
			QString imgNm = newName + "_" + wavebands.at(i) + "_orig.png";
			QString imgUndistNm = newName + "_" + wavebands.at(i) + "_undist.png";
			imwrite(imgNm.toStdString().c_str(), imgs.at(i));
			imwrite(imgUndistNm.toStdString().c_str(), imgU);
		}
	}

	QMessageBox::information(parentWidget, "Save successful", "Original and undistored waveband images have been saved", QMessageBox::Ok);
}

void CameraCalibration::undistortAndRemapStereoImages(Mat leftImage, Mat rightImage, Mat& leftImgOut, Mat& rightImgOut)
{
	if(!rectifyMapsCreated){ makeRectifyMapsForStereo(leftImage.size(), rightImage.size()); }
	remap(leftImage, leftImgOut, mapLx, mapLy, INTER_LINEAR, BORDER_CONSTANT, Scalar());
	remap(rightImage, rightImgOut, mapRx, mapRy, INTER_LINEAR, BORDER_CONSTANT, Scalar());
}

void CameraCalibration::makeRectifyMapsForStereo(Size leftImgSize, Size rightImgSize)
{
	initUndistortRectifyMap(camMatrices_goldeye[0], distCoeffs_goldeye[0], rectificTransf_L, projectionMat_L,
			leftImgSize, CV_32FC1, mapLx, mapLy);
	initUndistortRectifyMap(camMatrices_goldeye[1], distCoeffs_goldeye[1], rectificTransf_R, projectionMat_R,
			rightImgSize, CV_32FC1, mapRx, mapRy);
	rectifyMapsCreated = true;
}

void CameraCalibration::setDisparityParameters(int minDisparity, int nrOfDisparities, int SADWindowSize,
											   int prefilterSize, int prefilterCap,
											   int textureThresh, float uniquenessRatio,
											   int specklewindowSize, int speckleRange)
{
	//assure that parameters meet requirements
	//(disparaties must be multiple of 16, blocksize must be odd!)
//    int dispTemp = nrOfDisparities % 16;
//    if(dispTemp != 0) { nrOfDisparities -= dispTemp; } //subtract so it's multiple of 16
//    if(SADWindowSize % 2 == 0) { SADWindowSize++; } //make odd if even
//    if(SADWindowSize < 5) { SADWindowSize += 5-SADWindowSize; } //make >= 5
//	if(smoothKernelSize % 2 == 0) { smoothKernelSize++; } //make odd if even

	//pre filter (normalization)
	sbm.state->preFilterType = StereoBM::PREFILTER_NORMALIZED_RESPONSE;
	sbm.state->preFilterSize = prefilterSize;
	sbm.state->preFilterCap = prefilterCap;

	sgbm.preFilterCap = prefilterCap;

	//correspondence via SAD
	sbm.state->minDisparity = minDisparity;
	sbm.state->numberOfDisparities = nrOfDisparities;
	sbm.state->SADWindowSize = SADWindowSize;

	sgbm.minDisparity = minDisparity;
	sgbm.numberOfDisparities = nrOfDisparities;
	int sgbmWindowSize = SADWindowSize > 0 ? SADWindowSize : 3;
	int channels = 1; //because we convert images to grayscale
	sgbm.SADWindowSize = sgbmWindowSize;
	sgbm.P1 = 8 * channels * sgbmWindowSize * sgbmWindowSize;
	sgbm.P2 = 32 * channels * sgbmWindowSize * sgbmWindowSize;
	sgbm.disp12MaxDiff = 1; //Maximum allowed difference (in integer pixel units)
	//in the left-right disparity check. Set it to a non-positive value to disable the check.

	//post filters -> remove bad matches
	sbm.state->textureThreshold = textureThresh;
	sbm.state->uniquenessRatio = uniquenessRatio;
	sbm.state->speckleWindowSize = specklewindowSize;
	sbm.state->speckleRange = speckleRange;

	sgbm.uniquenessRatio = uniquenessRatio;
	sgbm.speckleWindowSize = specklewindowSize;
	sgbm.speckleRange = speckleRange;

	sgbm.fullDP = true;
}

Mat CameraCalibration::makeDisparityMap(Mat leftGrayImg, Mat rightGrayImg, bool useSGBM)
{
	//compute disparities
	Mat disp;//(leftGrayImg.rows, leftGrayImg.cols, CV_16UC1);
	if(useSGBM)
	{
		sgbm(leftGrayImg, rightGrayImg,disp);
	}
	else
	{
		sbm(leftGrayImg, rightGrayImg, disp, CV_16S);
	}

	//get real values (disp values are multiple of 16, see http://docs.opencv.org/modules/calib3d/doc/camera_calibration_and_3d_reconstruction.html#stereosgbm-operator)
	disp = disp / 16;
	return disp;
}

Mat CameraCalibration::voteInSuperpixels(int nrOfSuperPixel, Mat superpixelMap, Mat shortImg, int classAmount)
{
	nrOfSuperPixel += 30; //there seem to be always some numbers that are jumbed, resulting in unused indices

	Mat res (shortImg.rows, shortImg.cols, shortImg.type(), Scalar(0));

	//make 2D-vector to hold sum of disparities and pixel amount for each superpixel
	//(vector type is long in order to hold summed value, although disparity map type is short)
    vector< vector<long> > superpxs(nrOfSuperPixel, vector<long>(classAmount));

	//if superpixel or disparity map not in ushort format, return emtpy
	int type1 = superpixelMap.type();
	int type2 = shortImg.type();
	if( type1 != CV_16UC1 || type2 != CV_16SC1)
	{
		return res;
	}

	//init superpixel vector
	for(int i = 0; i < nrOfSuperPixel; ++i )
	{
        for (int j = 0; j < classAmount; ++j)
        {
            superpxs[i][j] = 0;
        }
	}

	//sum and count all disparity values for each superpixel
	MatIterator_<ushort> it_superpx, end;
	MatIterator_<short> it_disp;
	for( it_superpx = superpixelMap.begin<ushort>(), it_disp = shortImg.begin<short>(),
		 end = superpixelMap.end<ushort>(); it_superpx != end; ++it_superpx, ++it_disp )
	{
        short val = *it_disp;
        if(val < 254)//val > 0)
		{
			ushort temp = *it_superpx;
            superpxs[*it_superpx][val] += 1; //count how many times this class is in this superpixel
//			superpxs[*it_superpx][0] += val; //add disparity value
//			superpxs[*it_superpx][1]++; //count number of pixels in superpixel
		}
	}

    //check which class has most entries for each superpixel
    vector<int> mostEntries(nrOfSuperPixel);
    for (int i = 0; i < nrOfSuperPixel; ++i)
    {
        int max = 0;
        int maxIndex = 255;
        for (int j = 0; j < classAmount; ++j)
        {
            int amount = superpxs[i][j];
            if(amount > max)
            {
                max = amount;
                maxIndex = j;
            }
        }
        mostEntries[i] = maxIndex;
    }

//	//average values for each superpixel
//	for(int i = 0; i < nrOfSuperPixel; ++i )
//	{
//		ushort amountInSuperPx = superpxs[i][1];
//		if(amountInSuperPx > 0)
//		{
//			superpxs[i][0] /= amountInSuperPx;
//		}
//	}

	//write averaged values in output matrix
	MatIterator_<short> it_res;
	for( it_superpx = superpixelMap.begin<ushort>(), it_res = res.begin<short>(),
		 end = superpixelMap.end<ushort>(); it_superpx != end; ++it_superpx, ++it_res )
	{
//		*it_res = superpxs[*it_superpx][0];
        *it_res = mostEntries[*it_superpx];
	}

	return res;
}


Mat CameraCalibration::alignImageByFeatures(Mat imageL, Mat imageRtoBeAligned)
{
	//find features
	vector<KeyPoint> keypointsL, keypointsR;
	Mat descriptorsL, descriptorsR;
	SURF surf(1500);

	surf.detect(imageL, keypointsL);
	surf.detect(imageRtoBeAligned, keypointsR);
	surf.compute(imageL, keypointsL, descriptorsL);
	surf.compute(imageRtoBeAligned, keypointsR, descriptorsR);

	vector< DMatch > matches;
	Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce");
	matcher->match(descriptorsL, descriptorsR, matches);

	//keep only best matches (lowest distance)
	int numBest = 50;
	if(matches.size() > numBest)
	{
		std::nth_element(matches.begin(), matches.begin()+numBest, matches.end()); //sort to numBest's position
		matches.erase(matches.begin()+numBest, matches.end());//remove rest
	}

	Mat matchImg;
	drawMatches(imageL, keypointsL, imageRtoBeAligned, keypointsR, matches, matchImg);
	imshow("Matches", matchImg);

	//filter the matched descriptors
	vector<Point2f> matchedDescriptorsL, matchedDescriptorsR;
	Point2f leftMatch, rightMatch;
	foreach(DMatch match, matches)
	{
		leftMatch = keypointsL[match.queryIdx].pt;
		rightMatch = keypointsR[match.trainIdx].pt;
		matchedDescriptorsL.push_back(leftMatch);
		matchedDescriptorsR.push_back(rightMatch);
	}

	//if no descriptors found, return empty
	if(matchedDescriptorsL.size() == 0 || matchedDescriptorsR.size() == 0){ return Mat(); }

	//warp perspective of right image with homography matrix from descriptors
	Mat homography = findHomography(matchedDescriptorsR, matchedDescriptorsL, CV_RANSAC);
	Mat warpedImg;
	warpPerspective(imageRtoBeAligned, warpedImg, homography, imageRtoBeAligned.size());
	return warpedImg;
}

void CameraCalibration::saveCalibrationFile(QString calibFileName, int channelIndex)
{
	if(calibFileName != "" && channelIndex < nrOfNIRChannels)
	{
		calibFileName = calibFileName.remove(".singlecal").append(".singlecal");
		FileStorage fs(calibFileName.toStdString().c_str(), FileStorage::WRITE);
		fs << "CM" << camMatrices_goldeye[channelIndex];
		fs << "D" << distCoeffs_goldeye[channelIndex];
		fs.release();
	}
}

void CameraCalibration::saveStereoCalibrationFile(QString calibFileName, bool isRGBNIR)
{
	if(calibFileName != "")
	{
		if(isRGBNIR)
		{
			calibFileName = calibFileName.remove(".rgbnirstcal").append(".rgbnirstcal");
		}
		else
		{
			calibFileName = calibFileName.remove(".stereocal").append(".stereocal");
		}


		FileStorage fs(calibFileName.toStdString().c_str(), FileStorage::WRITE);
		fs << "CM_L" << camMatrices_goldeye[0];
		fs << "CM_R" << camMatrices_goldeye[1];
		fs << "D_L" << distCoeffs_goldeye[0];
		fs << "D_R" << distCoeffs_goldeye[1];
		fs << "R" << rotationMatrix;
		fs << "T" << translationVec;
		fs << "E" << essentialMat;
		fs << "F" << fundamentalMat;
		fs << "R_L" << rectificTransf_L;
		fs << "R_R" << rectificTransf_R;
		fs << "P_L" << projectionMat_L;
		fs << "P_R" << projectionMat_R;
		fs << "Q" << disparity2DepthMat;
		if(isRGBNIR)
		{
			fs << "FAC" << RGB2NIR_resizeFactor;
			fs << "REC" << RGB2NIR_cropRect;
		}
		fs.release();
	}
}


void CameraCalibration::loadCalibrationFile(QStringList calibFiles)
{
	int cnt = 0;
	foreach(QString file, calibFiles)
	{
		if(cnt >= nrOfNIRChannels){ break; }

		FileStorage fs(file.toStdString().c_str(), FileStorage::READ);

		//if stereo calibration file, load and return
		if(file.contains(".stereocal") || file.contains(".rgbnirstcal"))
		{
			Mat CML, CMR, DL, DR;
			fs["CM_L"] >> CML;
			fs["CM_R"] >> CMR;
			fs["D_L"] >> DL;
			fs["D_R"] >> DR;
			fs["R"] >> rotationMatrix;
			fs["T"] >> translationVec;
			fs["E"] >> essentialMat;
			fs["F"] >> fundamentalMat;
			fs["R_L"] >> rectificTransf_L;
			fs["R_R"] >> rectificTransf_R;
			fs["P_L"] >> projectionMat_L;
			fs["P_R"] >> projectionMat_R;
			fs["Q"] >> disparity2DepthMat;

			camMatrices_goldeye[0] = CML.clone();
			camMatrices_goldeye[1] = CMR.clone();
			distCoeffs_goldeye[0] = DL.clone();
			distCoeffs_goldeye[1] = DR.clone();

			stereoCalibrated = true;
			rectifyMapsCreated = false; //trigger making the rectify maps once in stereo undist.&remap method
			cameraCalibrated = goldeyeCalibrated = false;

			if(file.contains(".rgbnirstcal"))
			{
				fs["FAC"] >> RGB2NIR_resizeFactor;
				fs["REC"] >> RGB2NIR_cropRect;
				RGB2NIR_fittingComputed = true;
			}
			fs.release();
			return;
		}

		//else load separate files
		Mat CM, D;
		fs["CM"] >> CM;
		fs["D"] >> D;

		camMatrices_goldeye[cnt] = CM.clone();
		distCoeffs_goldeye[cnt] = D.clone();

		cnt++;
		fs.release();
	}

	cameraCalibrated = goldeyeCalibrated = stereoCalibrated = false;
	if(cnt == 1){ cameraCalibrated = true; }
	else if(cnt == nrOfNIRChannels){ goldeyeCalibrated = true; }
}

void CameraCalibration::makeAndSaveHomogeneityMatrices(QStringList calibImgTarFiles, QString folderURL)
{
	//get calibration images for each waveband
	QList<Mat> b935, b1060, b1300, b1550;

	foreach(QString tarFileName, calibImgTarFiles)
	{
		QList<Mat> imgs = InOut::getImagesFromTarFile(tarFileName);
		b935.append(imgs.at(0));
		b1060.append(imgs.at(1));
		b1300.append(imgs.at(2));
		b1550.append(imgs.at(3));
	}

	//average over all images
	double total = calibImgTarFiles.length();
	int w = b935.at(0).cols;
	int h = b935.at(0).rows;
	Mat avg935_temp(h, w, CV_16UC1, cv::Scalar(0));
	Mat avg1060_temp(h, w, CV_16UC1, cv::Scalar(0));
	Mat avg1300_temp(h, w, CV_16UC1, cv::Scalar(0));
	Mat avg1550_temp(h, w, CV_16UC1, cv::Scalar(0));

	//first sum up
	int i, x, y;
	for(i = 0; i < total; ++i)
	{
		for(y = 0; y < h; ++y)
		{
			for(x = 0; x < w; ++x)
			{
				avg935_temp.at<ushort>(y, x) += b935.at(i).at<uchar>(y, x);
				avg1060_temp.at<ushort>(y, x) += b1060.at(i).at<uchar>(y, x);
				avg1300_temp.at<ushort>(y, x) += b1300.at(i).at<uchar>(y, x);
				avg1550_temp.at<ushort>(y, x) += b1550.at(i).at<uchar>(y, x);
			}
		}
	}

	//now make average
	Mat avg935(h, w, CV_8UC1, cv::Scalar(0));
	Mat avg1060(h, w, CV_8UC1, cv::Scalar(0));
	Mat avg1300(h, w, CV_8UC1, cv::Scalar(0));
	Mat avg1550(h, w, CV_8UC1, cv::Scalar(0));
	for(y = 0; y < h; ++y)
	{
		for(x = 0; x < w; ++x)
		{
			avg935.at<uchar>(y, x) = (uchar)(avg935_temp.at<ushort>(y, x) / total);
			avg1060.at<uchar>(y, x) = (uchar)(avg1060_temp.at<ushort>(y, x) / total);
			avg1300.at<uchar>(y, x) = (uchar)(avg1300_temp.at<ushort>(y, x) / total);
			avg1550.at<uchar>(y, x) = (uchar)(avg1550_temp.at<ushort>(y, x) / total);
		}
	}

	//maximize brightness
	double min, max935, max1060, max1300, max1550;
	minMaxLoc(avg935, &min, &max935);
	minMaxLoc(avg1060, &min, &max1060);
	minMaxLoc(avg1300, &min, &max1300);
	minMaxLoc(avg1550, &min, &max1550);

	//normalize images
	Mat norm935(h, w, CV_64F, cv::Scalar(0));
	Mat norm1060(h, w, CV_64F, cv::Scalar(0));
	Mat norm1300(h, w, CV_64F, cv::Scalar(0));
	Mat norm1550(h, w, CV_64F, cv::Scalar(0));

	for(y = 0; y < h; ++y)
	{
		for(x = 0; x < w; ++x)
		{
			norm935.at<double>(y, x) = 1.0 / ( (float)avg935.at<uchar>(y, x) / max935 );
			norm1060.at<double>(y, x) = 1.0 / ( (float)avg1060.at<uchar>(y, x) / max1060 );
			norm1300.at<double>(y, x) = 1.0 / ( (float)avg1300.at<uchar>(y, x) / max1300 );
			norm1550.at<double>(y, x) = 1.0 / ( (float)avg1550.at<uchar>(y, x) / max1550 );
		}
	}

	//save in matrix in this object
	homoGen935 = norm935.clone();
	homoGen1060 = norm1060.clone();
	homoGen1300 = norm1300.clone();
	homoGen1550 = norm1550.clone();
	homogeneityCalibrated = true;

	//save to disk
	QString parentFolder = folderURL.split("/").last();
	QString fileNm = folderURL + "/" + parentFolder + "_homogCalib";
	FileStorage fs(fileNm.toStdString().c_str(), FileStorage::WRITE);
	fs << "Calib935" << norm935;
	fs << "Calib1060" << norm1060;
	fs << "Calib1300" << norm1300;
	fs << "Calib1550" << norm1550;
	fs.release();
}

bool CameraCalibration::loadHomogeneityMatrices(QString loadFileURL)
{
	FileStorage fs(loadFileURL.toStdString().c_str(), FileStorage::READ);
	Mat m1, m2, m3, m4;
	fs["Calib935"] >> m1;
	fs["Calib1060"] >> m2;
	fs["Calib1300"] >> m3;
	fs["Calib1550"] >> m4;

	homoGen935 = m1.clone();
	homoGen1060 = m2.clone();
	homoGen1300 = m3.clone();
	homoGen1550 = m4.clone();

	homogeneityCalibrated = true;
	fs.release();
	return true;
}

void CameraCalibration::applyHomogeneityMatrices(QString multiChImgtarFile)
{
	QList<Mat> imgs = InOut::getImagesFromTarFile(multiChImgtarFile);
	if(imgs.length() != 4){ return; }
	QString path = QFileInfo(multiChImgtarFile).path();

	int w = imgs.first().cols;
	int h = imgs.first().rows;
	int x,y;

	Mat c935(h, w, CV_8UC1, cv::Scalar(0));
	Mat c1060(h, w, CV_8UC1, cv::Scalar(0));
	Mat c1300(h, w, CV_8UC1, cv::Scalar(0));
	Mat c1550(h, w, CV_8UC1, cv::Scalar(0));


	for(y = 0; y < h; ++y)
	{
		for(x = 0; x < w; ++x)
		{
			c935.at<uchar>(y,x) = (uchar)std::min( 255, (int)( (double)imgs.at(0).at<uchar>(y,x) * homoGen935.at<double>(y,x) ) );
			c1060.at<uchar>(y,x) = (uchar)std::min( 255, (int)( (double)imgs.at(1).at<uchar>(y,x) * homoGen1060.at<double>(y,x) ) );
			c1300.at<uchar>(y,x) = (uchar)std::min( 255, (int)( (double)imgs.at(2).at<uchar>(y,x) * homoGen1300.at<double>(y,x) ) );
			c1550.at<uchar>(y,x) = (uchar)std::min( 255, (int)( (double)imgs.at(3).at<uchar>(y,x) * homoGen1550.at<double>(y,x) ) );
		}
	}

//	imshow("1", c935);
//	imshow("2", c1060);
//	imshow("3", c1300);
//	imshow("4", c1550);

	QString outNm = multiChImgtarFile.remove(".tar");
	imwrite((outNm + "_homgen935.png").toStdString().c_str(), c935);
	imwrite((outNm + "_homgen1060.png").toStdString().c_str(), c1060);
	imwrite((outNm + "_homgen1300.png").toStdString().c_str(), c1300);
	imwrite((outNm + "_homgen1550.png").toStdString().c_str(), c1550);
}

void CameraCalibration::makeHomographyMatrix(QString calibfile_A, QString calibfile_B, Size chessboardSize)
{
	Mat a = imread(calibfile_A.toStdString(), IMREAD_GRAYSCALE);
	Mat b = imread(calibfile_B.toStdString(), IMREAD_GRAYSCALE);
	vector<Point2f> cornersA, cornersB;
	findChessboardCorners(a, chessboardSize, cornersA, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
	findChessboardCorners(b, chessboardSize, cornersB, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);

	Mat homography = findHomography(cornersA, cornersB, CV_RANSAC);

	QString outDir = calibfile_A.remove(calibfile_A.split("/").last());
	string outNm = (outDir + "homography.mat").toStdString();
	FileStorage fs(outNm, CV_STORAGE_WRITE);
	fs << "homography" << homography;
	fs.release();
}















