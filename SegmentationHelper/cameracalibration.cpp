#include "cameracalibration.h"

CameraCalibration::CameraCalibration(QWidget *parent):
	cameraCalibrated(false),
    stereoCalibrated(false),
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

    //start stereo rectification
    Mat R1, R2, P1, P2, Q;
    stereoRectify(CM_L, D_L, CM_R, D_R, imgSize, rotMat, translVec, R1, R2, P1, P2, Q);

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

void CameraCalibration::undistortAndRemapStereoImages(QString fileNameLeft, QString fileNameRight)
{
    Mat mapLx, mapLy, mapRx, mapRy;
    Mat imgL, imgR, imgUL, imgUR;

    //load images and to remapping
    imgL = imread(fileNameLeft.toStdString().c_str());
    imgR = imread(fileNameRight.toStdString().c_str());

    initUndistortRectifyMap(cameraMatrices[0], distCoefficients[0], rectificTransf_L, projectionMat_L,
            imgL.size(), CV_32FC1, mapLx, mapLy);
    initUndistortRectifyMap(cameraMatrices[1], distCoefficients[1], rectificTransf_R, projectionMat_R,
            imgR.size(), CV_32FC1, mapRx, mapRy);

    remap(imgL, imgUL, mapLx, mapLy, INTER_LINEAR, BORDER_CONSTANT, Scalar());
    remap(imgR, imgUR, mapRx, mapRy, INTER_LINEAR, BORDER_CONSTANT, Scalar());

    //save images
    QString nmLeft = (fileNameLeft.remove(".png").remove(".jpg")).append("_remap.png");
    QString nmRight = (fileNameRight.remove(".png").remove(".jpg")).append("_remap.png");
    imwrite(nmLeft.toStdString().c_str(), imgUL);
    imwrite(nmRight.toStdString().c_str(), imgUR);
    QMessageBox::information(parentWidget, "Save successful", "Remapped images have been saved", QMessageBox::Ok);
}

bool CameraCalibration::liveCalibrateSingleCamera(int nrOfChessboards, int chessboard_width, int chessboard_height)
{
//	bool found = false;
//	int successes = 0;
//	int width = chessboard_width; //nr of inner corners in row
//	int height = chessboard_height; //nr of inner corner in column

//	Size chessboard_sz = Size(width, height);
//	Size imgSize;
//	int nrOfCorners = width*height;  //total number of inner corners

//	vector<vector<Point3f> > objectPoints;
//	vector<vector<Point2f> > imagePoints;

//	Mat img, imgGray;
//	VideoCapture capture = VideoCapture(0);
//	if( !capture.isOpened() )
//	{
//		QMessageBox::information(NULL, "Error", "Capture couldn't be iniciated (no camera?)", QMessageBox::Ok);
//		return false;
//	}
//	QMessageBox::information(NULL, "Start Capture", "Press SPACE to save detected corners and ESC to cancel", QMessageBox::Ok);

//	//objectPoints should contain physical location of each corners but
//	//since we don’t know that so we assign constant positions to all the corners
//	vector<Point3f> obj;
//	for (int j = 0; j < nrOfCorners; j++)
//	{
//		obj.push_back(Point3f(j / width, j % width, 0.0f));
//	}

//	//get chessboard corners until enough boards have been succesfully analyzed
//	int cnt = 0;
//	capture >> img;
//	while(successes < nrOfChessboards)
//	{
//		vector<Point2f> corners;
//		cvtColor(img, imgGray, CV_BGR2GRAY);
//		found = findChessboardCorners(imgGray, chessboard_sz, corners,
//										CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);

//		if(found)
//		{
//			cornerSubPix(imgGray, corners, Size(11, 11), Size(-1, -1),
//						 TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1)); //increase corner accuracy to sub-pixel level
//			drawChessboardCorners(img, chessboard_sz, corners, found);
//			if(cnt++ == 0) { imgSize = img.size(); }
//		}

//		//show image with corners and wait for user input
//		namedWindow("Image with detected Corners", CV_WINDOW_AUTOSIZE);
//		imshow("Image with detected Corners", img);
//		capture >> img;
//		int key = waitKey(1);

//		//quit for ESC and save corners for SPACE key
//		if(key==27) { return false; }

//		if(key==' ' && found!=0)
//		{
//			imagePoints.push_back(corners);
//			objectPoints.push_back(obj);

//			qDebug() << "Chessboard corners have been stored for image " << successes << "!";

//			successes++;
//			if(successes>=nrOfChessboards)
//				break;
//		}
//	}

//	//calibrate the camera and store matrizes
//	camMatrix = Mat(3, 3, CV_32FC1);
//	camMatrix.at<float>(0, 0) = 1;
//	camMatrix.at<float>(1, 1) = 1;
//	vector<Mat> rvecs, tvecs;

//	calibrateCamera(objectPoints, imagePoints, imgSize, camMatrix, distCoeff, rvecs, tvecs);

//	QMessageBox::information(NULL, "Calibration successful", "Camera has been calibrated", QMessageBox::Ok);

	return true;
}


void CameraCalibration::saveCalibrationFile(QString calibFileName, int channelIndex)
{
    if(calibFileName != "" && channelIndex < nrOfNIRChannels)
	{
        calibFileName += ".singlecal";
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
        calibFileName += ".sterecal";
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
