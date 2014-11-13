#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "seeds/seeds2.h"
#include "seeds/seedsHelper.h"
#include "slic/slic.h"
#include "segmentfelsenzwalb/segmentation.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	lastDir(QDir::homePath()),
	colorMap(256, vector<int>(3)),
	objectMap(256)
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

void MainWindow::on_btn_makeLabelImgs_released()
{
	QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Select XML files"), lastDir, tr("*.xml"));
	if(fileNames.length() == 0) return;
	QDir path = QFileInfo(fileNames.first()).path();
	lastDir = path.absolutePath();

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

void MainWindow::on_pushButton_stereoAgain_released()
{
	makeDisparityImage(lastStereoFileNameL, lastStereoFileNameR);
}


void MainWindow::makeSurfFeatures(QString fileName)
{
    Mat img, features;
    img = imread(fileName.toStdString().c_str());

    SURF surf(5000.);

}
void MainWindow::on_btn_surf_released()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select image file"), lastDir, tr("*.jpg *.png"));
    if(fileName == "") return;
    lastDir = QFileInfo(fileName).path();
    lastSurfFileName = fileName;
    makeSurfFeatures(fileName);
}

void MainWindow::on_pushButton_surfAgain_released()
{
    makeSurfFeatures(lastSurfFileName);
}
