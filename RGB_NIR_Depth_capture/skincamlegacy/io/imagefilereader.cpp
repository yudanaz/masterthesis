#include "imagefilereader.h"

ImageFileReader::ImageFileReader()
{
	myFPS = 0; // 0 means still image without updates.
}

ImageFileReader::ImageFileReader(calibrationSettings settings)
	: FileReader(settings)
{
	myFPS = 0; // 0 means still image without updates.
}

ImageFileReader::~ImageFileReader()
{
	//Nothing to do here - we did not create objects dynamically
}

MultispectralImage ImageFileReader::acquireNewFrame()
{
	//reset myFrame to a copy (cloned!) of the original image.
	myFrame = MultispectralImage(origImage, true);

	if(myCalSettings.applyFPNCal
			&& !sourceIsFPNCorr
			&& !sourceIsDarkSub)
	{
		applyFPNCalibration(myFrame);
	}

	if(myCalSettings.applyWhiteCal
			&& !sourceIsWhiteCal
			&& sourceIsDarkSub)
	{
		applyWhiteCalibration();
	}

	return myFrame;
}

void ImageFileReader::openSource (QString source)
{
	Mat srcImage;

	try
	{
		//now read either a multispectral image file or an openCV image file:
		if (source.endsWith(".tar", Qt::CaseInsensitive)
				|| source.endsWith("metadata.txt", Qt::CaseInsensitive))
		{
			readMultispectral(source);
			origImage = myFrame;
		}
		else if (source.endsWith(".jpg", Qt::CaseInsensitive)
				 || source.endsWith(".png", Qt::CaseInsensitive)
				 || source.endsWith(".bmp", Qt::CaseInsensitive))
		{
			srcImage = imread(source.toStdString().c_str(),
							  CV_LOAD_IMAGE_UNCHANGED);
			initializeFrame(srcImage, source);
			decodeCVImage(srcImage);
			origImage = myFrame;
		}
		else
			throw FileReaderException("Unknown image format!", source);
	}
	catch (FileReaderException e)
	{
		e.setSource(source);
		throw;
	}

	mySourceName = source;
}

void ImageFileReader::readMultispectral(QString source)
{
	QProcess process;
	QStringList arguments;
	QDir tempDir;
	QString txt;
	QString filename;
	int i;

	//check if we have been given a tar archive or a path to a metadata file
	if (source.endsWith(".tar"))
	{
		//extract archive into a temporary directory

		//get filename without file ending from full path (source)
		filename = source.mid(source.lastIndexOf("/") + 1);
		filename.chop(4);

		//create temporary directory
		tempDir = QDir(QDir::tempPath().append("/").append(filename));
		if(tempDir.exists())
		{
			tempDir.removeRecursively();
		}
		tempDir.mkpath(tempDir.absolutePath());

		//set external program and arguments according to current OS
#ifdef __linux__
		//Linux with tar:
		process.setProgram("tar");
		arguments << "-xf" << source << "-C" << tempDir.absolutePath();
#endif
#ifdef _WIN32
		//Windows with bsdtar installed:
		//get native directory separators (\ instead of /)
		process.setProgram("bsdtar");
		arguments << "-xf" << tempDir.toNativeSeparators(source)
				  << "-C" << tempDir.toNativeSeparators(tempDir.absolutePath());
#endif

		//run process
		process.setArguments(arguments);
		process.start();
		if(!process.waitForStarted())
			throw FileReaderException("Could not open archive!", source);
		if(!process.waitForFinished())
			throw FileReaderException("Could not open archive!", source);
	}
	else
	{
		//set tempDir to be source without the filename
		tempDir = QDir(source.mid(0, source.lastIndexOf("/")));
	}

	//open and read metadata file and prepare myFrame
	decodeMultispectralMetadata(QString("%1/metadata.txt").arg(tempDir.absolutePath()));

	//load waveband images and add them to the new multispectral image
	for (i = 0; i < wavebandsFromMetadata.length(); i++)
	{
		txt = QString("%1/%3.png").arg(tempDir.absolutePath())
			  .arg(wavebandsFromMetadata.at(i));
		Mat img = imread(txt.toStdString().c_str(), CV_LOAD_IMAGE_UNCHANGED);

		myFrame.setChannelImage(wavebandsFromMetadata.at(i), img);
	}

		//if source was a tar archive,
	//delete temporary files and folders (without checking for errors)
	if(source.endsWith(".tar"))
	{
		tempDir.removeRecursively();
	}
}
