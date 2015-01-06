#include "imagesource.h"

ImageSource::ImageSource()
{
	myFPS = 0;

	myCalSettings.applyFPNCal = false;
	myCalSettings.subtractDark = true;
	myCalSettings.applyWhiteCal = false;
	myCalSettings.fpnRefFile = QString("%1/fpnRef.tar").arg(QDir::homePath());

	myFPNcorr.refImages = NULL;
	myFPNcorr.offsets = NULL;
	myFPNcorr.scales = NULL;
	myFPNcorr.count = 0;
	myFPNcorrLoaded = false;

	sourceIsDarkSub = false;
	sourceIsFPNCorr = false;
	sourceIsWhiteCal = false;
}

ImageSource::ImageSource(struct calibrationSettings settings)
{
	myFPS = 0;
	myFPNcorr.refImages = NULL;
	myFPNcorr.offsets = NULL;
	myFPNcorr.scales = NULL;
	myFPNcorr.count = 0;
	myFPNcorrLoaded = false;

	calLock.lockForWrite();

	myCalSettings = settings;

	calLock.unlock();

	if(settings.applyFPNCal)
	{
		loadFPNReference();
	}
}

ImageSource::~ImageSource()
{
	if (myFPNcorr.offsets != NULL)
		delete[] myFPNcorr.offsets;
	if (myFPNcorr.scales != NULL)
		delete[] myFPNcorr.scales;
	if (myFPNcorr.refImages != NULL)
		delete[] myFPNcorr.refImages;
}

quint8 ImageSource::getChannels()
{
	return myFrame.getChannelCount();
}

QList<qint16> ImageSource::getWavebands()
{
	return myFrame.getWavebands();
}

quint8 ImageSource::getFPS()
{
	return myFPS;
}

void ImageSource::setImageCalibration(calibrationSettings newCal)
{
	calLock.lockForWrite();

	if(newCal.fpnRefFile.compare(myCalSettings.fpnRefFile) != 0)
	{
		if (myFPNcorr.offsets != NULL)
			delete[] myFPNcorr.offsets;
		if (myFPNcorr.scales != NULL)
			delete[] myFPNcorr.scales;
		if (myFPNcorr.refImages != NULL)
			delete[] myFPNcorr.refImages;

		myFPNcorr.refImages = NULL;
		myFPNcorr.offsets = NULL;
		myFPNcorr.scales = NULL;

		myFPNcorr.count = 0;

		myFPNcorrLoaded = false;
	}

	myCalSettings = newCal;

	if(newCal.applyFPNCal && !myFPNcorrLoaded)
	{
		try
		{
			loadFPNReference();
		}
		catch (...)
		{
			calLock.unlock();

			throw;
		}
	}

	calLock.unlock();
}

struct calibrationSettings ImageSource::getCurrentCalibration()
{
	calLock.lockForRead();
//	cout << "Lock: getCurrentCalibration" << endl;
	struct calibrationSettings temp = myCalSettings;
	calLock.unlock();
//	cout << "Unlock: getCurrentCalibration" << endl;
	return temp;
}

void ImageSource::setWhiteReference(CvPoint refPos)
{
	quint8 i;
	quint8 refChannel = 0;
	double max;
	QList<double> vals;

	QList<qint16> bands = myFrame.getWavebands();

	calLock.lockForWrite();
//	cout << "Lock: setWhiteReference" << endl;

	//choose channel with highest (uncorrected) value factor as reference
	max = 0;
	for (i = 1; i < myFrame.getChannelCount(); i++)
	{
		vals.append((double)myFrame.getImageByChannelNumber(i)
					.at<quint16>(refPos.y, refPos.x));
		if(vals.last() > max)
		{
			max = vals.last();
			refChannel = i;
		}
	}

	//calculate new calibration factors based on highest channel, ignoring dark channel:
	for (i = 1; i < myFrame.getChannelCount(); i++)
	{
		if (i == refChannel)
		{
			myCalSettings.whiteCal[bands.at(i)] = 1.0;
		}
		else
		{
			myCalSettings.whiteCal[bands.at(i)] = (max / vals[i-1]);
		}
	}

	//assume this means that we want to use white calibration
	myCalSettings.applyWhiteCal = true;

	//force re-calibration even for pre-calibrated image sources
	sourceIsWhiteCal = false;

	calLock.unlock();
//	cout << "Unlock: setWhiteReference" << endl;
}

/***
 * This method has been modified from the original:
 * fixed pattern noise reference file URL has been hard-coded for simplicity
 * */
void ImageSource::loadFPNReference()
{
    return;
    QProcess process;
	QStringList arguments;
	QDir imageDir;
	QFileInfo archive;

//	archive = QFileInfo(myCalSettings.fpnRefFile);
	archive = QFileInfo("fpnRef.tar");
	if(!archive.exists())
		throw ImageSourceException("Given FPN reference archive does not exist!");

	//extract reference image archive into a temporary directory
	imageDir = QDir(QDir::tempPath().append("/").append("scFPNref"));
	if(imageDir.exists())
	{
		imageDir.removeRecursively();
	}
	imageDir.mkpath(imageDir.absolutePath());

	//set external program and arguments according to current OS
#ifdef __linux__
	//Linux with tar:
	process.setProgram("tar");
	arguments << "-xf" << "fpnRef.tar" << "-C" << imageDir.absolutePath();
#endif
#ifdef _WIN32
	//Windows with bsdtar installed:
	//get native directory separators (\ instead of /)
	process.setProgram("bsdtar");
	arguments << "-xf" << imageDir.toNativeSeparators(myCalSettings.fpnRefFile)
			  << "-C" << imageDir.toNativeSeparators(imageDir.absolutePath());
#endif

	//run process
	process.setArguments(arguments);
	process.start();
	if(!process.waitForStarted())
		throw ImageSourceException("Could not open FPN reference archive!");
	if(!process.waitForFinished())
		throw ImageSourceException("Could not open FPN reference archive!");

//	qDebug() << "Archive extracted";

	//get list of reference images
	QStringList fileList = imageDir.entryList(QStringList() << "*.tif",
											 QDir::Files,
											 QDir::Name);

	if(fileList.length() == 0)
		throw ImageSourceException("FPN reference archive is empty!");

	//create correction structure
	myFPNcorr.count = fileList.length() - 1;
	myFPNcorr.scales = new Mat[myFPNcorr.count];
	myFPNcorr.offsets = new Mat[myFPNcorr.count];
	myFPNcorr.refImages = new Mat[myFPNcorr.count + 1];

	//read all reference images and create arrays of Mat and target values
	quint16 targetVals[fileList.length()];
	for (int i = 0; i < fileList.length(); i++)
	{
		myFPNcorr.refImages[i] = imread(QString("%1/%2")
							.arg(imageDir.absolutePath())
							.arg(fileList.at(i)).toStdString(),
							CV_LOAD_IMAGE_UNCHANGED);

		QString name = fileList[i];
		name.chop(4);
		targetVals[i] = (name.toInt())<<4;
	}

//	qDebug() << "Reference images loaded";

	//calculate scale and offset for each pair of reference images
	Mat diffReal, tempScaled;
	qint32 diffTarget;

	for(quint16 i = 0; i < myFPNcorr.count; i++)
	{
		subtract(myFPNcorr.refImages[i+1], myFPNcorr.refImages[i],
				 diffReal, noArray(), CV_32S);
		diffTarget = (qint32)targetVals[i+1] - targetVals[i];

		divide((double)diffTarget, diffReal, myFPNcorr.scales[i], CV_32F);

		multiply(myFPNcorr.refImages[i], myFPNcorr.scales[i], tempScaled, 1, CV_32F);
		subtract(Scalar(targetVals[i]), tempScaled, myFPNcorr.offsets[i], noArray(), CV_32S);
	}

//	quint16 in1 = myFPNcorr.refImages[69].at<quint16>(200,100);
//	quint16 in2 = myFPNcorr.refImages[70].at<quint16>(200,100);

//	quint16 targ1 = targetVals[69];
//	quint16 targ2 = targetVals[70];

//	float scale = myFPNcorr.scales[69].at<float>(200,100);
//	qint32 offset = myFPNcorr.offsets[69].at<qint32>(200,100);

	//set flag
	myFPNcorrLoaded = true;

	//clean up
	imageDir.removeRecursively();

//	qDebug() << "Correction matrix created";

/******************************************************************************
 * The following code is based on a 10th-degree polynom used to calculate the
 * required correction offset for a given pixel value. It loads a yml file
 * containing a 3-dimensional openCV matrix with 11 coefficients for every image
 * pixel.
 *
	FileStorage fs(myCalSettings.fpnRefFile.toStdString(), FileStorage::READ);
	if(fs.isOpened())
	{
		fs["polyMatrix"] >> myFPNcorr;
		fs.release();

		if(myFPNcorr.dims != 3)
		{
			myCalSettings.applyFPNCal = false;
			throw ImageSourceException("Error reading correction matrix from FPN reference file!");
		}
	}
	else
	{
		myCalSettings.applyFPNCal = false;
		throw ImageSourceException("Could not open FPN reference file!");
	}
*******************************************************************************/
}

void ImageSource::applyWhiteCalibration ()
{
	Mat temp;
	quint8 i;

	calLock.lockForRead();
//	cout << "Lock: applyWhiteCalibration" << endl;

	QList<qint16> bands = myFrame.getWavebands();

	//Perform white shift for all channels except dark
	for (i = 1; i < myFrame.getChannelCount(); i++)
	{
		if(myCalSettings.whiteCal.contains(bands.at(i)))
		{
			myFrame.getImageByWL(bands.at(i)).convertTo(temp, -1, myCalSettings.whiteCal[bands.at(i)]);
			myFrame.setChannelImage(bands.at(i), temp.clone());
		}
	}

	calLock.unlock();
//	cout << "Unlock: applyWhiteCalibration" << endl;

	myFrame.setWhiteCalibrated(true);
}

void ImageSource::applyFPNCalibration(MultispectralImage image)
{
//	QTime timer;
//	timer.start();

	if(!myFPNcorrLoaded)
		return;

	QMapIterator<qint16, Mat> it (image.getImages());

	while(it.hasNext())
	{
		it.next();
		applyFPNCalibration(it.value());
	}

	image.setFPNCorrected(true);

//	qDebug() << "Time taken for FPN calibration: " << timer.restart();
}

void ImageSource::applyFPNCalibration (Mat image)
{
	quint16 x, y, p;
	qint32 res;

	if(image.empty() || image.depth() != CV_16U || !myFPNcorrLoaded)
		return;

	calLock.lockForRead();

	qint8 refNum, low, up;

	for (y = 0; y < image.rows; y++)
	{
		for (x = 0; x < image.cols; x++)
		{
			p = image.at<quint16>(y,x);

			up = myFPNcorr.count;
			low = 0;

			while((up-low) > 1)
			{
				refNum = ((up-low)>>1) + low;
				if(p < myFPNcorr.refImages[refNum].at<quint16>(y,x))
				{
					up = refNum;
				}
				else
				{
					low = refNum;
				}
			}

			float scale = myFPNcorr.scales[low].at<float>(y,x);
			qint32 offset = myFPNcorr.offsets[low].at<qint32>(y,x);

			res = (p * scale + offset);

			if (res > 65535)
				image.at<quint16>(y,x) = 65535;
			else if (res < 0)
				image.at<quint16>(y,x) = 0;
			else
				image.at<quint16>(y,x) = (quint16)res;
		}
	}

/******************************************************************************
 * The following code is based on a 10th-degree polynom used to calculate the
 * required correction offset for a given pixel value.
 *
 * Pros: very fast (2-3ms), created polynom smoothes the reference values.
 * Cons: possibly inaccurate, as it does not model the reference data well enough.
 *
	double p;

	if(image.depth() == 16)
	{
		for (y = 0; y < image.rows; y++)
			for (x = 0; x < image.cols; x++)
			{
				p = (double)image.at<quint16>(y,x);

				double *coeffs = myFPNcorr.at<double[11]>(y,x);

				p = p +   coeffs[0] * pow(p, 10)
						+ coeffs[1] * pow(p, 9)
						+ coeffs[2] * pow(p, 8)
						+ coeffs[3] * pow(p, 7)
						+ coeffs[4] * pow(p, 6)
						+ coeffs[5] * pow(p, 5)
						+ coeffs[6] * pow(p, 4)
						+ coeffs[7] * pow(p, 3)
						+ coeffs[8] * pow(p, 2)
						+ coeffs[9] * p
						+ coeffs[10];

				image.at<quint16>(y,x) = (quint16)p;
			}
	}
*******************************************************************************/

	calLock.unlock();
}

bool ImageSource::getSourceIsDarkSub()
{
	return sourceIsDarkSub;
}

bool ImageSource::getSourceIsFPNCorr()
{
	return sourceIsFPNCorr;
}

bool ImageSource::getSourceIsWhiteCal()
{
	return sourceIsWhiteCal;
}

QString ImageSource::getSourceName()
{
	return mySourceName;
}

