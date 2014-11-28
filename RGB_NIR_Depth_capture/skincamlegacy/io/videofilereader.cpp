#include "videofilereader.h"

VideoFileReader::VideoFileReader()
{
	myFrameCount = 0.0;
}

VideoFileReader::VideoFileReader(struct calibrationSettings settings)
	: FileReader(settings)
{
	myFrameCount = 0.0;
}

VideoFileReader::~VideoFileReader()
{
	//Nothing to do here - we did not create objects dynamically
}

void VideoFileReader::openSource(QString source)
{
	if(source.endsWith("metadata.txt"))
	{
		isImageSequence = true;
		mySourcePath = source;
		myPosition = 0;

		decodeMultispectralMetadata(source);
	}
	else
	{
		isImageSequence = false;

		Mat firstFrame;

		//load the video file
		mySourceFile.open( source.toStdString() );

		//check if loading was successfull
		if( !mySourceFile.isOpened() )
		{
			throw FileReaderException("Can not read source file", source);
		}

		//get framerate and total number of frames
		myFPS = mySourceFile.get(CV_CAP_PROP_FPS);
		if( !(myFPS>0) )
		{
			throw FileReaderException("Incorrect framerate in file", source);
		}

		myFrameCount = mySourceFile.get(CV_CAP_PROP_FRAME_COUNT);
		if( !(myFrameCount>0) )
		{
			throw FileReaderException("No frames in file", source);
		}

		//get first frame
		mySourceFile >> firstFrame;

		//initialize multispectral image object
		initializeFrame(firstFrame, source);
	}

	mySourceName = source;
}

MultispectralImage VideoFileReader::acquireNewFrame()
{
	//in order to keep the given framerate, check how much time has elapsed
	//since the last frame update and wait if necessary.
	QTime targetTime = QTime::currentTime().addMSecs(1000/myFPS);

	if(isImageSequence)
	{
		//load waveband images and add them to the new multispectral image
		for (int i = 0; i < myFrame.getChannelCount(); i++)
		{
			QString file = QString("%1/%2_%3.png").arg(mySourcePath).arg(myPosition,5,10,QChar('0')).arg(i);
			Mat img = imread(file.toStdString().c_str(), CV_LOAD_IMAGE_UNCHANGED);

			if(myCalSettings.applyFPNCal
					&& !sourceIsFPNCorr
					&& !sourceIsDarkSub)
			{
				applyFPNCalibration(img);
			}

			myFrame.setChannelImage(wavebandsFromMetadata.at(i), img);
		}

		if(myCalSettings.applyWhiteCal && !sourceIsWhiteCal)
		{
			applyWhiteCalibration();
		}

		if(++myPosition >= myFrameCount)
			myPosition = 0;
	}
	else
	{
		Mat newFrame;

		//capture next frame from source file,
		//if there are no new frames coming, return an empty frame object
		if (!(mySourceFile.read(newFrame)))
		{
			//reset position to start and try again
			mySourceFile.set(CV_CAP_PROP_POS_FRAMES, 0);
			if (!(mySourceFile.read(newFrame)))
				throw ImageSourceException("Can not read from file!");
		}
		else
		{
			//decode frame and load new image into active framebuffer slot
			decodeCVImage(newFrame);
		}
	}

	while( QTime::currentTime() < targetTime )
	{
		//process events if there are any for a maximum time of 5 ms
		QCoreApplication::processEvents(QEventLoop::AllEvents, 5);
	}

	return myFrame;
}

double VideoFileReader::getFrameCount()
{
	return myFrameCount;
}

void VideoFileReader::setPosition(double frame)
{
	if(isImageSequence)
	{
		myPosition = (quint32)frame;
	}
	else
	{
		//call cv-method to set capture properties and adjust pos in frames
		mySourceFile.set(CV_CAP_PROP_POS_FRAMES, frame);
	}
}

