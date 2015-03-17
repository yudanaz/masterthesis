#include "helper.h"
#include "opencv2/opencv.hpp"
#include <QString>

int Helper::debugImgCnt = 0;

void Helper::debugImage(Mat img, QString msg)
{
	QString s;
	if(msg == ""){ s = "debug image" + QString::number(debugImgCnt++); }
	else{ s = msg; }
	imshow(s.toStdString().c_str(), img);
	cvWaitKey();
}

Helper::Helper()
{
}

// HOGDescriptor visual_imagealizer
// adapted for arbitrary size of feature sets and training images
Mat Helper::get_hogdescriptor_visual_image(Mat& origImg,
								   vector<float>& descriptorValues,
								   Size winSize,
								   Size cellSize,
								   int scaleFactor,
								   double viz_factor)
{
	Mat visual_image;
	resize(origImg, visual_image, Size(origImg.cols*scaleFactor, origImg.rows*scaleFactor));

	int gradientBinSize = 9;
	// dividing 180° into 9 bins, how large (in rad) is one bin?
	float radRangeForOneBin = 3.14/(float)gradientBinSize;

	// prepare data structure: 9 orientation / gradient strenghts for each cell
	int cells_in_x_dir = winSize.width / cellSize.width;
	int cells_in_y_dir = winSize.height / cellSize.height;
	int totalnrofcells = cells_in_x_dir * cells_in_y_dir;
	float*** gradientStrengths = new float**[cells_in_y_dir];
	int** cellUpdateCounter   = new int*[cells_in_y_dir];
	for (int y=0; y<cells_in_y_dir; y++)
	{
		gradientStrengths[y] = new float*[cells_in_x_dir];
		cellUpdateCounter[y] = new int[cells_in_x_dir];
		for (int x=0; x<cells_in_x_dir; x++)
		{
			gradientStrengths[y][x] = new float[gradientBinSize];
			cellUpdateCounter[y][x] = 0;

			for (int bin=0; bin<gradientBinSize; bin++)
				gradientStrengths[y][x][bin] = 0.0;
		}
	}

	// nr of blocks = nr of cells - 1
	// since there is a new block on each cell (overlapping blocks!) but the last one
	int blocks_in_x_dir = cells_in_x_dir - 1;
	int blocks_in_y_dir = cells_in_y_dir - 1;

	// compute gradient strengths per cell
	int descriptorDataIdx = 0;
	int cellx = 0;
	int celly = 0;

	for (int blockx=0; blockx<blocks_in_x_dir; blockx++)
	{
		for (int blocky=0; blocky<blocks_in_y_dir; blocky++)
		{
			// 4 cells per block ...
			for (int cellNr=0; cellNr<4; cellNr++)
			{
				// compute corresponding cell nr
				int cellx = blockx;
				int celly = blocky;
				if (cellNr==1) celly++;
				if (cellNr==2) cellx++;
				if (cellNr==3)
				{
					cellx++;
					celly++;
				}

				for (int bin=0; bin<gradientBinSize; bin++)
				{
					float gradientStrength = descriptorValues[ descriptorDataIdx ];
					descriptorDataIdx++;

					gradientStrengths[celly][cellx][bin] += gradientStrength;

				} // for (all bins)


				// note: overlapping blocks lead to multiple updates of this sum!
				// we therefore keep track how often a cell was updated,
				// to compute average gradient strengths
				cellUpdateCounter[celly][cellx]++;

			} // for (all cells)


		} // for (all block x pos)
	} // for (all block y pos)


	// compute average gradient strengths
	for (int celly=0; celly<cells_in_y_dir; celly++)
	{
		for (int cellx=0; cellx<cells_in_x_dir; cellx++)
		{

			float NrUpdatesForThisCell = (float)cellUpdateCounter[celly][cellx];

			// compute average gradient strenghts for each gradient bin direction
			for (int bin=0; bin<gradientBinSize; bin++)
			{
				gradientStrengths[celly][cellx][bin] /= NrUpdatesForThisCell;
			}
		}
	}


	std::cout << "descriptorDataIdx = " << descriptorDataIdx << std::endl;

	// draw cells
	for (int celly=0; celly<cells_in_y_dir; celly++)
	{
		for (int cellx=0; cellx<cells_in_x_dir; cellx++)
		{
			int drawX = cellx * cellSize.width;
			int drawY = celly * cellSize.height;

			int mx = drawX + cellSize.width/2;
			int my = drawY + cellSize.height/2;

			rectangle(visual_image,
					  Point(drawX*scaleFactor,drawY*scaleFactor),
					  Point((drawX+cellSize.width)*scaleFactor,
					  (drawY+cellSize.height)*scaleFactor),
					  CV_RGB(100,100,100),
					  1);

			// draw in each cell all 9 gradient strengths
			for (int bin=0; bin<gradientBinSize; bin++)
			{
				float currentGradStrength = gradientStrengths[celly][cellx][bin];

				// no line to draw?
				if (currentGradStrength==0)
					continue;

				float currRad = bin * radRangeForOneBin + radRangeForOneBin/2;

				float dirVecX = cos( currRad );
				float dirVecY = sin( currRad );
				float maxVecLen = cellSize.width/2;
				float scale = viz_factor; // just a visual_imagealization scale,
										  // to see the lines better

				// compute line coordinates
				float x1 = mx - dirVecX * currentGradStrength * maxVecLen * scale;
				float y1 = my - dirVecY * currentGradStrength * maxVecLen * scale;
				float x2 = mx + dirVecX * currentGradStrength * maxVecLen * scale;
				float y2 = my + dirVecY * currentGradStrength * maxVecLen * scale;

				// draw gradient visual_imagealization
				line(visual_image,
					 Point(x1*scaleFactor,y1*scaleFactor),
					 Point(x2*scaleFactor,y2*scaleFactor),
					 CV_RGB(0,0,255),
					 1);

			} // for (all bins)

		} // for (cellx)
	} // for (celly)


	// don't forget to free memory allocated by helper data structures!
	for (int y=0; y<cells_in_y_dir; y++)
	{
	  for (int x=0; x<cells_in_x_dir; x++)
	  {
		   delete[] gradientStrengths[y][x];
	  }
	  delete[] gradientStrengths[y];
	  delete[] cellUpdateCounter[y];
	}
	delete[] gradientStrengths;
	delete[] cellUpdateCounter;

	return visual_image;

}


/**
 * Function for thinning the given binary image
 *
 * @param  im  Binary image with range = 0-255
 */
void Helper::thinning(cv::Mat& im)
{
	im /= 255;

	cv::Mat prev = cv::Mat::zeros(im.size(), CV_8UC1);
	cv::Mat diff;

	do {
		thinningIteration(im, 0);
		thinningIteration(im, 1);
		cv::absdiff(im, prev, diff);
		im.copyTo(prev);
	}
	while (cv::countNonZero(diff) > 0);

	im *= 255;
}

void Helper::thinningIteration(cv::Mat& im, int iter)
{
	cv::Mat marker = cv::Mat::zeros(im.size(), CV_8UC1);

	for (int i = 1; i < im.rows-1; i++)
	{
		for (int j = 1; j < im.cols-1; j++)
		{
			uchar p2 = im.at<uchar>(i-1, j);
			uchar p3 = im.at<uchar>(i-1, j+1);
			uchar p4 = im.at<uchar>(i, j+1);
			uchar p5 = im.at<uchar>(i+1, j+1);
			uchar p6 = im.at<uchar>(i+1, j);
			uchar p7 = im.at<uchar>(i+1, j-1);
			uchar p8 = im.at<uchar>(i, j-1);
			uchar p9 = im.at<uchar>(i-1, j-1);

			int A  = (p2 == 0 && p3 == 1) + (p3 == 0 && p4 == 1) +
					 (p4 == 0 && p5 == 1) + (p5 == 0 && p6 == 1) +
					 (p6 == 0 && p7 == 1) + (p7 == 0 && p8 == 1) +
					 (p8 == 0 && p9 == 1) + (p9 == 0 && p2 == 1);
			int B  = p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;
			int m1 = iter == 0 ? (p2 * p4 * p6) : (p2 * p4 * p8);
			int m2 = iter == 0 ? (p4 * p6 * p8) : (p2 * p6 * p8);

			if (A == 1 && (B >= 2 && B <= 6) && m1 == 0 && m2 == 0)
				marker.at<uchar>(i,j) = 1;
		}
	}

	im &= ~marker;
}
