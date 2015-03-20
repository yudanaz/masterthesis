#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <unistd.h>
#include <QDebug>
#include "seeds/seeds2.h"
#include "seeds/seedsHelper.h"

int MainWindow::mouseX = -1;
int MainWindow::mouseY = -1;
bool MainWindow::drawing = false;
int MainWindow::width = 0;
int MainWindow::height = 0;

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	currentLabel(0)
{
	ui->setupUi(this);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_btn_load_released()
{
	//load orig image as color img
	Mat imgLoad = imread("img.png", IMREAD_GRAYSCALE);
	Mat img8bit;
	double min, max;
	minMaxLoc(imgLoad, &min, &max);
	imgLoad.convertTo(img8bit, CV_8UC1, 255/max);
	equalizeHist(img8bit, img8bit);
	cvtColor(img8bit, imgOrig, CV_GRAY2BGR);
	height = imgOrig.rows;
	width = imgOrig.cols;

	//init draw and labels image
	imgDraw = imgOrig.clone();
	imgLabels = Mat(imgOrig.size(), CV_8UC1);

	//make super pixel
	int NR_BINS = 5;
	int width = imgOrig.cols;
	int height = imgOrig.rows;
	int channels = imgOrig.channels();

	SEEDS seeds1(width, height, channels, NR_BINS);
	seeds1.initialize(2, 2, 2);
	seeds1.update_image_ycbcr(imgOrig);
	seeds1.iterate();
	imgSuperpx1 = seeds1.getLabelsAsMat().clone();

	SEEDS seeds2(width, height, channels, NR_BINS);
	seeds2.initialize(2, 2, 3);
	seeds2.update_image_ycbcr(imgOrig);
	seeds2.iterate();
	imgSuperpx2 = seeds2.getLabelsAsMat().clone();

	SEEDS seeds3(width, height, channels, NR_BINS);
	seeds3.initialize(3, 3, 3);
	seeds3.update_image_ycbcr(imgOrig);
	seeds3.iterate();
	imgSuperpx3 = seeds3.getLabelsAsMat().clone();

	SEEDS seeds4(width, height, channels, NR_BINS);
	seeds4.initialize(4, 4, 4);
	seeds4.update_image_ycbcr(imgOrig);
	seeds4.iterate();
	imgSuperpx4 = seeds4.getLabelsAsMat().clone();

	//make window and show
	destroyAllWindows();
	namedWindow("image", WINDOW_AUTOSIZE);
	setMouseCallback("image", mouseCallback, NULL);


	while(true)
	{
		drawAndShow();
		int key = cvWaitKey(30);
		if(key != -1){ interpretKey(key%256); }
	}
}

void MainWindow::drawAndShow()
{
	if(drawing)
	{
		bool drawNow = ui->radioButton_draw->isChecked();

		//get which size we're using
		int size = ui->lineEdit_superpixelSize->text().toInt();
		size = size < 1 ? 1 : size;
		size = size > 4 ? 4 : size;

		//get index of superpixel
		int currentSuperpx1 = imgSuperpx1.at<ushort>(mouseY, mouseX);
		int currentSuperpx2 = imgSuperpx2.at<ushort>(mouseY, mouseX);
		int currentSuperpx3 = imgSuperpx3.at<ushort>(mouseY, mouseX);
		int currentSuperpx4 = imgSuperpx4.at<ushort>(mouseY, mouseX);

		for (int y = 0; y < imgOrig.rows; ++y)
		{
			for (int x = 0; x < imgOrig.cols; ++x)
			{
				//draw or erase pixels that belong to same superpixel
				bool match = false;
				if(size == 1){ match = imgSuperpx1.at<ushort>(y,x) == currentSuperpx1; }
				else if(size == 2){ match = imgSuperpx2.at<ushort>(y,x) == currentSuperpx2; }
				else if(size == 3){ match = imgSuperpx3.at<ushort>(y,x) == currentSuperpx3; }
				else if(size == 4){ match = imgSuperpx4.at<ushort>(y,x) == currentSuperpx4; }

				if(match)
				{
					//check if it should be drawn or erased
					if(drawNow) //=empty
					{


						//make color
						int b = 200;
						int g = 50;
						int r = 200;
						Vec3b bgr(b,g,r);

						//get gray intensity to serve as new brightness for coloring
						int brightness = imgOrig.at<Vec3b>(y,x)[0];

						//convert color to HSV
						Vec3d hsv = BGR2HSV(bgr);
//						qDebug() << hsv[2];
						hsv[2] = brightness/255.0;
//						qDebug()  << brightness << " --> " << hsv[2];

						//convert back with new brightness
						Vec3b col = HSV2BGR(hsv);

//						int bb = 200;
//						int gg = 50;
//						int rr = 200;

//						bbb =

//						col[0] = bbb > 255 ? 255 : bbb;
//						col[0] = bbb <   0 ?   0 : bbb;
//						col[1] = ggg > 255 ? 255 : ggg;
//						col[1] = ggg <   0 ?   0 : ggg;
//						col[2] = rrr > 255 ? 255 : rrr;
//						col[2] = rrr <   0 ?   0 : rrr;

						imgDraw.at<Vec3b>(y,x) = col;
						imgLabels.at<uchar>(y,x) = currentLabel;
					}
					else//erase
					{
						Vec3b col = imgOrig.at<Vec3b>(y,x);
						imgDraw.at<Vec3b>(y,x) = col;
						imgLabels.at<uchar>(y,x) = 255; //"unknown" label
					}
				}
			}
		}
	}

	imshow("image", imgDraw);
}

void MainWindow::mouseCallback(int event, int x, int y, int flags, void* param)
{
	MainWindow *self = static_cast<MainWindow*>(param);
	self->doMouseCallback(event, x, y, flags);
}

void MainWindow::doMouseCallback(int event, int x, int y, int flags)
{
	if  ( event == EVENT_LBUTTONDOWN )
	{
		drawing = true;
	}
	else if ( event == EVENT_LBUTTONUP)
	{
		drawing = false;
	}
	else if ( event == EVENT_MOUSEMOVE )
	{
//		qDebug() << "Mouse move over the window - position (" << x << ", " << y << ")";
		mouseX = x > width-1 ? width-1 : x;
		mouseY = y > height-1 ? height-1 : y;
		if(mouseX < 0){ mouseX = 0; }
		if(mouseY < 0){ mouseY = 0; }
	}
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
	interpretKey(e->key());
}

void MainWindow::interpretKey(int key)
{
	int size = ui->lineEdit_superpixelSize->text().toInt();

	if(key == 'X' || key == 'x'){ size++; }
	else if (key == 'Y' || key == 'y'){ size--; }
	else if (key == 'A' || key == 'a' || key == 32)
	{
		bool b = ui->radioButton_draw->isChecked();
		ui->radioButton_draw->setChecked(!b);
		ui->radioButton_erase->setChecked(b);
	}

	size = size < 1 ? 1 : size;
	size = size > 4 ? 4 : size;
	ui->lineEdit_superpixelSize->setText(QString::number(size));
	ui->horizontalSlider->setValue(size);
}

Vec3d MainWindow::BGR2HSV(Vec3b bgr)
{
	float r = bgr[2]/255.0;
	float g = bgr[1]/255.0;
	float b = bgr[0]/255.0;
	float h = 0;
	float s = 0;
	float v = 0;

	float min, max, delta;
	min = MIN( MIN(r, g), b );
	max = MAX( MAX(r, g), b );
	v = max;				// v
	delta = max - min;
	if( max != 0 )
		s = delta / max;		// s
	else {
		// r = g = b = 0		// s = 0, v is undefined
		s = 0;
		h = -1;
		return Vec3b();
	}
	if( r == max )
		h = ( g - b ) / delta;		// between yellow & magenta
	else if( g == max )
		h = 2 + ( b - r ) / delta;	// between cyan & yellow
	else
		h = 4 + ( r - g ) / delta;	// between magenta & cyan
	h *= 60;				// degrees
	if( h < 0 )
		h += 360;

	Vec3d hsv(h,s,v);
	return hsv;
}

Vec3b MainWindow::HSV2BGR(Vec3d hsv)
{
	float r = 0;
	float g = 0;
	float b = 0;
	float h = hsv[0];
	float s = hsv[1];
	float v = hsv[2];

	int i;
	float f, p, q, t;
	if( s == 0 ) {
		// achromatic (grey)
		r = g = b = v;
		return Vec3b();
	}
	h /= 60;			// sector 0 to 5
	i = floor( h );
	f = h - i;			// factorial part of h
	p = v * ( 1 - s );
	q = v * ( 1 - s * f );
	t = v * ( 1 - s * ( 1 - f ) );
	switch( i ) {
		case 0:
			r = v;
			g = t;
			b = p;
			break;
		case 1:
			r = q;
			g = v;
			b = p;
			break;
		case 2:
			r = p;
			g = v;
			b = t;
			break;
		case 3:
			r = p;
			g = q;
			b = v;
			break;
		case 4:
			r = t;
			g = p;
			b = v;
			break;
		default:		// case 5:
			r = v;
			g = p;
			b = q;
			break;
	}

	return Vec3b(b*255,g*255,r*255);
}

QString MainWindow::getOpenCVTypeName(int type)
{
	QString r;

	uchar depth = type & CV_MAT_DEPTH_MASK;
	uchar chans = 1 + (type >> CV_CN_SHIFT);

	switch ( depth ) {
	case CV_8U:  r = "8U"; break;
	case CV_8S:  r = "8S"; break;
	case CV_16U: r = "16U"; break;
	case CV_16S: r = "16S"; break;
	case CV_32S: r = "32S"; break;
	case CV_32F: r = "32F"; break;
	case CV_64F: r = "64F"; break;
	default:     r = "User"; break;
	}

	r += "C";
	r += (chans+'0');

	return r;
}

void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
	ui->lineEdit_superpixelSize->setText(QString::number(position));
}
