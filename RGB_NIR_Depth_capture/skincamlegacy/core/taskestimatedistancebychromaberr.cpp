#include <cmath>
#include "taskestimatedistancebychromaberr.h"
#include "sharpnessestimator.h"

TaskEstimateDistanceByChromAberr::TaskEstimateDistanceByChromAberr()
{
	 metric = metric_Sobel;
	 sharpnessNbrhd = 7;
	 medianKernel = 7;
}

void TaskEstimateDistanceByChromAberr::process(MultispectralImage frame)
{
	Mat result = estimateDistance(frame);
	emit finishedProcessing(result);
}

void TaskEstimateDistanceByChromAberr::process(Mat frame)
{
	Mat result;
	emit finishedProcessing(result);
}

Mat TaskEstimateDistanceByChromAberr::estimateDistance(MultispectralImage img)
{
	int maxSharpness = 0;
	int sharpestBand = 1;

	//make multispectral result image
	quint16 width = img.getWidth();
	quint16 height = img.getHeight();
	Mat result = Mat::zeros(height, width, CV_16U);
	Rect roi(0, 0, width, height);

	//computer sharpness for all bands (less dark image)
	images.clear();
	sharpnessResults.clear();
	QMapIterator<qint16, Mat> it(img.getImages());
	waveBands.clear();
	while(it.hasNext())
	{
		it.next(); //jump dark image
		if(it.key() == 0)
			continue;
		images.append(it.value());
		sharpnessResults.append(computeLocalSharpness(it.value()));
		waveBands.append(it.key());
	}

	//make iterators for edge and result image
	int px;
	MatIterator_<uchar> it2, end;
	MatIterator_<unsigned short> itRes, endRes;

	//for every edge pixel in mask
	for( px = 0, it2 = edgeMask.begin<uchar>(), end = edgeMask.end<uchar>(),
		 itRes = result.begin<unsigned short>(), endRes = result.end<unsigned short>();
		 it2 != end; px++, ++it2, ++itRes)
	{
		uchar val = *it2;
		if(val != '\0')
		{
			//compute local sharpness
			Rect sharpnnNbrhd = makeNeighborhood(roi, px, sharpnessNbrhd);
			int sh935 = getHighestInNeighborhood(sharpnessResults.at(0), sharpnnNbrhd);
			int sh1060 = getHighestInNeighborhood(sharpnessResults.at(1), sharpnnNbrhd);
			int sh1300 = getHighestInNeighborhood(sharpnessResults.at(2), sharpnnNbrhd);
			int sh1550 = getHighestInNeighborhood(sharpnessResults.at(3), sharpnnNbrhd);

			//compute local contrast
			Rect contrastNeighborhood = makeNeighborhood(roi, px, 15); //contrast box size hardcoded to 15!
			Mat contrastRoi1(images.at(0), contrastNeighborhood);
			Mat contrastRoi2(images.at(1), contrastNeighborhood);
			Mat contrastRoi3(images.at(2), contrastNeighborhood);
			Mat contrastRoi4(images.at(3), contrastNeighborhood);
			double contr1 = computeContrast(contrastRoi1);
			double contr2 = computeContrast(contrastRoi2);
			double contr3 = computeContrast(contrastRoi3);
			double contr4 = computeContrast(contrastRoi4);

			//compute normalized differences for sharpness and contrast
			std::vector<double> sharpnDiffs = computeNormalizedDifferences(sh1550, sh1300, sh1060, sh935);
			std::vector<double> contrDiffs = computeNormalizedDifferences(contr1, contr2, contr3, contr4);

			//estimate distance from camera
			double temp = distEstimator.estimateDistance(sharpnDiffs[0], sharpnDiffs[1], sharpnDiffs[2], sharpnDiffs[3], sharpnDiffs[4], sharpnDiffs[5],
					contrDiffs[0], contrDiffs[1], contrDiffs[2], contrDiffs[3], contrDiffs[4], contrDiffs[5]);
			unsigned short dist = (unsigned short)temp;

			//write distance in result image
			*itRes = dist;

			//check which sharpness is highest and update sharpest band (for edge detection)
			if(sh935 > maxSharpness) { maxSharpness = sh935; sharpestBand = 1; }
			if(sh1060 > maxSharpness) { maxSharpness = sh1060; sharpestBand = 2; }
			if(sh1300 > maxSharpness) { maxSharpness = sh1300; sharpestBand = 3; }
			if(sh1550 > maxSharpness) { maxSharpness = sh1550; sharpestBand = 4; }
		}
	}

	//report sharpest band
	emit reportBack(sharpestBand);
	//qDebug() << "sharpestBand: "  << sharpestBand;

	if(medianKernel > 1) { medianSmooth(result, medianKernel); }
	return result;
}

void TaskEstimateDistanceByChromAberr::medianSmooth(Mat img, int kernel)
{
	unsigned short median = 0;
	int ll = 0;

	//for every non-zero pixel
	MatIterator_<unsigned short> it, end, it2, end2;
	int px;
	Rect imgRect(0, 0, img.cols, img.rows);
	for(px = 0, it = img.begin<unsigned short>(), end = img.end<unsigned short>(); it != end; px++, it++)
	{
		if(*it != '\0')
		{
			//get neighborhood of pixel
			Rect nbrhd = makeNeighborhood(imgRect, px, kernel);
			Mat imgNbrhd(img, nbrhd);

			//compute median and set pixel to that value
			QList<unsigned short> l;
			for(it2 = imgNbrhd.begin<unsigned short>(), end2 = imgNbrhd.end<unsigned short>(); it2 != end2; it2++)
			{
				unsigned short value = *it2;

				//only append non-zero values, else the median is gonna be zero
				if(value != 0) { l.append(value); }
			}
			qSort(l);

			ll = l.count(); //list length
			if(ll % 2 == 0) //list length is even
			{
				median = round((l[ll / 2] + l[ll / 2 - 1]) / 2.0);
			}
			else //list length is uneven
			{
				median = l[ll / 2];
			}
			*it = median;
		}
	}
}

Mat TaskEstimateDistanceByChromAberr::computeLocalSharpness(Mat img)
{
	switch (metric) {
		case metric_Spectral:
			return SharpnessEstimator::spectral(img);
			break;
		case metric_LaplaceKrotkov:
			return SharpnessEstimator::laplaceKrotkov(img);
			break;
		case metric_LaplaceOpenCV:
			return SharpnessEstimator::laplaceOpenCV(img);
			break;
		case metric_Sobel:
		default:
			return SharpnessEstimator::sobel(img);
			break;
	}
}

std::vector<double> TaskEstimateDistanceByChromAberr::computeNormalizedDifferences(double v1550, double v1300, double v1060, double v935)
{
	double d1 = std::abs(v1550 - v1300);
	double d2 = std::abs(v1550 - v1060);
	double d3 = std::abs(v1550 - v935);
	double d4 = std::abs(v1300 - v1060);
	double d5 = std::abs(v1300 - v935);
	double d6 = std::abs(v1060 - v935);
	double base = d1 + d2 + d3 + d4 + d5 + d6;

	vector<double> diffs;
	diffs.reserve(6);
	if(base != 0.0) //avoid division by zero
	{
		diffs.push_back(d1 / base);
		diffs.push_back(d2 / base);
		diffs.push_back(d3 / base);
		diffs.push_back(d4 / base);
		diffs.push_back(d5 / base);
		diffs.push_back(d6 / base);
	}
	else
	{
		for (int i = 0; i < 6; ++i) { diffs.push_back(0.0); }
	}

	return diffs;
}

Rect TaskEstimateDistanceByChromAberr::makeNeighborhood(Rect minMaxRect, int pixelIndex, int neighborhoodSize)
{
	Rect rect;

	//get min and max pos and nbrhd size. !!Position must be at user-defined ROI!!
	int minX = minMaxRect.x;
	int minY = minMaxRect.y;
	int maxX = minMaxRect.x + minMaxRect.width - 1;
	int maxY = minMaxRect.y + minMaxRect.height - 1;
	int boxSizeX = neighborhoodSize;
	int boxSizeY = neighborhoodSize;

	//avoid that box is bigger than image
	if(boxSizeX > minMaxRect.width) { boxSizeX = minMaxRect.width; }
	if(boxSizeY > minMaxRect.height) { boxSizeY = minMaxRect.height; }

	//set position
	int xpos = minX + (pixelIndex % minMaxRect.width) - boxSizeX/2;
	int ypos = minY + (pixelIndex / minMaxRect.width) - boxSizeY/2;

	//avoid overstepping image borders
	if(xpos < minX)
	{
		boxSizeX += xpos - minX + 1;
		xpos = minX;
	}
	else if (xpos + boxSizeX - 1 > maxX)
	{
		boxSizeX = maxX - xpos + 1;
	}
	if(ypos < minY)
	{
		boxSizeY += ypos - minY + 1;
		ypos = minY;
	}
	else if (ypos + boxSizeY - 1 > maxY)
	{
		boxSizeY = maxY - ypos + 1;
	}

	rect.x = xpos;
	rect.y = ypos;
	rect.width = boxSizeX;
	rect.height = boxSizeY;

	return rect;
}

int TaskEstimateDistanceByChromAberr::getHighestInNeighborhood(Mat img, Rect neighborhood)
{
	double min, max;
	Mat roi(img, neighborhood);
	minMaxLoc(roi, &min, &max);
	return max;
}

double TaskEstimateDistanceByChromAberr::computeContrast(Mat roi)
{
	double min, max;
	minMaxLoc(roi, &min, &max);
	return (max - min) / (max + min);
}

void TaskEstimateDistanceByChromAberr::setParameters(int metric, Mat edgeMask, int sharpnessNbrhdSize, int medianSmoothingKernel)
{
	this->metric = (sharpnessMetricMethod)metric;
	edgeMask.copyTo(this->edgeMask);
	this->sharpnessNbrhd = sharpnessNbrhdSize;
	this->medianKernel = medianSmoothingKernel;
}
