#include "imageprocessor.h"

ImageProcessor::ImageProcessor(SkinCamConfig aConfig)
	:mouseClickNbhrd(7), cannyHighThresh(127), cannyLowThresh(127), cannyImage(0), medianKernel(7),
	  autoSelectCannyImage(false), lastSharpestBand(0),
	  distLowEnd(aConfig.distanceLowEnd), distHighEnd(aConfig.distanceHighEnd)
{
	setConfig(aConfig);
}

ImageProcessor::~ImageProcessor()
{
	;
}

void ImageProcessor::skinDetectionReady(Mat result)
{
	//add result image to list of results
	skinDetectionResults.append(result);
}

void ImageProcessor::distEstimationReady(Mat result)

{
	Mat result8Bit = make8BitColorDistanceImage(result, distLowEnd, distHighEnd);

	imgDesc distImg8Bit, distImg16Bit;
	distImg8Bit.desc = "Distance";
	distImg8Bit.img = result8Bit;
	distImg16Bit.desc = "Distance16Bit";
	distImg16Bit.img = result;
	distEstimationResults.append(distImg8Bit);
	distEstimationResults.append(distImg16Bit);
}

void ImageProcessor::onError()
{
	errorOccurred = true;
}

void ImageProcessor::setDistEstimParams(int lowCannyThresh, int highCannyThresh, int selectedImageForCanny, bool autoSelectCannyImage, int medianSmoothingKernel, int distLowEnd, int distHighEnd)
{
	this->cannyImage = selectedImageForCanny;
	this->autoSelectCannyImage = autoSelectCannyImage;
	this->cannyHighThresh = highCannyThresh;
	this->cannyLowThresh = lowCannyThresh;
	this->medianKernel = medianSmoothingKernel;
	this->distLowEnd = distLowEnd;
	this->distHighEnd = distHighEnd;
}

void ImageProcessor::reportFromDistEstimation(int sharpestBand)
{
	lastSharpestBand = sharpestBand;
}

void ImageProcessor::setConfig(SkinCamConfig newConfig)
{
	lockConfig.lockForWrite();
	myConfig = newConfig;

	//if skin detection is enabled, create one subthread for each filter
	//or remove unnecessary filters
	if(myConfig.detectSkinByQuotient && (myConfig.quotientFilters.size() > 0))
	{
		while(subthreads.size() < myConfig.quotientFilters.size())
		{
			subthreads.append(new QThread());
			skinTasks.append(new TaskDetectSkinByQuotient());
			skinTasks.last()->moveToThread(subthreads.last());

			connect(this, SIGNAL(doSkinDetection(MultispectralImage)),
					skinTasks.last(), SLOT(process(MultispectralImage)));

			connect(skinTasks.last(), SIGNAL(finishedProcessing(Mat)),
					this, SLOT(skinDetectionReady(Mat)));

			connect(skinTasks.last(), SIGNAL(errorProcessing()),
					this, SLOT(onError()));

			subthreads.last()->start();
		}
		while (subthreads.size() > myConfig.quotientFilters.size())
		{
			subthreads.last()->quit();
			subthreads.last()->wait();
			delete subthreads.last();
			subthreads.removeLast();
			skinTasks.removeLast();
		}

		//assign one filter to each task
		for(int i = 0; i < skinTasks.size(); i++)
			skinTasks[i]->setFilter(myConfig.quotientFilters.at(i));
	}

	//if distance estimation is enabled, create a subthread for it
	if (myConfig.estimateDistance)
	{
		//clean old threads, create new
		while (distThreads.size() > 0)
		{
			distThreads.last()->quit();
			distThreads.last()->wait();
			delete distThreads.last();
			distThreads.removeLast();
		}
		distThreads.append(new QThread());

		//connect signals & slots for thread
		distTask = new TaskEstimateDistanceByChromAberr();
		distTask->moveToThread(distThreads.last());
		connect(this, SIGNAL(doDistanceEstimation(MultispectralImage)), distTask,
				SLOT(process(MultispectralImage)));
		connect(this, SIGNAL(setDistEstimParams(int,Mat,int,int)), distTask,
				SLOT(setParameters(int,Mat,int,int)));
		connect(distTask, SIGNAL(finishedProcessing(Mat)), this,
				SLOT(distEstimationReady(Mat)));
		connect(distTask, SIGNAL(errorProcessing()), this, SLOT(onError()));
		connect(distTask, SIGNAL(reportBack(int)), this, SLOT(reportFromDistEstimation(int)));

		distThreads.last()->start();
	}

	lockConfig.unlock();
}

int ImageProcessor::getEstimatedDistanceAt(int x, int y)
{
	if(distEstimationResults.count() == 0) {return 0;}

	//get distance value on provided mouse position, or if that value is zero, then
	//the next available value close to it (in a neigborhod around it)
	//-> clicking on a pixel-wide line with the mouse might be difficult
	Mat img = distEstimationResults.at(1).img;
	int w = img.cols;
	int h = img.rows;
	int range = mouseClickNbhrd / 2;
	int d = img.at<unsigned short>(y, x);

	//if there is a value at the provided position, return it
	if(d != 0) { return d; }

	//else search for a value close to it
	for (int i = -range; i < range; ++i)
	{
		for (int j = -range; j < range; ++j)
		{
			//get new position respecting image borders
			int xx = x + i;
			int yy = y + j;
			xx = xx < 0 ? 0 : xx;
			xx = xx > w ? w : xx;
			yy = yy < 0 ? 0 : yy;
			yy = yy > h ? h : yy;

			d = img.at<unsigned short>(yy, xx);
			if(d != 0) { return d; }
		}
	}

	return 0;
}

void ImageProcessor::process(MultispectralImage frame)
{
	MultispectralImage frame8Bit;
	QList<imgDesc> results;

	quint8 i;

	Mat filterMask;
	Mat maskedFCImage;
	double maxVal = 0;
	double maxTemp = 0.0;

	Mat temp;
	double spread;

	Mat motionMask;

	errorOccurred = false;

	lockConfig.lockForRead();

	//main processing tasks
	//*********************

	//subtract dark image, if enabled
	if(myConfig.calibration.subtractDark && !frame.getDarkSubtracted())
	{
		for(i = 1; i < frame.getChannelCount(); i++)
		{
			//subtract dark image from current image
			Mat tmp;
			cv::subtract(frame.getImageByChannelNumber(i),
						 frame.getDarkImage(),
						 tmp);

			//set result as new channel image
			frame.setChannelImage(frame.getWavebands().at(i), tmp);
		}

		frame.setDarkSubtracted(true);
	}

	//perform skin detection by using quotient filters, if enabled
	if(myConfig.detectSkinByQuotient && (myConfig.quotientFilters.size() > 0))
	{
		//clear result list
		skinDetectionResults.clear();

		//signal processing of all filters
		emit doSkinDetection(frame);
	}

	//if image depth is more than 8bit, image has to be resampled to be displayed
	if(frame.getDepth() > 8)
	{
		//if automatic contrast is enabled, find the brightest spot in all channels
		if(myConfig.contrastAutomatic)
		{
			//iterate through all bands (except dark) to find maximum value
			for(i = 1; i < frame.getChannelCount(); i++)
			{
				minMaxLoc(frame.getImageByChannelNumber(i), NULL, &maxTemp);
				if ( maxTemp > maxVal )
				{
					maxVal = maxTemp;
				}
			}

			//subtract contrast dark offset from maximum
			maxVal -= myConfig.contrastOffset;

			//slowly increase or decrease contrast value
			if((maxVal / myConfig.contrastValue) < 220)
			{
				myConfig.contrastValue -= (myConfig.contrastValue - (maxVal / 255)) / 10;
			}
			else if((maxVal / myConfig.contrastValue) > 250)
			{
				myConfig.contrastValue += ((maxVal / 255) - myConfig.contrastValue) / 10;
			}
		}

		//calculate spread factor
		spread = 1.0 / (double)myConfig.contrastValue;

		//configure GUI image object
		frame8Bit.setSize(frame.getWidth(), frame.getHeight());
		frame8Bit.setDepth(8);

		//scale down every band
		for (i = 0; i < frame.getChannelCount(); i++)
		{
			//subtract contrast offset, if enabled
			Mat tempOffset;
			if(myConfig.contrastOffset > 0)
			{
				subtract(frame.getImageByChannelNumber(i),
						 Scalar(myConfig.contrastOffset),
						 tempOffset);
			}
			else
			{
				tempOffset = frame.getImageByChannelNumber(i);
			}

			//convert to 8 bit using spread factor
			tempOffset.convertTo(temp, 8, spread );
			frame8Bit.setChannelImage(frame.getWavebands().at(i), temp.clone());
		}
	}
	else
	{
		frame8Bit = frame;
	}

	//detect edges
	if(myConfig.edgeDetection)
	{
		QMapIterator<qint16, Mat> it(frame8Bit.getImages());
		while(it.hasNext())
		{
			it.next();

			Mat edges = doEdgeDetection(it.value(), myConfig.edgeThreshold);

			struct imgDesc edgeResult;
			edgeResult.desc = QString("Edges %1nm").arg(it.key());
			edgeResult.img = edges;
			results.append(edgeResult);
		}
	}

	//Estimate distance (in separate thread)
	if (myConfig.estimateDistance)
	{
		//make edge mask on selected image
		Mat edges;
		if(autoSelectCannyImage) //automatically select sharpest band image for edge detection
		{
			Canny(frame8Bit.getImageByChannelNumber(lastSharpestBand), edges, cannyLowThresh, cannyHighThresh);
		}
		else //use band image selected by the user (in GUI)
		{
			Canny(frame8Bit.getImageByChannelNumber(cannyImage), edges, cannyLowThresh, cannyHighThresh);
		}

		//emit signals to distance estimation thread
		distEstimationResults.clear();
		emit setDistEstimParams((int)myConfig.sharpMetric, edges, myConfig.sharpnessNbrhdSize, medianKernel);
		emit doDistanceEstimation(frame8Bit);

		//wait for thread to finish
		while (!errorOccurred && distEstimationResults.size() < 1) //frame8Bit.getChannelCount()-1)
		{
			QCoreApplication::processEvents();
		}
		if(errorOccurred)
		{
			emit errorProcessing(ImageSourceException("Error in task: estimateDistanceByChromAberr."));
			return;
		}

		//append distance estimation result to results in order to display them
		if(!distEstimationResults.empty())
		{
			//get 8 bit image from 1st list entry (at position 0)
			results.append(distEstimationResults.at(0));
		}
	}

	//wait for threads to finish:
	//***************************

	//wait until all threads are finished, get results and delete them

	if(myConfig.detectSkinByQuotient && (myConfig.quotientFilters.size() > 0))
	{
		maskedFCImage = Mat::zeros(frame8Bit.getDarkImage().rows,
								   frame8Bit.getDarkImage().cols, CV_8UC3);

		//wait until all threads are finished and get results
		while(!errorOccurred &&
			  (myConfig.quotientFilters.size() > skinDetectionResults.size()))
		{
			QCoreApplication::processEvents(QEventLoop::AllEvents);
		}
		if(errorOccurred)
		{
			emit errorProcessing(ImageSourceException("Error in task: detectSkinByQuotients."));
			return;
		}
		//multiply (cut) the filter masks
		filterMask = skinDetectionResults.at(0);
		for(i = 1; i < skinDetectionResults.size(); i++ )
		{
			multiply(filterMask, skinDetectionResults.at(i),
					 filterMask, 1.0);
		}

		//remove positive pixels with motion artifacts
		if(myConfig.suppressMotion && (lastFrame.getChannelCount() == frame.getChannelCount()))
		{
			motionMask = Mat::ones(maskedFCImage.rows, maskedFCImage.cols, CV_8UC1);

			for(i= 0; i < frame.getChannelCount(); i++)
			{
				Mat diffF, threshF, thresh;
				Mat curF, prevF;

				//get frame channels and convert to float
				frame.getImageByChannelNumber(i).convertTo(curF, CV_32F);
				lastFrame.getImageByChannelNumber(i).convertTo(prevF, CV_32F);

				//calculate absolute difference between current and previous frame
				absdiff(curF, prevF, diffF);

				//threshold the absolute difference
				threshold(diffF, threshF, myConfig.motionThreshold, 1.0, THRESH_BINARY_INV);

				//convert to 8 bit unsigned
				threshF.convertTo(thresh, CV_8U);

				//update motion mask with new thresholded difference mask
				multiply(motionMask, thresh, motionMask);
			}

			//now multiply motion mask with filter mask to remove positive filter results
			//where there was motion detected
			multiply(motionMask, filterMask, filterMask);

			//add motion mask to results
			struct imgDesc motionResult;
			motionResult.desc = "Motion";
			threshold(motionMask, motionResult.img, 0, 255, THRESH_BINARY_INV) ;
			results.append(motionResult);
		}

		//Morph result:
		if(myConfig.morphResult)
		{
			Mat element(4,4,CV_8U,Scalar(1));
			morphologyEx(filterMask, filterMask, MORPH_OPEN, element);
		}

		//set mask on top of (8bit) false colour image
		bitwise_or(maskedFCImage,
				   frame8Bit.getFalseColorImage(myConfig.falseColorChannels),
				   maskedFCImage, filterMask);

		if(myConfig.showMaskContours)
		{
			vector<vector<Point> > contours;
			CvScalar green = CV_RGB(0,255,0);
			//CvScalar blue = CV_RGB(0,0,255);

			findContours(filterMask, contours,
						 CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

			drawContours(maskedFCImage, contours, -1, green, 2, 8);
		}

		struct imgDesc skinMask;
		struct imgDesc skinResult;

		skinMask.desc = "QF Mask";
		threshold(filterMask, skinMask.img, 0, 255, THRESH_BINARY) ;
		results.append(skinMask);

		skinResult.desc = "Masked FC Image";
		skinResult.img = maskedFCImage;
		results.append(skinResult);
	}

	lockConfig.unlock();

	emit finishedProcessing(frame, frame8Bit, results);

	lastFrame = frame;
}

Mat ImageProcessor::doEdgeDetection(Mat frame, quint8 threshold)
{
	Mat detected_edges;

	// Reduce noise with a kernel 3x3
	blur( frame, detected_edges, Size(3,3) );

	// Canny detector
	cv::Canny( detected_edges, detected_edges, threshold, threshold * 3, 3 );

	// Using Canny's output as a mask, we display our result
	Mat dst = Mat::zeros(frame.rows, frame.cols, CV_8UC3);

	frame.copyTo( dst, detected_edges );

	return dst;
}

Mat ImageProcessor::make8BitColorDistanceImage(Mat edges16Bit, int min_mm, int max_mm)
{
	float range = max_mm - min_mm;

	//create 3channel 8 bit image matrix
	Mat colorImg(edges16Bit.rows, edges16Bit.cols, CV_8UC3);

	//iterate over edge image, compute color values from edge pixel values and write to color image
	MatIterator_<unsigned short> it, end; //16 bit iterator
	int pixel = 0;

	for(it = edges16Bit.begin<unsigned short>(), end = edges16Bit.end<unsigned short>(); it != end; it++, pixel++)
	{
		unsigned short value = *it;
		if(value != '\0') //not black = zero
		{
			int blue, red;

			//avoid senseless distance interval settings
			if(value < min_mm) { blue = 0; red = 255; }
			else if(value > max_mm) { blue = 255; red = 0; }
			else
			{
				blue = (value - min_mm) / range * 255;
				red = 255-blue;
			}

			colorImg.at<cv::Vec3b>(pixel)[0] = blue;
			colorImg.at<cv::Vec3b>(pixel)[1] = 0;
			colorImg.at<cv::Vec3b>(pixel)[2] = red;
		}
		else
		{
			colorImg.at<cv::Vec3b>(pixel)[0] = 0;
			colorImg.at<cv::Vec3b>(pixel)[1] = 0;
			colorImg.at<cv::Vec3b>(pixel)[2] = 0;
		}
	}

	return colorImg;
}
