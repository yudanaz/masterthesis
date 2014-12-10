#include "cameracalibration.h"

CameraCalibration::CameraCalibration(QWidget *parent):
	cameraCalibrated(false),
	stereoCalibrated(false),
	rectifyMapsCreated(false),
	nrOfNIRChannels(4), //4 NIR channels max right now (will change with new flashlight)
	cameraMatrices(4),
	distCoefficients(4)
{
	parentWidget = parent;
}

void CameraCalibration::calibrateSingleCamera(QStringList calibImgFiles, int chessboard_width, int chessboard_height)
{
	//get images by name and use them for calibration
	QList<Mat> calibImgs;
	foreach(QString s, calibImgFiles)
	{
		calibImgs.append( imread(s.toStdString().c_str()) );
	}
	calibrateCamFromImages(calibImgs, 0, chessboard_width, chessboard_height);
	QMessageBox::information(parentWidget, "Calibration successful", "Camera has been calibrated", QMessageBox::Ok);
	cameraCalibrated = true;
	goldeyeCalibrated = false;
	stereoCalibrated = false;

	//ask user where to store the calibration matrices
	QString calibFileName = QFileDialog::getSaveFileName(parentWidget, "Select file to store calibration matrices",
														 QDir::homePath());
	saveCalibrationFile(calibFileName, 0);
}

void CameraCalibration::calibrateGoldeyeMultiChannel(QStringList calibImgTarFiles, int chessboard_width, int chessboard_height)
{
	//ask user where to store the calibration matrices
	QString calibFileName = QFileDialog::getSaveFileName(parentWidget, "Select file to store calibration matrices",
														 QDir::homePath());

	//open each tar file and get band images
	QList<Mat> b935, b1060, b1300, b1550;

	foreach(QString tarFile, calibImgTarFiles)
	{
		QList<Mat> imgs = InOut::getImagesFromTarFile(tarFile);
		if(imgs.length() < 4){ return; }
		b935.append(imgs.at(0));
		b1060.append(imgs.at(1));
		b1300.append(imgs.at(2));
		b1550.append(imgs.at(3));
	}

	//make calibration and save calibration matrix for each waveband
	calibrateCamFromImages(b935, 0, chessboard_width, chessboard_height, true);
	saveCalibrationFile(calibFileName + "_935", 0);
	calibrateCamFromImages(b1060, 1, chessboard_width, chessboard_height, true);
	saveCalibrationFile(calibFileName + "_1060", 1);
	calibrateCamFromImages(b1300, 2, chessboard_width, chessboard_height, true);
	saveCalibrationFile(calibFileName + "_1300", 2);
	calibrateCamFromImages(b1550, 3, chessboard_width, chessboard_height, true);
	saveCalibrationFile(calibFileName + "_1550", 3);

	QMessageBox::information(parentWidget, "Calibration successful", "Goldeye has been calibrated", QMessageBox::Ok);
	goldeyeCalibrated = true;
	cameraCalibrated = false;
	stereoCalibrated = false;
}

void CameraCalibration::calibrateStereoCameras(QStringList calibFilesLeft, QStringList calibFilesRight,
											   int chessboard_width, int chessboard_height)
{
	//get calib images for left and right camera
	QList<Mat> calibImgsLeft, calibImgsRight;
	foreach(QString file, calibFilesLeft){ calibImgsLeft.append(imread(file.toStdString().c_str())); }
	foreach(QString file, calibFilesRight){ calibImgsRight.append(imread(file.toStdString().c_str())); }
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
	cameraMatrices[0] = CM_L.clone();
	cameraMatrices[1] = CM_R.clone();
	distCoefficients[0] = D_L.clone();
	distCoefficients[1] = D_R.clone();
	rotationMatrix = rotMat.clone();
	translationVec = translVec.clone();
	essentialMat = E.clone();
	fundamentalMat = F.clone();
	rectificTransf_L = R1.clone();
	rectificTransf_R = R2.clone();
	projectionMat_L = P1.clone();
	projectionMat_R = P2.clone();
	disparity2DepthMat = Q.clone();

	//ask user where to store the calibration data and save
	QString calibFileName = QFileDialog::getSaveFileName(parentWidget, "Select file to store stereo calibration file",
														 QDir::homePath());
	saveStereoCalibrationFile(calibFileName);

	QMessageBox::information(parentWidget, "Calibration successful", "Stereo Camera Pair has been calibrated", QMessageBox::Ok);
	goldeyeCalibrated = false;
	cameraCalibrated = false;
	stereoCalibrated = true;
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
	cameraMatrices[channelIndex] = camMatrix.clone();
	distCoefficients[channelIndex] = distCoeff.clone();
}

void CameraCalibration::getObjectAndImagePoints(QList<Mat> calibImgs, int width, int height,
												vector<Point3f> obj,
												vector<vector<Point3f> >& objectPoints,
												vector<vector<Point2f> >& imagePoints,
												bool isGrayScale)
{
	bool success = false;
	Size chessboard_sz = Size(width, height);

	//get chessboard corners for all images
	QProgressDialog progress("Analyzing chessboard patterns", "Cancel", 0, calibImgs.length(), parentWidget);
	progress.setMinimumWidth(450);
	progress.setMinimumDuration(1000);
	progress.setWindowModality(Qt::WindowModal);
	int progressCnt = 0;

	int cnt = 0;
	foreach(Mat img, calibImgs)
	{
		vector<Point2f> corners;
		Mat imgGray;
		if(!isGrayScale){ cvtColor(img, imgGray, CV_BGR2GRAY); }
		else{ imgGray = img; }
		success = findChessboardCorners(imgGray, chessboard_sz, corners,
										CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);

		if(success)
		{
			imagePoints.push_back(corners);
			objectPoints.push_back(obj);
		}

		//show found corners in image
		drawChessboardCorners(img, chessboard_sz, corners, success);
		namedWindow("found corners", CV_WINDOW_KEEPRATIO);
		imshow("found corners", img);
		cvWaitKey(2);

		progress.setValue(progressCnt++);
		if(progress.wasCanceled()) { return; }
	}
	progress.setValue(calibImgs.length());
	destroyWindow("found corners");
}

void CameraCalibration::undistortSingleImage(QString fileName)
{
	Mat img, imgUndist;
	img = imread(fileName.toStdString().c_str());
	undistort(img, imgUndist, cameraMatrices[0], distCoefficients[0]);

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
			undistort(imgs.at(i), imgU, cameraMatrices[i], distCoefficients[i]);

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
	initUndistortRectifyMap(cameraMatrices[0], distCoefficients[0], rectificTransf_L, projectionMat_L,
			leftImgSize, CV_32FC1, mapLx, mapLy);
	initUndistortRectifyMap(cameraMatrices[1], distCoefficients[1], rectificTransf_R, projectionMat_R,
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

	//correspondence vai SAD
	sbm.state->minDisparity = minDisparity;
	sbm.state->numberOfDisparities = nrOfDisparities;
	sbm.state->SADWindowSize = SADWindowSize;

	//post filters -> remove bad matches
	sbm.state->textureThreshold = textureThresh;
	sbm.state->uniquenessRatio = uniquenessRatio;
	sbm.state->speckleWindowSize = specklewindowSize;
	sbm.state->speckleRange = speckleRange;
}

Mat CameraCalibration::makeDisparityImage(Mat leftGrayImg, Mat rightGrayImg)
{
	//set parameters

	//smooth images
//	GaussianBlur(leftGrayImg, leftGrayImg, Size(smoothKernelSize, smoothKernelSize), 0);
//	GaussianBlur(rightGrayImg, rightGrayImg, Size(smoothKernelSize, smoothKernelSize), 0);

	//compute disparities
	Mat disp(leftGrayImg.rows, leftGrayImg.cols, CV_16SC1);
	sbm(leftGrayImg, rightGrayImg, disp, CV_16S);

	//normalize (from 16 to 8 bit) and return
	double minVal, maxVal;
	minMaxLoc(disp, &minVal, &maxVal); //find minimum and maximum intensities
	Mat disp2;
	disp.convertTo(disp2,CV_8U, 255.0/(maxVal - minVal), -minVal * 255.0/(maxVal - minVal));
	return disp2;
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
		std::nth_element(matches.begin(), matches.begin()+numBest, matches.end()); //sort to 10th position
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
		fs << "CM" << cameraMatrices[channelIndex];
		fs << "D" << distCoefficients[channelIndex];
		fs.release();
	}
}

void CameraCalibration::saveStereoCalibrationFile(QString calibFileName)
{
	if(calibFileName != "")
	{
		calibFileName = calibFileName.remove(".stereocal").append(".stereocal");
		FileStorage fs(calibFileName.toStdString().c_str(), FileStorage::WRITE);
		fs << "CM_L" << cameraMatrices[0];
		fs << "CM_R" << cameraMatrices[1];
		fs << "D_L" << distCoefficients[0];
		fs << "D_R" << distCoefficients[1];
		fs << "R" << rotationMatrix;
		fs << "T" << translationVec;
		fs << "E" << essentialMat;
		fs << "F" << fundamentalMat;
		fs << "R_L" << rectificTransf_L;
		fs << "R_R" << rectificTransf_R;
		fs << "P_L" << projectionMat_L;
		fs << "P_R" << projectionMat_R;
		fs << "Q" << disparity2DepthMat;
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
		if(file.contains(".stereocal"))
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

			cameraMatrices[0] = CML.clone();
			cameraMatrices[1] = CMR.clone();
			distCoefficients[0] = DL.clone();
			distCoefficients[1] = DR.clone();

			stereoCalibrated = true;
			rectifyMapsCreated = false; //trigger making the rectify maps once in stereo undist.&remap method
			cameraCalibrated = goldeyeCalibrated = false;
			return;
		}

		//else load separate files
		Mat CM, D;
		fs["CM"] >> CM;
		fs["D"] >> D;

		cameraMatrices[cnt] = CM.clone();
		distCoefficients[cnt] = D.clone();

		cnt++;
	}

	cameraCalibrated = goldeyeCalibrated = stereoCalibrated = false;
	if(cnt == 1){ cameraCalibrated = true; }
	else if(cnt == nrOfNIRChannels){ goldeyeCalibrated = true; }
}
