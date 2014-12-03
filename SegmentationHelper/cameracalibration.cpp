#include "cameracalibration.h"

CameraCalibration::CameraCalibration(QWidget *parent):
	cameraCalibrated(false),
	stereoCalibrated(false)
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
	calibrateCamFromImages(calibImgs, chessboard_width, chessboard_height);
	QMessageBox::information(parentWidget, "Calibration successful", "Camera has been calibrated", QMessageBox::Ok);

	//ask user where to store the calibration matrices
	QString calibFileName = QFileDialog::getSaveFileName(parentWidget, "Select file to store calibration matrices",
														 QDir::homePath(), "*.*");
	saveCalibrationMatrices(calibFileName);

	cameraCalibrated = true;
}

void CameraCalibration::calibrateGoldeyeMultiChannel(QStringList calibImgTarFiles, int chessboard_width, int chessboard_height)
{
	//ask user where to store the calibration matrices
	QString calibFileName = QFileDialog::getSaveFileName(parentWidget, "Select file to store calibration matrices",
														 QDir::homePath(), "*.*");

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
	calibrateCamFromImages(b935, chessboard_width, chessboard_height, true);
	saveCalibrationMatrices(calibFileName + "_935");
	calibrateCamFromImages(b1060, chessboard_width, chessboard_height, true);
	saveCalibrationMatrices(calibFileName + "_1060");
	calibrateCamFromImages(b1300, chessboard_width, chessboard_height, true);
	saveCalibrationMatrices(calibFileName + "_1300");
	calibrateCamFromImages(b1550, chessboard_width, chessboard_height, true);
	saveCalibrationMatrices(calibFileName + "_1550");

	QMessageBox::information(parentWidget, "Calibration successful", "Goldeye has been calibrated", QMessageBox::Ok);
	cameraCalibrated = true;
}

void CameraCalibration::calibrateCamFromImages(QList<Mat> calibImgs, int chessboard_width, int chessboard_height, bool isGrayScale)
{
	bool success = false;
//	int nrOfChessboards;
	int width = chessboard_width; //nr of inner corners in row
	int height = chessboard_height; //nr of inner corner in column

	Size chessboard_sz = Size(width, height);
	Size imgSize;
	int nrOfCorners = width*height;  //total number of inner corners

	vector<vector<Point3f> > objectPoints;
	vector<vector<Point2f> > imagePoints;

	//objectPoints should contain physical location of each corners but
	//since we don’t know that so we assign constant positions to all the corners
	vector<Point3f> obj;
	for (int j = 0; j < nrOfCorners; j++)
	{
		obj.push_back(Point3f(j / width, j % width, 0.0f));
	}

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
			if(cnt++ == 0) { imgSize = img.size(); }
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

	//calibrate the camera and store matrizes
	camMatrix = Mat(3, 3, CV_32FC1);
	camMatrix.at<float>(0, 0) = 1;
	camMatrix.at<float>(1, 1) = 1;
	vector<Mat> rvecs, tvecs;

	calibrateCamera(objectPoints, imagePoints, imgSize, camMatrix, distCoeff, rvecs, tvecs);
}

void CameraCalibration::undistortSingleImage(QString fileName)
{
	Mat img, imgUndist;
	img = imread(fileName.toStdString().c_str());
	undistort(img, imgUndist, camMatrix, distCoeff);

	namedWindow("original", CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO);
	namedWindow("undistorted", CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO);
	imshow("original", img);
	imshow("undistorted", imgUndist);
}

void CameraCalibration::undistortGoldeyeMultiChImg(QString tarFileName)
{
	QString basicFileName = QFileDialog::getSaveFileName(parentWidget, "Select file to store original and undistorted channel images",
														 QDir::homePath(), "*.png");
	basicFileName.remove(".png");

	QStringList wavebands;
	wavebands << "935" << "1060" << "1300" << "1550";

	//undistort all channels of multichannel image
	QList<Mat> imgs = InOut::getImagesFromTarFile(tarFileName);
	for(int i = 0; i< imgs.length(); ++i)
	{
		Mat imgU;
		undistort(imgs.at(i), imgU, camMatrix, distCoeff);

		//save orig and undistorted for all channels
		QString imgNm = basicFileName + "_" + wavebands.at(i) + "_orig.png";
		QString imgUndistNm = basicFileName + "_" + wavebands.at(i) + "_undist.png";
		imwrite(imgNm.toStdString().c_str(), imgs.at(i));
		imwrite(imgUndistNm.toStdString().c_str(), imgU);
	}

	QMessageBox::information(parentWidget, "Save successful", "Original and undistored waveband images have been saved", QMessageBox::Ok);
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


void CameraCalibration::saveCalibrationMatrices(QString calibFileName)
{
	if(calibFileName != "")
	{
		FileStorage fs(calibFileName.toStdString().c_str(), FileStorage::WRITE);
		fs << "CM" << camMatrix;
		fs << "D" << distCoeff;
		fs.release();
	}
}
