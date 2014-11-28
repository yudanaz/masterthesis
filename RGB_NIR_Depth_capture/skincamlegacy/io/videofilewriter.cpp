#include "videofilewriter.h"


VideoFileWriter::VideoFileWriter(quint8 fps) : FileWriter()
{
	myFPS = fps;
}


VideoFileWriter::VideoFileWriter(QString file, quint8 fps) : FileWriter(file)
{
	myFPS = fps;
}


VideoFileWriter::VideoFileWriter(QString file, QList<qint16> wavebands, quint8 fps)
	: FileWriter(file, wavebands)
{
	myFPS = fps;
}


VideoFileWriter::~VideoFileWriter()
{
	;
}


void VideoFileWriter::writeMultispectral (MultispectralImage frame)
{
	QDir dir = QDir(targetFile);

	static bool initialized = false;
	static quint32 count = 0;

	QString filename;
	quint8 i;

	//create folder and metadata file
	if(!initialized)
	{
		writeMetadataFile(dir, frame, myFPS);
	}

	//create openCV image files for each channel
	for(i = 0; i < frame.getChannelCount(); i++)
	{
		filename = QString("%1/%2_%3.tif").arg(targetFile)
				   .arg(count,5,10,QChar('0')).arg(i);
		imwrite(filename.toStdString().c_str(), frame.getImageByChannelNumber(i));
	}

	count++;
}

void VideoFileWriter::writeMatImage (Mat frame)
{
	if(!myVideoWriter.isOpened())
	{
		if(!open(Size(frame.cols, frame.rows), frame.channels()==3))
			throw FileWriterException("Could not open target file", targetFile);
	}

	myVideoWriter.write(frame);
}

bool VideoFileWriter::open(Size size, bool isColor)
{
	QFileInfo file = QFileInfo(targetFile);
	QDir dir = file.absolutePath();
	if(!dir.exists())
	{
		if(!dir.mkdir(file.absolutePath()))
			return false;
	}

	QString filename = targetFile;
	if(filename.endsWith(".avi"))
	{
		filename.chop(4);
	}
	filename.append("_");
	filename.append(filenameMetadata);
	filename.append(".avi");

	return myVideoWriter.open(filename.toStdString(), CV_FOURCC('F','F','V','1'),
							  myFPS, size, isColor);
}
