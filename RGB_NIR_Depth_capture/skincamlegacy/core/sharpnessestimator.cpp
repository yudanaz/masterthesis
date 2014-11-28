#include "sharpnessestimator.h"

SharpnessEstimator::SharpnessEstimator()
{
}

Mat SharpnessEstimator::sobel(Mat img)
{
	Mat dx, dy, imgGrad, imgGradAbs;

	int kernel = 3;

	//compute horizontal and vertical derivative approx with sobel
	cv::Sobel(img, dx, CV_16S, 1, 0, kernel);
	cv::Sobel(img, dy, CV_16S, 0, 1, kernel);

	//combine into one image
	cv::add(dx, dy, imgGrad);

	//get absolute value (direction of gradient isn't important to us)
	cv::convertScaleAbs(imgGrad, imgGradAbs);

	return imgGradAbs;
}

Mat SharpnessEstimator::laplaceKrotkov(Mat img)
{
	Mat res;

	//create custom filter kernel and convolve
	float laplaceKernel[3][3] = {
		//{0.5,1,.05},
		//{1, -6, 1},
		//{0.5,1,.05}
		{0.166667,  0.666667,  0.166667},
		{0.666667, -3.333333,  0.066667},
		{0.166667,  0.666667,  0.166667}
	};
	Mat kernel(3, 3, CV_32F, &laplaceKernel);
	cv::filter2D(img, res, -1, kernel);

	return res;
}

Mat SharpnessEstimator::laplaceOpenCV(Mat img)
{
	Mat res;
	cv::Laplacian(img, res, -1, 3);
	return res;
}

Mat SharpnessEstimator::spectral(Mat img)
{
	Mat res;

	//expand image to optimal size for dft
	Mat padded;
	int rows = getOptimalDFTSize(img.rows);
	int cols = getOptimalDFTSize(img.cols);
	copyMakeBorder(img, padded, 0, rows - img.rows, 0, cols - img.cols, BORDER_CONSTANT, Scalar::all(0));

	//make place for complex and real values
	Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
	Mat complexI;
	merge(planes, 2, complexI);

	//make discrete fourier transform
	cv::dft(complexI, complexI);

	//transform the real and complex values to magnitude
	split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
	magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
	Mat magI = planes[0];

	//Switch to a logarithmic scale (dynamic range of Fourier coefficients too large for screen)
	magI += Scalar::all(1);
	log(magI, magI);

	//crop and rearrange and normalize
	//magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));
	magI = magI(Rect(0, 0, img.cols & -2, img.rows & -2));
	int cx = magI.cols/2;
	int cy = magI.rows/2;

	Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
	Mat q1(magI, Rect(cx, 0, cx, cy));  // Top-Right
	Mat q2(magI, Rect(0, cy, cx, cy));  // Bottom-Left
	Mat q3(magI, Rect(cx, cy, cx, cy)); // Bottom-Right

	Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
	q2.copyTo(q1);
	tmp.copyTo(q2);

	normalize(magI, magI, 0, 255, CV_MINMAX); // Transform the matrix with float values into a
											// viewable image form (float between values 0 and 1).

	//make sure matrix is continuous and 8bit
	if(!magI.isContinuous())
	{
		magI = magI.clone();
	}
	magI.convertTo(res, 8);

	return res;
}
