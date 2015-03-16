#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "seeds/seeds2.h"
#include "seeds/seedsHelper.h"
#include "slic/slic.h"
#include "segmentfelsenzwalb/segmentation.h"
#include "helper.h"
#include "imagepreprocessor.h"

#define WHITE Scalar(255, 255, 255)
#define PINK Scalar(255, 51, 153)
#define IMGTYPES "*.jpg *.png *.ppm"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	lastDir(QDir::homePath()),
	colorMap(256, vector<int>(3)),
	objectMap(256),
	camCalib(this)
{
	ui->setupUi(this);

	//read color map from app dir
	QFile colorfin(QDir::currentPath() + "/VOCcolormap.txt");
	colorfin.open(QFile::ReadOnly | QFile::Text);
	QTextStream colorIn(&colorfin);
	int count = 0;
	while (!colorIn.atEnd() && count < 256)
	{
		QStringList rgb = colorIn.readLine().split("\t", QString::SkipEmptyParts);
		colorMap[count][0] = rgb.at(0).toDouble() * 255;
		colorMap[count][1] = rgb.at(1).toDouble() * 255;
		colorMap[count][2] = rgb.at(2).toDouble() * 255;
//		qDebug() << colorMap[count][0] << " " << colorMap[count][1] << " " << colorMap[count][2];
		count++;
	}
	colorfin.close();

	//red object map from app dir
	QFile objectfin(QDir::currentPath() + "/ObjectClasses.txt");
	objectfin.open(QFile::ReadOnly | QFile::Text);
	QTextStream objectIn(&objectfin);
	count = 0;
	while (!objectIn.atEnd() && count < 255)
	{
		objectMap[count] = objectIn.readLine();
//		qDebug() << objectMap[count];
		count++;
	}
	objectfin.close();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::makeLabelImages(QStringList fileNames)
{
	QDir path = QFileInfo(fileNames.first()).path();

	QList<Mat> colorImgs;
	QList<Mat> grayImgs;

	QList<QString> ColorFileNames;
	QList<QString> GrayFileNames;
	int rows = 0;
	int cols = 0;

	vector<int> imageSettings;
	imageSettings.push_back(CV_IMWRITE_PNG_COMPRESSION);
	imageSettings.push_back(5); //compression value

	//make progress bar
	QProgressDialog progress("Creating Label Images", "Cancel", 0, fileNames.length(), this);
	progress.setMinimumWidth(450);
	progress.setMinimumDuration(1000);
	progress.setWindowModality(Qt::WindowModal);
	int progressCnt = 0;

	//for each xml, create image and draw objects
	foreach (QString fnm, fileNames)
	{
		QFile fin(fnm);
		fin.open(QFile::ReadOnly | QFile::Text);
		QXmlStreamReader xml(&fin);

		Mat colorImg(rows, cols, CV_8UC3);
		Mat grayImg(rows, cols, CV_8UC1);
		QString colorFileName = "";
		QString grayFileName = "";
		bool collectingPoints = false;
		int x = 0;
		int colorIndex;

		Polygon polygon;
		QList<Polygon> polygons;


		while (!xml.atEnd())
		{
			//look through tags
			if (xml.isStartElement())
			{
				QString tagName(xml.name().toString());

				//get width, height and output name and create images
				if(tagName == "filename")
				{
					QString elemText = xml.readElementText().remove(".jpg");
					colorFileName.append(path.absolutePath().append("/").append(elemText + "_labels_color.png"));
					grayFileName.append(path.absolutePath().append("/").append(elemText + "_labels.png"));
				}
				else if(tagName == "nrows"){ rows = xml.readElementText().toInt(); }
				else if(tagName == "ncols")
				{
					cols = xml.readElementText().toInt();

					//create new mat obj if rows, cols and type are different from original (that should happen only the first time)
					colorImg.create(rows, cols, CV_8UC3); //color label image
					grayImg.create(rows, cols, CV_8UC1); //greyscale label image

					//paint background color (black)
					colorImg = Scalar(0, 0, 0);
					grayImg = Scalar(0);
				}

				if (tagName == "object")
				{
					collectingPoints = true;
					polygon.points.clear(); //init
				}
				if(tagName == "name")
				{
					QString objType = xml.readElementText();

					//set color according to label
					colorIndex = 999; //in case no matching label name is found
					for (int i = 0; i < 256; ++i)
					{
						if(objectMap[i] == objType)
						{
							colorIndex = i;
							break;
						}
					}
					polygon.color = colorIndex;
				}
				if(tagName == "attributes")
				{
					polygon.index == xml.readElementText().toInt();
				}

				//collect all points of the polygon
				if(collectingPoints)
				{
					if(tagName == "x")
					{
						x = xml.readElementText().toInt();
					}
					if(tagName == "y")
					{
						int y = xml.readElementText().toInt();
						Point p(x, y);
						polygon.points.append(p);
					}
				}

			}
			else if (xml.isEndElement())
			{
				//when object tag is closed, add polygon to list
				if(xml.name().toString() == "object")
				{
					collectingPoints = false;
					polygons.append(polygon.clone());
				}
			}

			xml.readNext();
		}

		//draw the polygons in color according to object type

		foreach(Polygon polygon, polygons)
		{
			Point points[1][polygon.points.count()];

			for(int i = 0; i < polygon.points.count(); ++i)
			{
				points[0][i] = polygon.points[i];
			}

			const Point* ppt[1] = { points[0] };
			int npt[] = { polygon.points.length() };

			//paint labels in color and grayscale images
			if(colorIndex != 999) //if matching label has been found
			{
				Scalar color(colorMap[colorIndex][0], colorMap[colorIndex][1], colorMap[colorIndex][2]);
				Scalar gray(1 + colorIndex);
				fillPoly( colorImg, ppt, npt, 1, color, 8 );
				fillPoly( grayImg, ppt, npt, 1, gray, 8 );
			}
		}

		//save images to disk
		imwrite( colorFileName.toStdString().c_str(), colorImg, imageSettings);
		imwrite( grayFileName.toStdString().c_str(), grayImg, imageSettings);

		//show progress in progress bar
		progress.setValue(++progressCnt);
		if(progress.wasCanceled()) { return; }
	}
	progress.setValue(fileNames.length());
}

void MainWindow::makeSeedsSuperpixels(QString fileName)
{
	Mat img;
	img = imread(fileName.toStdString().c_str(), CV_LOAD_IMAGE_COLOR);
	//imshow("loaded Image", img);

	//get info for seed alg
	int seedW, seedH, seedLevels;
	QStringList seedInfo = ui->lineEdit_seedInfo->text().split(",");
	if(seedInfo.length() == 3)
	{
		seedW = seedInfo.at(0).toInt();
		seedH = seedInfo.at(1).toInt();
		seedLevels = seedInfo.at(2).toInt();
	}
	else
	{
		seedW = 2;
		seedH = 2;
		seedLevels = 4;
	}

	QTime myTimer;
	myTimer.start();

	//run SEEDS algorithm
	int NR_BINS = 5;
	int width = img.cols;
	int height = img.rows;
	int channels = img.channels();
	SEEDS seeds(width, height, channels, NR_BINS);
	seeds.initialize(seedW, seedH, seedLevels);
	seeds.update_image_ycbcr(img);
//	seeds.update_image_ycbcr(imgUINT);
	seeds.iterate();

	qDebug() << "elapsed time: " << myTimer.elapsed();

	//get labels, transform to opencv mat obj and display
//	UINT* _labels = seeds.get_labels();
	Mat cvLabels, cvLabels8bit;
	cvLabels = seeds.getLabelsAsMat();
	cvLabels.convertTo(cvLabels8bit, CV_8UC1, 256.0/seeds.count_superpixels());
	imshow("Labels", cvLabels8bit);
//	imshow("Original", img);

//	int sz = 3*width*height;

//	UINT* output_buff = new UINT[sz];
//	for (int i = 0; i<sz; i++) output_buff[i] = 0;

	UINT* imgUINT = seeds.getUINT(img);
	UINT *labels = seeds.get_labels();
	DrawContoursAroundSegments(imgUINT, labels, width, height, 0xFF1493, true);//0xff0000 draws white contours
	Mat labelsOverlayed = seeds.getMat(imgUINT);
	imshow("Superpixels", labelsOverlayed);

	//save label names
	QString fileNameOnly = fileName.split("/").last().split(".")[0];
	QString labelsFileName = lastDir + "/" + fileNameOnly + "labels.txt";
	seeds.SaveLabels_Text(labelsFileName.toStdString().c_str());

	//display the number of superpixels created with this settings
	QString nrOfSuperpxs = QString::number(seeds.count_superpixels());
	ui->label_nrOfSuperpxs->setText("= " + nrOfSuperpxs + " superpixels");
}

void MainWindow::makeSlicSuperpixels(QString fileName)
{
	//load img
	IplImage *img = cvLoadImage(fileName.toStdString().c_str(), CV_LOAD_IMAGE_COLOR);
	IplImage *labImg = cvCloneImage(img);

	QTime myTimer;
	myTimer.start();

	//convert to LAB space
	cvCvtColor(labImg, img, CV_BGR2Lab);

	/* get the number of superpixels and weight-factors from the user. */
	int w = img->width, h = img->height;
	int nr_superpixels, nc;
	QStringList slicInfo = ui->lineEdit_slicInfo->text().split(",");
	if(slicInfo.length() == 2)
	{
		nr_superpixels = slicInfo[0].toInt();
		nc = slicInfo[1].toInt();
	}
	else
	{
		nr_superpixels = 400; nc = 40;
	}

	double step = sqrt((w * h) / (double) nr_superpixels);

	/* Perform the SLIC superpixel algorithm. */
	Slic slic;
	slic.generate_superpixels(labImg, step, nc);
	slic.create_connectivity(labImg);

	qDebug() << "elapsed time: " << myTimer.elapsed();

	/* Display the contours and show the result. */
	slic.display_contours(img, CV_RGB(255,20,147));
	cvShowImage("Superpixels", img);
}




void MainWindow::makeFelsenzwalbSuperpixels(QString fileName)
{
	Segmentation segm;
	Mat img;
	img = imread(fileName.toStdString().c_str(), CV_LOAD_IMAGE_COLOR);

	QStringList segInfo = ui->lineEdit_felsenzwalbInfo->text().split(",");
	float sigma, k;
	int min;
	if(segInfo.length() == 3)
	{
		sigma = segInfo[0].toDouble();
		k = segInfo[1].toDouble();
		min = segInfo[2].toInt();
	}
	else
	{
		sigma = 0.5; k = 500; min = 20;
	}
	int num_ccs;
	imshow("Superpixels", segm.makeSuperPixelSegmenation(img, sigma, k, min, &num_ccs));
	ui->label_nrOfSuperpixelsFelsenzwalb->setText("= " + QString::number(num_ccs) + " superpixels");
}



//void MainWindow::makeDisparityImage(QString fileNameL, QString fileNameR)
void MainWindow::makeDisparityImage(Mat leftImgCol, Mat rightImgCol)
{
	bool improve = false;
	if(ui->checkBox_improveWithSeed->isChecked()){ improve = true; }

	//load L and R images as grayscale
	Mat leftImg, rightImg, disp, dispImprov;
	cvtColor(leftImgCol, leftImg, CV_BGR2GRAY);
	cvtColor(rightImgCol, rightImg, CV_BGR2GRAY);

	//set parameters
	camCalib.setDisparityParameters(ui->slider_minDisp->value(),
									ui->slider_dispRange->value(),
									ui->slider_SADwindow->value(),
									ui->slider_prefilterSize->value(),
									ui->slider_prefilterCAP->value(),
									ui->slider_textureThresh->value(),
									ui->slider_uniqueness->value(),
									ui->slider_speckleWindow->value(),
									ui->slider_speckleRange->value());

	//process
	disp = camCalib.makeDisparityMap(leftImg, rightImg, ui->checkBox_useSGBM->isChecked());

	if(improve)
	{
		//get info for seed alg
		int seedW, seedH, seedLevels;
		QStringList seedInfo = ui->lineEdit_seedInfo->text().split(",");
		if(seedInfo.length() == 3)
		{
			seedW = seedInfo.at(0).toInt();
			seedH = seedInfo.at(1).toInt();
			seedLevels = seedInfo.at(2).toInt();
		}
		else
		{
			seedW = 2;
			seedH = 2;
			seedLevels = 4;
		}

		//apply x-offset to original left image in order to get an image that is more aligned
		//with the disparity map (which is somewhere between L and R image)
		int xoffset = ui->lineEdit_SEEDxoffset->text().toInt();
		Mat leftImgColShifted = Mat::zeros(leftImgCol.size(), leftImgCol.type());
		leftImgCol(Rect(xoffset, 0,leftImgCol.cols-xoffset, leftImgCol.rows)).copyTo(
					leftImgColShifted(Rect(0, 0, leftImgCol.cols-xoffset, leftImgCol.rows)));

		//run SEEDS algorithm
		int NR_BINS = 5;
		int width = leftImg.cols;
		int height = leftImg.rows;
		int channels = leftImg.channels();
		SEEDS seeds(width, height, channels, NR_BINS);
		seeds.initialize(seedW, seedH, seedLevels); //hard-coded for now, see makeSeedsSuperpixels() method
		seeds.update_image_ycbcr(leftImgColShifted);
		seeds.iterate();

		//show labels
	//    Mat cvLabels, cvLabels8bit;
	//    cvLabels = seeds.getLabelsAsMat();
	//    cvLabels.convertTo(cvLabels8bit, CV_8UC1, 256.0/seeds.count_superpixels());
	//    imshow("seeds superpixels", cvLabels8bit);

		//improve disparity image with superpixels
		dispImprov = camCalib.improveDisparityMap(seeds.count_superpixels(), seeds.getLabelsAsMat(), disp);
	}

	//normalize (from 16 to 8 bit) and show
	double minVal, maxVal;
	Mat disp2, disp3;
	minMaxLoc(disp, &minVal, &maxVal); //find minimum and maximum intensities
	disp.convertTo(disp2, CV_8U, 255.0/(maxVal - minVal), -minVal * 255.0/(maxVal - minVal));
	//minMaxLoc(dispImprov, &minVal, &maxVal); //find minimum and maximum intensities
	if(improve) { dispImprov.convertTo(disp3, CV_8U, 255.0/(maxVal - minVal), -minVal * 255.0/(maxVal - minVal)); }

	imshow("left image", leftImg); // disparity image is aligned with left image
	//imshow("right image", rightImg);
	imshow("disparity map", disp2);
	if(improve){ imshow("improved disparity map", disp3); }
}

void MainWindow::makeSurfFeatures(QString fileName)
{
	Mat img, features, descriptors;
	img = imread(fileName.toStdString().c_str());
	std::vector<KeyPoint> keypoints;

	//get info
	QStringList sl = ui->lineEdit_surfInfo->text().split(",");
	if(sl.length() < 2) { return; }
	double thresh = sl.first().toDouble();

	QTime timer;
	timer.start();

	//find keypoints & descriptors
	if(sl.last().toLower() == "surf")
	{
		SURF surf(thresh);
		surf.detect(img, keypoints);
		surf.compute(img, keypoints, descriptors);
	}
	else if (sl.last().toLower() == "sift")
	{
		SIFT sift(0,3,thresh);
		sift.detect(img, keypoints);
		sift.compute(img, keypoints, descriptors);
	}

	//draw keypoints
	drawKeypoints(img, keypoints, features, PINK, DrawMatchesFlags::DEFAULT);
	imshow("Features", features);

	//draw decriptors as grayscale matrix
	Mat descr_transp;
	transpose(descriptors, descr_transp);
	imshow("Descriptors", descr_transp);
//    qDebug() << "keypoint descriptors:";
//    int y,x;
//    for(y = 0; y < descriptors.rows; ++y)
//    {
//        QString s = "";
//        for(x = 0; x < descriptors.cols; ++x)
//            s += QString::number(descriptors.at<uchar>(y,x)) + ", ";
//        qDebug() << s;
//    }

	//output some stuff
	qDebug() << "proc time: " << timer.elapsed() << " ms";
	qDebug() << "number of features: " << keypoints.size();
}



/*******************************************************
********************************************************
********************************************************
*** GUI METHODS:
*******************************************************
*******************************************************
*******************************************************/
void MainWindow::on_btn_makeLabelImgs_released()
{
	QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Select XML files"), lastDir, tr("*.xml"));
	if(fileNames.length() == 0) return;
	lastDir = QFileInfo(fileNames.first()).absolutePath();
	makeLabelImages(fileNames);
}

void MainWindow::on_btn_seed_released()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Select image file"), lastDir, tr(IMGTYPES));
	if(fileName == "") return;
	lastDir = QFileInfo(fileName).path();
	lastSeedsFilename = fileName;
	makeSeedsSuperpixels(fileName);
}

void MainWindow::on_pushButton_seedAgain_released()
{
	if(lastSeedsFilename != "") { makeSeedsSuperpixels(lastSeedsFilename); }
}

void MainWindow::on_btn_slic_released()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Select image file"), lastDir, tr(IMGTYPES));
	if(fileName == "") return;
	lastDir = QFileInfo(fileName).path();
	lastSlicFilename = fileName;
	makeSlicSuperpixels(fileName);
}

void MainWindow::on_pushButton_slicAgain_released()
{
	if(lastSlicFilename != "") { makeSlicSuperpixels(lastSlicFilename); }
}

void MainWindow::on_btn_felsenzwalb_released()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Select image file"), lastDir, tr(IMGTYPES));
	if(fileName == "") return;
	lastDir = QFileInfo(fileName).path();
	lastFelsenzwalbFilename = fileName;
	makeFelsenzwalbSuperpixels(fileName);
}

void MainWindow::on_pushButton_felsenzwalbAgain_released()
{
	if(lastFelsenzwalbFilename != "") { makeFelsenzwalbSuperpixels(lastFelsenzwalbFilename); }
}

void MainWindow::on_btn_surf_released()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Select image file"), lastDir, tr(IMGTYPES));
	if(fileName == "") return;
	lastDir = QFileInfo(fileName).path();
	lastFeatureFileName = fileName;
	makeSurfFeatures(fileName);
}

void MainWindow::on_pushButton_surfAgain_released()
{
	if(lastFeatureFileName != "") { makeSurfFeatures(lastFeatureFileName); }
}

void MainWindow::on_btn_calibSingle_released()
{
	QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Select calibration chessboard images"), lastDir, tr(IMGTYPES));
	if(fileNames.count() == 0) return;
	lastDir = QFileInfo(fileNames.first()).path();

	//get images by name and use them for calibration
	QList<Mat> calibImgs;
	foreach(QString s, fileNames)
	{
		calibImgs.append( imread(s.toStdString().c_str()) );
	}

	//ask user where to store the calibration matrices
	QString calibFileName = QFileDialog::getSaveFileName(this, "Select file to store calibration matrices",
														 QDir::homePath());

	QStringList params = ui->lineEdit_calibSingleInfo->text().split(",");
	camCalib.calibrateSingleCamera(calibImgs, params.first().toInt(), params.last().toInt(), calibFileName);
}

void MainWindow::on_btn_calibGoldeye_released()
{
	QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Select tar-files containing chessboard images"), lastDir, tr("*.tar"));
	if(fileNames.count() == 0) return;
	lastDir = QFileInfo(fileNames.first()).path();

	//get calibration image packs from tar files
	QList< QList<Mat> > calibImgs;
	foreach(QString tarFile, fileNames)
	{
		QList<Mat> imgs = InOut::getImagesFromTarFile(tarFile);
		if(imgs.length() < 4){ return; }
		calibImgs.append(imgs);
	}

	//ask user where to store the calibration matrices
	QString calibFileName = QFileDialog::getSaveFileName(this, "Select file to store calibration matrices",
														 QDir::homePath());

	QStringList params = ui->lineEdit_calibSingleInfo->text().split(",");
	camCalib.calibrateGoldeyeMultiChannel(calibImgs, params.first().toInt(), params.last().toInt(), calibFileName);
	QMessageBox::information(this, "Calibration successful", "Goldeye has been calibrated", QMessageBox::Ok);
}

void MainWindow::on_btn_calibStereo_released()
{
	QStringList fileNames_L = QFileDialog::getOpenFileNames(this, tr("Select LEFT camera files containing chessboard image"), lastDir);
	if(fileNames_L.count() == 0) return;
	lastDir =QFileInfo(fileNames_L.first()).path();
	QStringList fileNames_R = QFileDialog::getOpenFileNames(this, tr("Select RIGHT camera files containing chessboard image"), lastDir);
	if(fileNames_R.count() == 0) return;
	lastDir =QFileInfo(fileNames_R.first()).path();

	//get calib images for left and right camera
	QList<Mat> calibImgsLeft, calibImgsRight;
	foreach(QString file, fileNames_L){ calibImgsLeft.append(imread(file.toStdString().c_str())); }
	foreach(QString file, fileNames_R){ calibImgsRight.append(imread(file.toStdString().c_str())); }

	//ask user where to store the calibration data and save
	QString calibFileName = QFileDialog::getSaveFileName(this, "Select file to store stereo calibration file",
														 QDir::homePath());

	QStringList params = ui->lineEdit_calibSingleInfo->text().split(",");
	camCalib.calibrateStereoCameras(calibImgsLeft, calibImgsRight, params.first().toInt(), params.last().toInt(), calibFileName);
}

void MainWindow::on_btn_calibStereoGRBNIR_released()
{
	QStringList fileNames_L = QFileDialog::getOpenFileNames(this, tr("Select NIR (left) camera files containing chessboard image"), lastDir);
	if(fileNames_L.count() == 0) return;
	lastDir =QFileInfo(fileNames_L.first()).path();
	QStringList fileNames_R = QFileDialog::getOpenFileNames(this, tr("Select RGB (right) camera files containing chessboard image"), lastDir);
	if(fileNames_R.count() == 0) return;
	lastDir =QFileInfo(fileNames_R.first()).path();

	//get calib images for left and right camera
	QList<Mat> calibImgsNIR_L, calibImgsRGB_R;
	foreach(QString file, fileNames_L){ calibImgsNIR_L.append(imread(file.toStdString().c_str())); }
	foreach(QString file, fileNames_R){ calibImgsRGB_R.append(imread(file.toStdString().c_str())); }

	//ask user where to store the calibration data and save
	QString calibFileName = QFileDialog::getSaveFileName(this, "Select file to store NIR-RGB-stereo calibration file",
														 QDir::homePath());

	QStringList params = ui->lineEdit_calibSingleInfo->text().split(",");
	camCalib.calibrateRGBNIRStereoCameras(calibImgsNIR_L, calibImgsRGB_R, params.first().toInt(), params.last().toInt(), calibFileName);
}

void MainWindow::on_btn_undistSingle_released()
{
	if(!camCalib.isCalibrated_cam())
	{
		QMessageBox::information(this, "Error", "Camera has not been calibrated", QMessageBox::Ok);
		return;
	}

	QString fileName = QFileDialog::getOpenFileName(this, tr("Select image file to undistort"), lastDir, tr(IMGTYPES));
	lastDir =QFileInfo(fileName).path();
	if(fileName != ""){ camCalib.undistortSingleImage(fileName); }
}


void MainWindow::on_btn_undistGoldeye_released()
{
	if(!camCalib.isCalibrated_goldeye())
	{
		QMessageBox::information(this, "Error", "Goldeye has not been calibrated", QMessageBox::Ok);
		return;
	}

	QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Select multichannel image to undistort"), lastDir, tr("*.tar"));
	lastDir =QFileInfo(fileNames.first()).path();
	if(fileNames.length() > 0){ camCalib.undistortGoldeyeMultiChImg(fileNames); }
}


void MainWindow::on_btn_undistStereo_released()
{
	if(!camCalib.isCalibrated_stereo())
	{
		QMessageBox::information(this, "Error", "Stereo Cam Pair has not been calibrated", QMessageBox::Ok);
		return;
	}
	undistortStereo(false);
}


void MainWindow::on_btn_undistStereoRGBNIR_released()
{
	if(!camCalib.isCalibrated_stereo())
	{
		QMessageBox::information(this, "Error", "Stereo Cam Pair has not been calibrated", QMessageBox::Ok);
		return;
	}
	if(!camCalib.isComputed_RGB2NIRfitting())
	{
		QMessageBox::information(this, "Error", "The RGB-to-NIR-fitting has not been computed", QMessageBox::Ok);
		return;
	}
	undistortStereo(true);
}

void MainWindow::undistortStereo(bool isRGB_NIR_Stereo)
{
	//get files
	QStringList fileNames_L = QFileDialog::getOpenFileNames(this, tr("Select LEFT image file(s)"), lastDir, tr(IMGTYPES));
	if(fileNames_L.length() == 0){ return; }
	lastDir = QFileInfo(fileNames_L.first()).path();
	QStringList fileNames_R = QFileDialog::getOpenFileNames(this, tr("Select RIGHT image file(s)"), lastDir, tr(IMGTYPES));
	if(fileNames_R.length() == 0){ return; }
	lastDir = QFileInfo(fileNames_R.first()).path();

	QString nmLeft, nmRight;

	//make undistorted images for all stereo pairs
	for(int i = 0; i < fileNames_L.length(); ++i)
	{
		QString fileName_L = fileNames_L.at(i);
		QString fileName_R = fileNames_R.at(i);

		if(fileName_L != "" && fileName_R != "")
		{

			//get images from files, do remapping, get disparity image and show it
			Mat leftImage = imread(fileName_L.toStdString().c_str());
			Mat rightImage = imread(fileName_R.toStdString().c_str());

			//if this stereo image is made of RGB and NIR images, resize and crop the RGB image
			if(isRGB_NIR_Stereo)
			{
				rightImage = camCalib.resizeAndCropRGBImg(rightImage);

				//also make sobel images and max contrst width
//                Sobel(leftImage, leftImage, -1, 1, 1, 3);
//                Sobel(rightImage, rightImage, -1, 1, 1, 3);
			}


			Mat imgUL, imgUR;
			camCalib.undistortAndRemapStereoImages(leftImage, rightImage, imgUL, imgUR);

			//save images -> use left fileName as base name and append "L" and "R"
			QString fileNameLeft = fileName_L;
			QString fileNameRight = fileName_L;
			nmLeft = (fileNameLeft.remove(".png").remove(".jpg")).append("_remap_L.png");
			nmRight = (fileNameRight.remove(".png").remove(".jpg")).append("_remap_R.png");
			imwrite(nmLeft.toStdString().c_str(), imgUL);
			imwrite(nmRight.toStdString().c_str(), imgUR);
		}
	}
	QMessageBox::information(this, "Save successful", "Remapped images have been saved", QMessageBox::Ok);

	//show and remmeber last image
	lastStereoFileL = imread(nmLeft.toStdString().c_str(), CV_LOAD_IMAGE_COLOR);
	lastStereoFileR = imread(nmRight.toStdString().c_str(), CV_LOAD_IMAGE_COLOR);
	makeDisparityImage(lastStereoFileL, lastStereoFileR);
	lastStereoFileNameL = nmLeft;
	lastStereoFileNameR = nmRight;
}


void MainWindow::on_btn_stereoVision_released()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Select LEFT image"), lastDir, tr(IMGTYPES));
	if(fileName == "") return;
	lastStereoFileNameL = fileName;
	lastDir = QFileInfo(fileName).path();

	fileName = QFileDialog::getOpenFileName(this, tr("Select RIGHT image"), lastDir, tr(IMGTYPES));
	if(fileName == "") return;
	lastStereoFileNameR = fileName;
	lastDir = QFileInfo(fileName).path();

	lastStereoFileL = imread(lastStereoFileNameL.toStdString().c_str(), CV_LOAD_IMAGE_COLOR);
	lastStereoFileR = imread(lastStereoFileNameR.toStdString().c_str(), CV_LOAD_IMAGE_COLOR);

	makeDisparityImage(lastStereoFileL, lastStereoFileR);
}

void MainWindow::on_pushButton_stereoAgain_released()
{
	if(lastStereoFileNameL != "" && lastStereoFileNameR != "")
	{
		makeDisparityImage(lastStereoFileL, lastStereoFileR);
	}
}

void MainWindow::on_btn_undistLOAD_released()
{
	QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Select calibration file(s)"), lastDir, tr("*.rgbnirstcal *.stereocal *.singlecal"));
	if(fileNames.length() == 0){ return; }
	lastDir =QFileInfo(fileNames.first()).path();

	camCalib.loadCalibrationFile(fileNames);

	QString msg = "Camera calibration file has been loaded for ";
	if(camCalib.isCalibrated_cam()){ msg +=  "Single Camera."; }
	else if(camCalib.isCalibrated_stereo()){ msg +=  "Stereo Cam Pair."; }
	else if(camCalib.isCalibrated_goldeye()){ msg +=  "Goldeye."; }
	QMessageBox::information(this, "Calibration Data", msg, QMessageBox::Ok);
}



void MainWindow::on_btn_alignImgs_released()
{
	//get files
	QString fileName_L = QFileDialog::getOpenFileName(this, tr("Select LEFT image file"), lastDir, tr(IMGTYPES));
	if(fileName_L == ""){ return; }
	lastDir = QFileInfo(fileName_L).path();
	QString fileName_R = QFileDialog::getOpenFileName(this, tr("Select RIGHT image file"), lastDir, tr(IMGTYPES));
	if(fileName_R == ""){ return; }
	lastDir = QFileInfo(fileName_R).path();

	if(fileName_L == "" || fileName_R == ""){ return; }

	//get images from files
	Mat leftImage = imread(fileName_L.toStdString().c_str());
	Mat rightImage = imread(fileName_R.toStdString().c_str());

	//make transformation matrix from translation and rotation and warp perspetive
	Mat alignedRight = camCalib.alignImageByFeatures(leftImage, rightImage);

	//save algined image
	QString saveName = fileName_R.remove(".png").remove(".jpg").append("_aligned.png");
	imwrite(saveName.toStdString().c_str(), alignedRight);
	QMessageBox::information(this, "Success", "The aligned right image has been saved!", QMessageBox::Ok);
}


void MainWindow::on_btn_homogenMake_released()
{
	QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Select tar-files containing calibration images"), lastDir, tr("*.tar"));
	if(fileNames.count() == 0) return;
	lastDir = QFileInfo(fileNames.first()).path();

	camCalib.makeAndSaveHomogeneityMatrices(fileNames, lastDir);

	QMessageBox::information(this, "Homogeneity Calibration Successful", "Homogeneity calibration matrix has created and saved", QMessageBox::Ok);
}


void MainWindow::on_btn_homogenLoad_released()
{
	QString fileName = QFileDialog::getOpenFileName(this, "Select homogeneity calibration file", lastDir);
	if(fileName == ""){ return; }
	lastDir = QFileInfo(fileName).path();

	if(camCalib.loadHomogeneityMatrices(fileName))
	{
		QMessageBox::information(this, "Load Successful", "Homogeneity calibration file has been loaded", QMessageBox::Ok);
	}
}

void MainWindow::on_btn_homogenApply_released()
{
	QStringList fileNames = QFileDialog::getOpenFileNames(this, "Select multichannel images (*.tar)", lastDir, tr("*.tar"));
	if(fileNames.length() == 0){ return; }
	lastDir = QFileInfo(fileNames.first()).path();

	foreach(QString fileName, fileNames){ camCalib.applyHomogeneityMatrices(fileName); }

	QMessageBox::information(this, "Success", "Processed images have been stored in original folder", QMessageBox::Ok);
}


//BLOCK-MATCHING SLIDER:
void MainWindow::on_slider_prefilterSize_sliderMoved(int position)
{
	//make odd
	if(position % 2 == 0)
	{
		position++;
		ui->slider_prefilterSize->setValue(position);
	}
	ui->label_prefilterSize->setText( QString::number(position) );
	makeDisparityImage(lastStereoFileL, lastStereoFileR);
}

void MainWindow::on_slider_prefilterCAP_sliderMoved(int position)
{
	ui->label_prefilterCAP->setText( QString::number(position) );
	makeDisparityImage(lastStereoFileL, lastStereoFileR);
}

void MainWindow::on_slider_SADwindow_sliderMoved(int position)
{
	//make odd
	if(position % 2 == 0)
	{
		position++;
		ui->slider_SADwindow->setValue(position);
	}
	ui->label_SADwindow->setText( QString::number(position) );
	makeDisparityImage(lastStereoFileL, lastStereoFileR);
}

void MainWindow::on_slider_minDisp_sliderMoved(int position)
{
	ui->label_minDisp->setText( QString::number(position) );
	makeDisparityImage(lastStereoFileL, lastStereoFileR);
}

void MainWindow::on_slider_dispRange_sliderMoved(int position)
{
	//make multiple of 16
	int dispTemp = position % 16;
	if(dispTemp != 0)
	{
		position -= dispTemp;
		ui->slider_dispRange->setValue(position);
	}
	ui->label_dispRange->setText( QString::number(position) );
	makeDisparityImage(lastStereoFileL, lastStereoFileR);
}

void MainWindow::on_slider_textureThresh_sliderMoved(int position)
{
	ui->label_textureThresh->setText( QString::number(position) );
	makeDisparityImage(lastStereoFileL, lastStereoFileR);
}

void MainWindow::on_slider_speckleWindow_sliderMoved(int position)
{
	//make odd
	if(position % 2 == 0)
	{
		position++;
		ui->slider_speckleWindow->setValue(position);
	}
	ui->label_speckleWindow->setText( QString::number(position) );
	makeDisparityImage(lastStereoFileL, lastStereoFileR);
}

void MainWindow::on_slider_speckleRange_sliderMoved(int position)
{
	ui->label_speckleRange->setText( QString::number(position) );
	makeDisparityImage(lastStereoFileL, lastStereoFileR);
}

void MainWindow::on_slider_uniqueness_sliderMoved(int position)
{
	ui->label_uniqueness->setText( QString::number(position) );
	makeDisparityImage(lastStereoFileL, lastStereoFileR);
}

void MainWindow::on_btn_saveParams_released()
{
	QString fileName = QFileDialog::getSaveFileName(this, "Select file for BM parameters", lastDir);
	if(fileName == ""){ return; }

	QFile file(fileName.remove(".bmparams").append(".bmparams"));
	if (!file.open(QIODevice::WriteOnly))
	{
		 QMessageBox::information(this, tr("Unable to open file"), file.errorString());
		 return;
	}

	QTextStream out(&file);
	out << "mindDisp:" << ui->slider_minDisp->value() << "\n" <<
			"dispRange:" << ui->slider_dispRange->value() << "\n" <<
			"SADwindow:" << ui->slider_SADwindow->value() << "\n" <<
			"prefilterSize:" << ui->slider_prefilterSize->value() << "\n" <<
			"prefilterCAP:" << ui->slider_prefilterCAP->value() << "\n" <<
			"textureThresh:" << ui->slider_textureThresh->value() << "\n" <<
			"uniquenessRatio:" << ui->slider_uniqueness->value() << "\n" <<
			"speckleWindow:" << ui->slider_speckleWindow->value() << "\n" <<
			"speckleRange:" << ui->slider_speckleRange->value();
	file.close();
}

void MainWindow::on_btn_loadParams_released()
{
	QString fileName = QFileDialog::getOpenFileName(this, "Select file for BM parameters", lastDir, tr("*.bmparams"));
	if(fileName == ""){ return; }

	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly))
	{
		 QMessageBox::information(this, tr("Unable to open file"), file.errorString());
		 return;
	}

	QTextStream in(&file);
	int v1, v2, v3, v4, v5, v6, v7, v8, v9;
	if(!in.atEnd()){ v1 = in.readLine().split(":").last().toInt(); }
	if(!in.atEnd()){ v2 = in.readLine().split(":").last().toInt(); }
	if(!in.atEnd()){ v3 = in.readLine().split(":").last().toInt(); }
	if(!in.atEnd()){ v4 = in.readLine().split(":").last().toInt(); }
	if(!in.atEnd()){ v5 = in.readLine().split(":").last().toInt(); }
	if(!in.atEnd()){ v6 = in.readLine().split(":").last().toInt(); }
	if(!in.atEnd()){ v7 = in.readLine().split(":").last().toInt(); }
	if(!in.atEnd()){ v8 = in.readLine().split(":").last().toInt(); }
	if(!in.atEnd()){ v9 = in.readLine().split(":").last().toInt(); }
	file.close();


	ui->slider_minDisp->setValue(v1);
	ui->label_minDisp->setText(QString::number(v1));

	ui->slider_dispRange->setValue(v2);
	ui->label_dispRange->setText(QString::number(v2));

	ui->slider_SADwindow->setValue(v3);
	ui->label_SADwindow->setText(QString::number(v3));

	ui->slider_prefilterSize->setValue(v4);
	ui->label_prefilterSize->setText(QString::number(v4));

	ui->slider_prefilterCAP->setValue(v5);
	ui->label_prefilterCAP->setText(QString::number(v5));

	ui->slider_textureThresh->setValue(v6);
	ui->label_textureThresh->setText(QString::number(v6));

	ui->slider_uniqueness->setValue(v7);
	ui->label_uniqueness->setText(QString::number(v7));

	ui->slider_speckleWindow->setValue(v8);
	ui->label_speckleWindow->setText(QString::number(v8));

	ui->slider_speckleRange->setValue(v9);
	ui->label_speckleRange->setText(QString::number(v9));
}

void MainWindow::on_checkBox_useSGBM_clicked()
{
	bool enabled = !ui->checkBox_useSGBM->isChecked();
	ui->slider_prefilterSize->setEnabled(enabled);
	ui->label_prefilterSize->setEnabled(enabled);
	ui->label_preFilterSizeLabel->setEnabled(enabled);

	ui->slider_textureThresh->setEnabled(enabled);
	ui->label_textureThresh->setEnabled(enabled);
	ui->label_textureThresholdLabel->setEnabled(enabled);

	makeDisparityImage(lastStereoFileL, lastStereoFileR);

}

void MainWindow::on_btn_makeImgPatches_released()
{
	QStringList fileNames = QFileDialog::getOpenFileNames(this, "Select Files", lastDir, IMGTYPES);
	if(fileNames.count() == 0){ return; }
	lastDir = QFileInfo(fileNames.first()).path();

	bool ok;
//    int divide = QInputDialog::getInt(this, "Pieces", "Divide by?", 4, 0, 32, 2, &ok);
//    if(!ok){ return; }

	int patchSz = QInputDialog::getInt(this, "PatchSize", "What is the patch size?", 46, 0, 256, 2, &ok);
	if(!ok){ return; }

	ImagePreprocessor preproc;

	foreach(QString fileName, fileNames)
	{
		preproc.cutImageIn4Pieces(patchSz, fileName);
	}

//    QString outDir = lastDir + "/patches";
//    QDir dir;
//    dir.mkdir(outDir);

//    ImagePreprocessor preproc;

//    int imgIndex = 0;
//    int imgTotal = fileNames.count();
//    foreach(QString fileName, fileNames)
//    {
//        if(fileName.contains("_nir")){ imgTotal--; }
//        else if(fileName.contains("_label")){ imgTotal--; }
//        else if(fileName.contains("_depth")){ imgTotal--; }
//    }

//    //prepare text stream to save all file names
//    QFile outFile(outDir + "/images.txt");
//    outFile.open(QFile::WriteOnly);
//    QTextStream txtOut(&outFile);

//    foreach(QString fileName, fileNames)
//    {
//        //if a label file or nir file, jump to next
//        if(fileName.contains("_labels") || fileName.contains("_nir") || fileName.contains("_depth"))
//        {
//            continue;
//        }

//        //read all images (rgb + nir + depth) and add to list
//        QList<Mat> imgs;
//        QString fileNameNoExtension = fileName;
//        fileNameNoExtension.remove(QRegExp(".png|.jpg"));
//        QString onlyFileName = fileNameNoExtension.split("/").last();
//        imgs.append( imread(fileName.toStdString().c_str()) );

//        //NIR
//        if(QFile::exists(fileNameNoExtension + "_nir.jpg"))
//        {
//            imgs.append( imread((fileNameNoExtension + "_nir.jpg").toStdString().c_str()) );
//        }
//        else if(QFile::exists(fileNameNoExtension + "_nir.png"))
//        {
//            imgs.append( imread((fileNameNoExtension + "_nir.png").toStdString().c_str()) );
//        }

//        //Depth
//        if(QFile::exists(fileNameNoExtension + "_depth.jpg"))
//        {
//            imgs.append( imread((fileNameNoExtension + "_depth.jpg").toStdString().c_str(), CV_LOAD_IMAGE_GRAYSCALE) );
//        }
//        else if(QFile::exists(fileNameNoExtension + "_depth.png"))
//        {
//            imgs.append( imread((fileNameNoExtension + "_depth.png").toStdString().c_str(), CV_LOAD_IMAGE_GRAYSCALE) );
//        }

//        //read lable image
//        QString labelFileName = (fileNameNoExtension + "_labels.png");
//        Mat labelImg = imread(labelFileName.toStdString().c_str());

//        if(labelImg.empty())
//        {
//            QMessageBox::information(this, "Label Image for" + fileNameNoExtension + " not in folder",
//                                     "Label image must have same name + _labels.png!", QMessageBox::Ok);
//            return;
//        }

//        QTime timer;
//        timer.start();
//        preproc.makeImagePatches(imgs, labelImg, 9, 46, 3, onlyFileName, outDir, ++imgIndex, imgTotal);
//        qDebug() << timer.elapsed();

//        //write file name in output file
//        txtOut << onlyFileName << "/" << onlyFileName << "\n";
//    }

}

void MainWindow::on_btn_makeTrainVal_released()
{
	QStringList fileNames = QFileDialog::getOpenFileNames(this, "Select image files", lastDir, IMGTYPES);
	if(fileNames.count() == 0){ return; }
	lastDir = QFileInfo(fileNames.first()).path();

	bool ok;
	int trainPercentage = QInputDialog::getInt(this, "Set percentage of training data", "Training data in percent:",
											   80, 0, 100, 1, &ok);
	if( !ok || !(trainPercentage > 0 && trainPercentage < 100) ){ return; }


	QFile outfileTrain(lastDir + "/train.txt");
	outfileTrain.open(QFile::WriteOnly);
	QTextStream outTrain(&outfileTrain);
	QFile outfileVal(lastDir + "/val.txt");
	outfileVal.open(QFile::WriteOnly);
	QTextStream outVal(&outfileVal);

	int cnt = 0;
	int max = fileNames.count();

	QProgressDialog progress("Making training and validation label text files", "cancel", 0, max);
	progress.setValue(0);
	progress.setMinimumWidth(450);
	progress.setMinimumDuration(100);
	progress.setWindowModality(Qt::WindowModal);

	//count pure filenames without NIR, Depth
	int filesTotal = 0;
	foreach(QString fileName, fileNames)
	{
		if(!(fileName.contains("_depth") || fileName.contains("_nir") || fileName.contains("_labels")) )
		{ filesTotal++; }
	}

	int trainCnt = 0;
	int trainMax = (filesTotal * (float)trainPercentage / 100.0) + 0.5;
	int testCnt = 0;
	int testMax = filesTotal - trainMax;

	while(fileNames.size() != 0)
	{
		int index = rand() % fileNames.size();
		QString fileName = fileNames[index].remove(".jpg");
		if(!(fileName.contains("_depth") || fileName.contains("_nir") || fileName.contains("_labels")) )
		{
			if( (rand() % 100) < trainPercentage) //add to training text file
			{
				if(trainCnt++ < trainMax){ outTrain << fileName << "\n"; }
				else{ outVal << fileName << "\n"; }
			}
			else //add to validation text file
			{
				if(testCnt++ < testMax){ outVal << fileName << "\n"; }
				else{ outTrain << fileName << "\n"; }
			}
		}

		fileNames.removeAt(index);

		progress.setValue(cnt++);
		if(progress.wasCanceled()){ return; }
	}

//    QStringList fileNames = QFileDialog::getOpenFileNames(this, "Select label text files", lastDir, "*.txt");
//    if(fileNames.count() == 0){ return; }
//    lastDir = QFileInfo(fileNames.first()).path();

//    bool ok;
//    int trainPercentage = QInputDialog::getInt(this, "Set percentage of training data", "Training data in percent:",
//                                               80, 0, 100, 1, &ok);
//    if( !ok || !(trainPercentage > 0 && trainPercentage < 100) ){ return; }


//    QFile outfileTrain(lastDir + "/train.txt");
//    outfileTrain.open(QFile::WriteOnly);
//    QTextStream outTrain(&outfileTrain);
//    QFile outfileVal(lastDir + "/val.txt");
//    outfileVal.open(QFile::WriteOnly);
//    QTextStream outVal(&outfileVal);

////    int trainAmount = fileNames.count() * trainPercentage / 100;
//    int cnt = 0;
//    int max = fileNames.count();

//    QProgressDialog progress("Making training and validation label files", "cancel", 0, max);
//    progress.setValue(0);
//    progress.setMinimumWidth(450);
//    progress.setMinimumDuration(100);
//    progress.setWindowModality(Qt::WindowModal);

//    foreach(QString fileName, fileNames)
//    {
//        QFile infile(fileName);
//        infile.open(QFile::ReadOnly);
//        QTextStream in(&infile);

//        //add  a line to either training or validation labels depending on random number and probability
//        while(!in.atEnd())
//        {
//            if( (rand() % 100) < trainPercentage) //add to training text file
//            {
//                outTrain << in.readLine() << "\n";
//            }
//            else //add to validation text file
//            {
//                outVal << in.readLine() << "\n";
//            }
//        }
//        infile.close();

//        progress.setValue(cnt++);
//        if(progress.wasCanceled()){ return; }
//    }

//    while(fileNames.count() != 0)
//    {
//        int randomIndex = rand() % fileNames.count();
//        QFile infile(fileNames.at(randomIndex));
//        infile.open(QFile::ReadOnly);
//        QTextStream in(&infile);

//        if(cnt < trainAmount) //add to training text file
//        {
//            while(!in.atEnd()){ outTrain << in.readLine() << "\n"; }
//        }
//        else //add to validation text file
//        {
//            while(!in.atEnd()){ outVal << in.readLine() << "\n"; }
//        }

//        fileNames.removeAt(randomIndex);
//        progress.setValue(cnt++);
//        if(progress.wasCanceled()){ return; }
//    }

	outfileTrain.close();
	outfileVal.close();
	progress.setValue(max);
}


/*****************************************************************************************
 * JUST FOR TEST / DEBUGGING / ETC PURPOSES
 *****************************************************************************************/
void MainWindow::on_pushButton_test_released()
{
	//////////////////////////////////////////////////////////////
	// TRANSFORM STANFORD BACKGROUND DATASET LABELS INTO PNG
	//////////////////////////////////////////////////////////////
	QString fileName = QFileDialog::getOpenFileName(this, "Select File", lastDir, "horizons.txt");
	if(fileName == ""){ return; }
	lastDir = QFileInfo(fileName).path();

	QFile f(fileName);
	f.open(QFile::ReadOnly);
	QTextStream in(&f);

	QDir dir;
	dir.mkdir(lastDir + "/labelsImgs");

	while(!in.atEnd())
	{
		QStringList sl = in.readLine().split(" ");
		QString nm = sl.at(0);
		int w = sl.at(1).toInt();
		int h = sl.at(2).toInt();

		Mat img(h, w, CV_8UC1);

		QFile lblf(lastDir + "/labels/" + nm + ".regions.txt");
		lblf.open(QFile::ReadOnly);
		QTextStream lblin(&lblf);

		for (int y = 0; y < h; ++y)
		{
			QStringList line = lblin.readLine().split(" ");
			for (int x = 0; x < w; ++x)
			{
				uchar val = line.at(x).toInt();
				if(val < 0){ img.at<uchar>(y,x) = 255; }//in original, <0 is unknown, in our case 255 is unknown
				else{ img.at<uchar>(y,x) = val; }
			}
		}

		imwrite((lastDir + "/labelsImgs/" + nm + "_labels.png").toStdString(), img);
	}

	f.close();
	//////////////////////////////////////////////////////////////
	// endof TRANSFORM STANFORD BACKGROUND DATASET LABELS INTO PNG
	//////////////////////////////////////////////////////////////

//	QString fileName = QFileDialog::getOpenFileName(this, "Select File", lastDir, IMGTYPES);
//	if(fileName == ""){ return; }
//	lastDir = QFileInfo(fileName).path();
//	ImagePreprocessor preproc;
//	Mat a = imread(fileName.toStdString().c_str(), IMREAD_ANYDEPTH | IMREAD_ANYCOLOR);
//	a.convertTo(a, CV_8UC1, 255.0/2047.0);
//	imshow("just loaded", a);
//	qDebug() << a.type();

//	qDebug() << CV_16UC1;
//	qDebug() << CV_8UC1;
//	qDebug() << CV_8UC3;

//    vector<Mat> imgs;
//    imgs.push_back(imread("1.png", IMREAD_GRAYSCALE));
//    imgs.push_back(imread("2.png", IMREAD_GRAYSCALE));
//    imgs.push_back(imread("3.png", IMREAD_GRAYSCALE));
//    imgs.push_back(imread("4.png", IMREAD_GRAYSCALE));
//    imgs.push_back(imread("5.png", IMREAD_GRAYSCALE));
//    imgs.push_back(imread("6.png", IMREAD_GRAYSCALE));
//    imgs.push_back(imread("7.png", IMREAD_GRAYSCALE));

//    Mat multich;
//    cv::merge(imgs, multich);

//    cv::FileStorage fs("mergeALL", FileStorage::WRITE);
//    fs << "M1" << multich;
//    fs.release();


//    //////////////////////////////////////////////////////////////
//    // TEST MAKE IMAGE PATCHES
//    //////////////////////////////////////////////////////////////

//    Mat labelImg; //fake label FOR NOW
//    cvtColor(a, labelImg, CV_BGR2GRAY);

//    QString outDir = lastDir+"/test_patches";
//    QDir dir;
//    dir.mkdir(outDir);
//    preproc.makeImagePatches(a, labelImg, 15, 46, "test", outDir);
//    //////////////////////////////////////////////////////////////
//    // endof TEST MAKE IMAGE PATCHES
//    //////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////
	// TEST LOCAL NORMALIZATION
	//////////////////////////////////////////////////////////////
	//split image in its channels
//    vector<Mat> channels(a.channels());
//    cv::split(a, channels);

//    //normalize channels separately
//    vector<Mat> channelsNorm;
//    foreach(Mat img, channels)
//    {
//        Mat aNorm = preproc.NormalizeLocally(img, 15);
//        channelsNorm.push_back(aNorm);
//    }

//    //merge channels and do normalization on multichannel image for comparison
//    Mat aNorm;
//    cv::merge(channelsNorm, aNorm);
//    vector<Mat> anorms;
//    anorms.push_back(preproc.NormalizeLocally(a, 15, 45));
//    anorms.push_back(preproc.NormalizeLocally(a, 15, 91));
//    anorms.push_back(preproc.NormalizeLocally(a, 15, 121));
//    anorms.push_back(preproc.NormalizeLocally(a, 15, 151));

//    anorms.push_back(preproc.NormalizeLocally(a, 9, 45));
//    anorms.push_back(preproc.NormalizeLocally(a, 9, 91));
//    anorms.push_back(preproc.NormalizeLocally(a, 9, 121));
//    anorms.push_back(preproc.NormalizeLocally(a, 9, 151));

//    anorms.push_back(preproc.NormalizeLocally(a, 9, 27));
//    anorms.push_back(preproc.NormalizeLocally(a, 9, 55));
//    anorms.push_back(preproc.NormalizeLocally(a, 9, 73));
//    anorms.push_back(preproc.NormalizeLocally(a, 9, 91));

//    int cnt = 0;
//    foreach(Mat a, anorms)
//    {
//        QString s = "TestNorm_" + QString::number(cnt++) + ".png";
//        imwrite(s.toStdString().c_str() , a);
//    }

	//normalize to [0,1] in order to display
//    cv::normalize(aNorm, aNorm, 0, 1, NORM_MINMAX);
//    cv::normalize(aNorm2, aNorm2, 0, 1, NORM_MINMAX);

	//print one channel to console - because of normalization, everything should be close to 128
//    vector<Mat> temp(a.channels());
//    cv::split(aNorm2, temp);
//    Helper::Print1ChMatrixToConsole(temp.at(0));


	//save to disk in order to compare images in GIMP to see if separate channel normalization
	//and multichannel normalization do exactly the same thing.
//    Mat aNorm_8bit, aNorm2_8bit;
//    aNorm.convertTo(aNorm_8bit, CV_8U, 255);
//    aNorm2.convertTo(aNorm2_8bit, CV_8U, 255);
//    imwrite("test1.png", aNorm_8bit);
//    imwrite("test2.png", aNorm2_8bit);
	//////////////////////////////////////////////////////////////
	// endof TEST LOCAL NORMALIZATION
	//////////////////////////////////////////////////////////////


//    //////////////////////////////////////////////////////////////
//    // ADAPT IMAGE NET LABEL DATA
//    //////////////////////////////////////////////////////////////
//    //read downloaded ImageNet folders
//    QStringList allDirs, allFiles;
//    QDir dir(QFileDialog::getExistingDirectory(this, "Get Dir", lastDir));
//    QStringList dList = dir.entryList();
//    foreach(QString s, dList)
//    {
//        if(s.length() < 3) continue;
//        allDirs.append(dir.absoluteFilePath(s));
//    }
//    foreach(QString s, allDirs)
//    {
//        QDir dir2(s);
//        QStringList fList = dir2.entryList();
//        foreach(QString ss, fList)
//        {
//            QStringList l = dir2.absoluteFilePath(ss).split("/");
//            QString nm = l.at(l.length()-2) + "/" + l.last();
//            allFiles.append(nm);
//        }
//    }

//    QString origTxt = QFileDialog::getOpenFileName(this, "Select file", lastDir);
//    QFile f1(origTxt);
//    f1.open(QIODevice::ReadOnly);
//    QFile f2(origTxt.remove(".txt").append("2.txt"));
//    f2.open(QIODevice::WriteOnly);
//    QTextStream txtStreamIn(&f1);
//    QTextStream txtStreamOUT(&f2);
//    bool goOn = true;
//    int i = 0;
//    while(!txtStreamIn.atEnd() && goOn)
//    {
//        QString s = txtStreamIn.readLine();
//        QStringList temp = s.split(" ");


//        //search for file name and if found, write to out file with class number
//        int i = 0;
//        bool found = false;
//        foreach(QString file, allFiles)
//        {
//            if(file == temp.first())
//            {
//                txtStreamOUT << s << "\n";
////                if(++i > 100) goOn = false;
//                found = true;
//                continue;
//            }
//            i++;
//        }

////        if(found){ allFiles.removeAt(i); }
//    }
//    f1.close(); f2.close();
//    //////////////////////////////////////////////////////////////
//    // endof ADAPT IMAGE NET LABEL DATA
//    //////////////////////////////////////////////////////////////
}





void MainWindow::on_btn_makeHomographyMatrix_released()
{
	QString fileNameA = QFileDialog::getOpenFileName(this, "Select image A", lastDir, IMGTYPES);
	if(fileNameA == ""){ return; }
	lastDir = QFileInfo(fileNameA).path();
	QString fileNameB = QFileDialog::getOpenFileName(this, "Select image B", lastDir, IMGTYPES);
	if(fileNameB == ""){ return; }
	lastDir = QFileInfo(fileNameB).path();

	QStringList params = ui->lineEdit_calibSingleInfo->text().split(",");
	camCalib.makeHomographyMatrix(fileNameA, fileNameB, Size(params[0].toInt(), params[1].toInt()));
}

void MainWindow::on_btn_registerImages_released()
{
	QString fileNameA = QFileDialog::getOpenFileName(this, "Select image A to be registered to image B", lastDir, IMGTYPES);
	if(fileNameA == ""){ return; }
	lastDir = QFileInfo(fileNameA).path();
	QString fileNameMat = QFileDialog::getOpenFileName(this, "Select homography matrix from A to B", lastDir, "*.mat");
	if(fileNameMat == ""){ return; }
	lastDir = QFileInfo(fileNameMat).path();

	Mat a = imread(fileNameA.toStdString());
	Mat b = imread(fileNameA.toStdString());
	FileStorage fs(fileNameMat.toStdString(), CV_STORAGE_READ);
	Mat h, aa;
	fs["homography"] >> h;
	warpPerspective(a, aa, h, a.size());
	fs.release();

	imwrite( (fileNameA.remove(".png") + "_registered.png").toStdString(), aa );
}
