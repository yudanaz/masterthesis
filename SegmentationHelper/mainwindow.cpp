#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "seeds/seeds2.h"
#include "seeds/seedsHelper.h"
#include "slic/slic.h"
#include "segmentfelsenzwalb/segmentation.h"

#define WHITE Scalar(255, 255, 255)
#define PINK Scalar(255, 51, 153)

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	lastDir(QDir::homePath()),
	colorMap(256, vector<int>(3)),
    objectMap(256),
    cameraCalibrated(false),
    stereoCalibrated(false)
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
        QList<Point> polygon;
        int x = 0;
        int colorIndex;

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
                    polygon.clear();
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
                        polygon.append(p);
                    }
                }

            }
            else if (xml.isEndElement())
            {
                //when object tag is closed, draw the polygon in color according to object type
                if(xml.name().toString() == "object")
                {
                    collectingPoints = false;

                    Point points[1][polygon.count()];

                    for(int i = 0; i < polygon.count(); ++i)
                    {
                        points[0][i] = polygon[i];
                    }

                    const Point* ppt[1] = { points[0] };
                    int npt[] = { polygon.length() };

                    //paint labels in color and grayscale images
                    if(colorIndex != 999) //if matching label has been found
                    {
                        Scalar color(colorMap[colorIndex][0], colorMap[colorIndex][1], colorMap[colorIndex][2]);
                        Scalar gray(1 + colorIndex);
                        fillPoly( colorImg, ppt, npt, 1, color, 8 );
                        fillPoly( grayImg, ppt, npt, 1, gray, 8 );
                    }
                }
            }

            xml.readNext();
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

	//display the number of superpixels created with this settings
	QString nrOfSuperpxs = QString::number((img.cols * img.rows) / (pow(seedW, seedLevels-1) * pow(seedH, seedLevels-1)));
	ui->label_nrOfSuperpxs->setText("= " + nrOfSuperpxs + " superpixels");

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
//	Mat labels;
//	labels = seeds.getLabelsAsMat();
//	imshow("Labels", labels);
//	imshow("Original", img);

	int sz = 3*width*height;

//	UINT* output_buff = new UINT[sz];
//	for (int i = 0; i<sz; i++) output_buff[i] = 0;

	UINT* imgUINT = seeds.getUINT(img);
	DrawContoursAroundSegments(imgUINT, seeds.get_labels(), width, height, 0xFF1493, true);//0xff0000 draws white contours
	Mat labelsOverlayed = seeds.getMat(imgUINT);
	imshow("Superpixels", labelsOverlayed);

	//save label names
//	QString fileNameOnly = fileName.split("/").last().split(".")[0];
//	QString labelsFileName = lastDir + "/" + fileNameOnly + "labels.txt";
//	seeds.SaveLabels_Text(labelsFileName.toStdString().c_str());
}

void MainWindow::on_btn_slic_released()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Select image file"), lastDir, tr("*.jpg *.png"));
	if(fileName == "") return;
	lastDir = QFileInfo(fileName).path();
	lastSlicFilename = fileName;
	makeSlicSuperpixels(fileName);
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

void MainWindow::makeDisparityImage(QString fileNameL, QString fileNameR)
{
    int nrDisp, blockSize;
    QStringList info = ui->lineEdit_stereoInfo->text().split(",");
    if(info.length() == 2)
    {
        nrDisp = info.at(0).toInt()*16;
        blockSize = info.at(1).toInt();
    }
    else
    {
        nrDisp = 16; blockSize = 15;
    }
    if(blockSize % 2 == 0) { blockSize++; } //make odd if even
    if(blockSize < 5) { blockSize += 5-blockSize; } //make >= 5

    //load L and R images as grayscale
    Mat left, right;
    left = imread(fileNameL.toStdString().c_str(), CV_LOAD_IMAGE_GRAYSCALE);
    right = imread(fileNameR.toStdString().c_str(), CV_LOAD_IMAGE_GRAYSCALE);

    //compute disparities
    Mat disp(left.rows, left.cols, CV_16SC1);
    StereoBM sbm(StereoBM::BASIC_PRESET, nrDisp, blockSize);
    sbm(left, right, disp, CV_16S);

    //normalize and display
    double minVal, maxVal;
    minMaxLoc(disp, &minVal, &maxVal); //find minimum and maximum intensities
    Mat disp2;
    disp.convertTo(disp2,CV_8U, 255.0/(maxVal - minVal), -minVal * 255.0/(maxVal - minVal));

    imshow("left", left);
    imshow("right", right);
    imshow("image", disp2);
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

void MainWindow::calibrateSingleCamera(QStringList calibImgFiles, int chessboard_width, int chessboard_height)
{
    bool success = false;
    int nrOfChessboards;
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
    int cnt = 0;
    foreach(QString fileName, calibImgFiles)
    {
        vector<Point2f> corners;
        Mat img, imgGray;
        img = imread(fileName.toStdString().c_str());
        cvtColor(img, imgGray, CV_BGR2GRAY);
        success = findChessboardCorners(imgGray, chessboard_sz, corners,
                                        CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);

        if(success)
        {
            imagePoints.push_back(corners);
            objectPoints.push_back(obj);
            if(cnt++ == 0) { imgSize = img.size(); }
        }
    }

    //calibrate the camera and store matrizes
    camMatrix = Mat(3, 3, CV_32FC1);
    camMatrix.at<float>(0, 0) = 1;
    camMatrix.at<float>(1, 1) = 1;
    vector<Mat> rvecs, tvecs;

    calibrateCamera(objectPoints, imagePoints, imgSize, camMatrix, distCoeff, rvecs, tvecs);

    QMessageBox::information(NULL, "Calibration successful", "Camera has been calibrated", QMessageBox::Ok);
}

void MainWindow::undistortSingleImage(QString fileName)
{
    Mat img, imgUndist;
    img = imread(fileName.toStdString().c_str());
    undistort(img, imgUndist, camMatrix, distCoeff);

    namedWindow("original", CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO);
    namedWindow("undistorted", CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO);
    imshow("original", img);
    imshow("undistorted", imgUndist);
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
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select image file"), lastDir, tr("*.jpg *.png"));
    if(fileName == "") return;
    lastDir = QFileInfo(fileName).path();
    lastSeedsFilename = fileName;
    makeSeedsSuperpixels(fileName);
}

void MainWindow::on_pushButton_seedAgain_released()
{
    if(lastSeedsFilename != "") { makeSeedsSuperpixels(lastSeedsFilename); }
}

void MainWindow::on_pushButton_slicAgain_released()
{
    if(lastSlicFilename != "") { makeSlicSuperpixels(lastSlicFilename); }
}

void MainWindow::on_btn_felsenzwalb_released()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select image file"), lastDir, tr("*.jpg *.png"));
    if(fileName == "") return;
    lastDir = QFileInfo(fileName).path();
    lastFelsenzwalbFilename = fileName;
    makeFelsenzwalbSuperpixels(fileName);
}

void MainWindow::on_pushButton_felsenzwalbAgain_released()
{
	if(lastFelsenzwalbFilename != "") { makeFelsenzwalbSuperpixels(lastFelsenzwalbFilename); }
}

void MainWindow::on_btn_stereoVision_released()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Select LEFT image"), lastDir, tr("*.jpg *.png"));
	if(fileName == "") return;
	lastStereoFileNameL = fileName;
	lastDir = QFileInfo(fileName).path();
//    fileName = QFileDialog::getOpenFileName(this, tr("Select RIGHT image"), lastDir, tr("*.jpg *.png"));
//	if(fileName == "") return;

	//right image should end with "R" where left img ended with "L"
	QStringList sl = fileName.split(".");
	lastStereoFileNameR = sl.at(sl.length()-2);
	lastStereoFileNameR = lastStereoFileNameR.remove("L") + "R." + sl.last();
	lastDir = QFileInfo(fileName).path();

	makeDisparityImage(lastStereoFileNameL, lastStereoFileNameR);
}

void MainWindow::on_pushButton_stereoAgain_released()
{
	makeDisparityImage(lastStereoFileNameL, lastStereoFileNameR);
}

void MainWindow::on_btn_surf_released()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Select image file"), lastDir, tr("*.jpg *.png"));
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
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Select calibration chessboard images"), lastDir, tr("*.jpg *.png"));
    if(fileNames.count() == 0) return;
    lastDir = QFileInfo(fileNames.first()).path();

    QStringList params = ui->lineEdit_calibSingleInfo->text().split(",");
    calibrateSingleCamera(fileNames, params.first().toInt(), params.last().toInt());
    cameraCalibrated = true;
}

void MainWindow::on_btn_undistSingle_released()
{
    if(!cameraCalibrated)
    {
        QMessageBox::information(NULL, "Error", "Camera has not been calibrated", QMessageBox::Ok);
        return;
    }

    QString fileName = QFileDialog::getOpenFileName(this, tr("Select image file to undistort"), lastDir, tr("*.jpg *.png"));
    lastDir =QFileInfo(fileName).path();
    undistortSingleImage(fileName);
}
