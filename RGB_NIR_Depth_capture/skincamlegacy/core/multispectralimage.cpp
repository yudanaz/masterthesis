#include "multispectralimage.h"

MultispectralImage::MultispectralImage()
{
	myWidth = 0;
	myHeight = 0;
	myDepth = 0;
	myChImages.clear();
	isFPNCorrected = false;
	isWhiteCalibrated = false;
	isDarkSubtracted = false;
	isMotionCompensated = false;
}

MultispectralImage::MultispectralImage(quint16 newWidth, quint16 newHeight,
										quint8 newDepth)
{
	myWidth = newWidth;
	myHeight = newHeight;
	myDepth = newDepth;

	myChImages.clear();

	isFPNCorrected = false;
	isWhiteCalibrated = false;
	isDarkSubtracted = false;
	isMotionCompensated = false;
}

MultispectralImage::MultispectralImage(const MultispectralImage& source,
									   bool clone )
{
	myHeight = source.myHeight;
	myWidth = source.myWidth;
	myDepth = source.myDepth;
	isFPNCorrected = source.isFPNCorrected;
	isDarkSubtracted = source.isDarkSubtracted;
	isWhiteCalibrated = source.isWhiteCalibrated;
	isMotionCompensated = source.isMotionCompensated;

	myChImages.clear();
	if (clone)
	{
		QMapIterator<qint16, Mat> it(source.myChImages);
		while(it.hasNext())
		{
			it.next();
			myChImages.insert(it.key(), it.value().clone());
		}
	}
	else
	{
		myChImages = source.myChImages;
	}
}

MultispectralImage& MultispectralImage::operator=( const MultispectralImage& img )
{
	if( this == &img )
	{
		return *this;
	}
	else
	{
		myHeight = img.myHeight;
		myWidth = img.myWidth;
		myDepth = img.myDepth;
		myChImages = img.myChImages;
		isFPNCorrected = img.isFPNCorrected;
		isDarkSubtracted = img.isDarkSubtracted;
		isWhiteCalibrated = img.isWhiteCalibrated;
		isMotionCompensated = img.isMotionCompensated;

		return *this;
	}
}

MultispectralImage::~MultispectralImage()
{
	myChImages.clear();
}

Mat MultispectralImage::getImageByWL(qint16 wl)
{
	if (!myChImages.contains(wl))
	{
		throw ImageHandlingException("Requested image channel does not exist!");
	}

	return myChImages.value(wl);
}

Mat MultispectralImage::getImageByChannelNumber(qint8 channel)
{
	if (getChannelCount() <= channel)
	{
		throw ImageHandlingException("Requested image channel does not exist!");
	}

	return getImageByWL(myChImages.keys().at(channel));
}

QMap<qint16, Mat> MultispectralImage::getImages()
{
	return myChImages;
}

void MultispectralImage::setChannelImage (qint16 channel, Mat newImage)
{
	//check if the new image has the correct size
	if ((newImage.cols != myWidth) || (newImage.rows != myHeight))
	{
		throw ImageSizeException(cvSize(newImage.cols,newImage.rows),
										cvSize(myWidth,myHeight));
	}

	//add image
	myChImages.insert(channel, newImage);
}

Mat MultispectralImage::getDarkImage()
{
	return getImageByWL(0);
}

Mat MultispectralImage::getFalseColorImage(QList<qint16> channels)
{
	Mat newFCImage;
	quint8 i;
	Mat rgb[3];

	if (channels.size() != 3)
		throw ImageHandlingException("Incorrect number of channels given for "\
									 "generation of false color image!");

	for (i = 0; i < 3; i++)
	{
		if(!myChImages.contains(channels.at(i)))
		{
			throw ImageHandlingException("Invalid channels given for "\
										 "generation of false color image!");
		}
		//Attention: openCV images (Mat) are stored in BGR, not in RGB.
		//Therefore, we have to turn the channel order around:
		rgb[2 - i] = myChImages.value(channels.at(i));
	}

	cv::merge(rgb, 3, newFCImage);

	return newFCImage;
}

Mat MultispectralImage::getSideBySideImage(QList<qint16> channels)
{
	Mat newSbSImage;
	Mat temp;
	quint8 i;

	if (channels.size() == 0)
		throw ImageHandlingException("No channels given for generation of "\
									 "side by side image!");

	for(i = 0; i < channels.size(); i++)
	{
		if (!myChImages.contains(channels.at(i)))
		{
			throw ImageHandlingException("Invalid channels given for "\
										 "generation of false color image!");
		}
	}

	//create new image by taking the first channel
	newSbSImage = myChImages.value(channels.at(0));

	//for 1 to 3 channels, store in one row
	if(channels.size() <= 3)
	{
		//concatenate the next channels horizontally
		for(i = 1; i < channels.size(); i++)
		{
			hconcat(newSbSImage, myChImages.value(channels.at(i)), newSbSImage);
		}
	}
	//for 4 to 6 channels, store in two rows
	else if (channels.size() <= 6)
	{
		//concatenate first half of channels horizontally
		for(i = 1; i < ((channels.size()+1) / 2); i++)
		{
			hconcat(newSbSImage, myChImages.value(channels.at(i)), newSbSImage);
		}
		//concatenate second half of channels horizontally
		temp = myChImages.value(channels.at(((channels.size()+1) / 2)));
		for(i = ((channels.size()+3) / 2); i < channels.size(); i++)
		{
			hconcat(temp, myChImages.value(channels.at(i)), temp);
		}
		//if the number of channels is unequal, add a blank image
		if(channels.size() % 2 == 1)
		{
			hconcat(temp, Mat::zeros(myChImages.value(0).rows,
									 myChImages.value(0).cols,
									 myChImages.value(0).type()), temp);
		}
		//concatenate both halfs vertically
		vconcat(newSbSImage, temp, newSbSImage);
	}
	else
	{
		throw ImageHandlingException("Too many channels!");
	}

	return newSbSImage;
}

QMap<qint16, quint16> MultispectralImage::getPixelValueMap(quint16 x, quint16 y)
{
	QMap<qint16, quint16> values;
	quint8 i;

	QMapIterator<qint16, Mat> it(myChImages);
	while(it.hasNext())
	{
		it.next();
		values.insert(it.key(), it.value().at<quint16>(y, x));
	}

	QList<qint16> list = myChImages.keys();
	return values;
}

QList<quint16> MultispectralImage::getPixelValueList(quint16 x, quint16 y)
{
	QList<quint16> values;

	QMapIterator<qint16, Mat> it(myChImages);
	while(it.hasNext())
	{
		it.next();
		values.append(it.value().at<quint16>(y, x));
	}

	return values;
}

quint16 MultispectralImage::getWidth()
{
	return myWidth;
}

quint16 MultispectralImage::getHeight()
{
	return myHeight;
}

void MultispectralImage::setSize(quint16 width, quint16 height)
{
	myWidth = width;
	myHeight = height;

	//clear any contained images
	myChImages.clear();
}

quint8 MultispectralImage::getDepth()
{
	return myDepth;
}

void MultispectralImage::setDepth(quint8 depth)
{
	myDepth = depth;
}

quint8 MultispectralImage::getChannelCount()
{
	return myChImages.keys().length();
}

QList<qint16> MultispectralImage::getWavebands()
{
	return myChImages.keys();
}

void MultispectralImage::setFPNCorrected(bool en)
{
	isFPNCorrected = en;
}

bool MultispectralImage::getFPNCorrected()
{
	return isFPNCorrected;
}

void MultispectralImage::setDarkSubtracted(bool en)
{
	isDarkSubtracted = en;
}

bool MultispectralImage::getDarkSubtracted()
{
	return isDarkSubtracted;
}

void MultispectralImage::setWhiteCalibrated(bool en)
{
	isWhiteCalibrated = en;
}

bool MultispectralImage::getWhiteCalibrated()
{
	return isWhiteCalibrated;
}

