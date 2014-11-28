#include "taskdetectskinbyquotient.h"

TaskDetectSkinByQuotient::TaskDetectSkinByQuotient()
{
	;
}

TaskDetectSkinByQuotient::TaskDetectSkinByQuotient(struct quotientFilter filter)
{
	myFilter = filter;
}

void TaskDetectSkinByQuotient::setFilter(quotientFilter filter)
{
	lockFilter.lockForWrite();
	myFilter = filter;
	lockFilter.unlock();
}

Mat TaskDetectSkinByQuotient::applyFilter(Mat image1F, Mat image2F)
{
	Mat result, quotImageF, maskLowF, maskHighF, maskLow, maskHigh;

	static Mat ones = Mat::ones(image2F.size(), image2F.type());

	//threshold absolute intensity values to myFilter wrong classifications due
	//to noise in dark image areas:
	if(myFilter.minValue1 > 0)
		threshold(image1F, image1F, myFilter.minValue1, 0, THRESH_TOZERO);
	if(myFilter.minValue2 > 0)
		threshold(image2F, image2F, myFilter.minValue2, 0, THRESH_TOZERO);
	//if a min value of 0 is allowed for second channel, avoid div by zero:
	else
	{
		add(image2F, ones, image2F);
	}

	//build the quotient image and do thresholding:
	divide(image1F, image2F, quotImageF);
	threshold(quotImageF, maskLowF, myFilter.lowerThreshold, 1.0, THRESH_BINARY);

	//convert float back to 8bit unsigned
	maskLowF.convertTo(maskLow, CV_8U);

	//if there is an upper limit below 999 configured, do upper thresholding:
	if(myFilter.upperThreshold <= 998)
	{
		threshold(quotImageF, maskHighF, myFilter.upperThreshold, 1.0, THRESH_BINARY_INV);

		maskHighF.convertTo(maskHigh, CV_8U);

		//multiply low and high mask (intersection)
		multiply(maskLow, maskHigh, result);
	}
	else
	{
		result = maskLow;
	}

	return result;
}

void TaskDetectSkinByQuotient::process(MultispectralImage msSource)
{
	Mat image1F, image2F;

	lockFilter.lockForRead();

	//if source is a multispectral image, check for at least 2 channels:
	if (msSource.getChannelCount() < 2)
	{
		emit errorProcessing();
	}
	//try to get both images for the current myFilter and convert to float:
	try
	{
		msSource.getImageByWL(myFilter.channel1).convertTo(image1F, CV_32F);
		msSource.getImageByWL(myFilter.channel2).convertTo(image2F, CV_32F);;
	}
	//if one of these is empty or does not exist, skip this myFilter
	catch (out_of_range e)
	{
		emit errorProcessing();
	}

	Mat result = applyFilter(image1F, image2F);

	lockFilter.unlock();

	emit finishedProcessing(result);
}

void TaskDetectSkinByQuotient::process(Mat matSource)
{
	Mat image1F, image2F;

	vector<Mat> rgb(3);

	lockFilter.lockForRead();

	//if source is a Mat image, check if it has 3 channels:
	if(matSource.channels() != 3)
	{
		emit errorProcessing();
	}
	//check if the myFilter's channel do exist:
	if(myFilter.channel1 > 2 || myFilter.channel2 > 2)
	{
		emit errorProcessing();
	}

	//get requested channels and convert to float:
	split(matSource, rgb);
	rgb.at(myFilter.channel1).convertTo(image1F, CV_32F);;
	rgb.at(myFilter.channel2).convertTo(image2F, CV_32F);;

	Mat result = applyFilter(image1F, image2F);

	lockFilter.unlock();

	emit finishedProcessing(result);
}
