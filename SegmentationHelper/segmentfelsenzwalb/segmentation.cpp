#include <QTime>
#include <QDebug>
#include "segment-image.h"
#include "segmentation.h"

Segmentation::Segmentation()
{
}

Mat Segmentation::makeSuperPixelSegmenation(Mat img, float sigma, float k, int min_size, int *num_ccs)
{
	image<rgb> *input = getRGBimg(img);

	QTime myTimer;
	myTimer.start();

	image<rgb> *seg = segment_image(input, sigma, k, min_size, num_ccs);

	qDebug() << "elapsed time: " << myTimer.elapsed();

	return getMat(seg);

//	return getMat(getRGBimg(img));
}

Mat Segmentation::getMat(image<rgb> *rgbimg)
{
	int w = rgbimg->width(), h = rgbimg->height();
	Mat img(h, w, CV_8UC3);
	for (int y = 0; y < h; ++y)
	{
		for (int x = 0; x < w; ++x)
		{
			img.at<Vec3b>(y,x)[0] = rgbimg->access[y][x].r;
			img.at<Vec3b>(y,x)[1] = rgbimg->access[y][x].g;
			img.at<Vec3b>(y,x)[2] = rgbimg->access[y][x].b;
		}
	}
	return img;
}

image<rgb>* Segmentation::getRGBimg(Mat img)
{
	int w = img.cols, h = img.rows;
	image<rgb> *rgbimg = new image<rgb>(w,h);
	for (int y = 0; y < h; ++y)
	{
		for (int x = 0; x < w; ++x)
		{
			rgbimg->access[y][x].r = img.at<Vec3b>(y,x)[0];
			rgbimg->access[y][x].g = img.at<Vec3b>(y,x)[1];
			rgbimg->access[y][x].b = img.at<Vec3b>(y,x)[2];
		}
	}
	return rgbimg;
}
