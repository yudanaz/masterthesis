#include "skindetector.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

SkinDetector::SkinDetector()
{
	QFile fin("diff-filters.txt");
	bool success = fin.open(QFile::ReadOnly | QFile::Text);
	if(!success)
	{
		QMessageBox::information(NULL, "Error", "Difference Filter file couldn't be found!", QMessageBox::Ok);
		return;
	}
	QTextStream in(&fin);

	//jump two lines and read rest into lists
	in.readLine(); in.readLine();
	QStringList sl1 = in.readLine().split("; ");
	QStringList sl2 = in.readLine().split("; ");
	QStringList sl3 = in.readLine().split("; ");
	fin.close();

	//set filter parameters
	lowThresh1 = sl1.at(2).toDouble();
	lowThresh2 = sl2.at(2).toDouble();
	lowThresh3 = sl3.at(2).toDouble();
	highThresh1 = sl1.at(3).toDouble();
	highThresh2 = sl2.at(3).toDouble();
	highThresh3 = sl3.at(3).toDouble();
	min970 = sl1.at(4).toInt();
	min1300 = sl1.at(5).toInt();
	min1550 = sl2.at(5).toInt();
}


Mat SkinDetector::detect(Mat nirImg)
{
	Mat img970, img1300, img1550;
	vector<Mat> channels(3);
	cv::split(nirImg, channels);

//	imshow("970", channels.at(0));
//	imshow("1300", channels.at(1));
//	imshow("1550", channels.at(2));cvWaitKey();

	channels.at(2).convertTo(img970, CV_32F, 1600);
	channels.at(1).convertTo(img1300, CV_32F, 1600);
	channels.at(0).convertTo(img1550, CV_32F, 1600);

	//apply filters
	Mat resND = Mat::ones(nirImg.rows, nirImg.cols, CV_8U);
	Mat f1 = filter(img970, img1300, min970, min1300, lowThresh1, highThresh1);
	Mat f2 = filter(img970, img1550, min970, min1550, lowThresh2, highThresh2);
	Mat f3 = filter(img1300, img1550, min1300, min1550, lowThresh3, highThresh3);

//	imshow("f1", f1*200);
//	imshow("f2", f2*200);
//	imshow("f3", f3*200); cvWaitKey();

	multiply(resND, f1, resND);
	multiply(resND, f2, resND);
	multiply(resND, f3, resND);

	return resND;
}

Mat SkinDetector::filter(Mat img1, Mat img2, int min1, int min2, double lowThresh, double highThresh)
{
	Mat image1F, image2F, result, diffImageF, maskLowF, maskHighF, maskLow, maskHigh;

	image1F = img1;
	image2F = img2;

	//threshold absolute intensity values to filter wrong classifications due
	//to noise in dark image areas:
	if(min1 > 0)
	{
		threshold(image1F, image1F, min1, 0, THRESH_TOZERO);
	}
	if(min2 > 0)
	{
		threshold(image2F, image2F, min2, 0, THRESH_TOZERO);
	}

	//build the normalized difference image
	//Notice: if pixel p in both images is 0, we will divide by 0,
	//but divide() will set dst(p) to 0 in this case. This is fine,
	//as 0 means that image1(p) and image2(p) are identical.
	divide( (image1F - image2F), (image1F + image2F), diffImageF);

	if((lowThresh > -1.0) || (highThresh < 1.0))
	{
		//do lower thresholding:
		threshold(diffImageF, maskLowF, lowThresh, 1.0, THRESH_BINARY);

		//convert float back to 8bit unsigned
		maskLowF.convertTo(maskLow, CV_8U);

		//do upper thresholding:
		threshold(diffImageF, maskHighF, highThresh, 1.0, THRESH_BINARY_INV);

		//convert float back to 8bit unsigned
		maskHighF.convertTo(maskHigh, CV_8U);

		//multiply low and high mask (intersection)
		multiply(maskLow, maskHigh, result);
	}

	return result;
}
