#include "imagefilewriter.h"

ImageFileWriter::ImageFileWriter() : FileWriter()
{
	imageSettings.push_back(CV_IMWRITE_PNG_COMPRESSION);
	imageSettings.push_back(5);
}

ImageFileWriter::ImageFileWriter(QString path) : FileWriter(path)
{
	imageSettings.push_back(CV_IMWRITE_PNG_COMPRESSION);
	imageSettings.push_back(5);
}

ImageFileWriter::ImageFileWriter(QString path, QList<qint16> wavebands)
	: FileWriter(path, wavebands)
{
	imageSettings.push_back(CV_IMWRITE_PNG_COMPRESSION);
	imageSettings.push_back(5);
}

void ImageFileWriter::writeMatImage(Mat frame)
{
	QString filename;

	filename = targetFile;
	if(filename.endsWith(".png"))
	{
		filename.chop(4);
	}

	//add metadata to filename if given
	if(!filenameMetadata.isEmpty())
	{
		filename.append("_");
		filename.append(filenameMetadata);
	}

	//append .png as extension
	filename.append(".png");

	//write image to file
	if(!imwrite(filename.toStdString().c_str(), frame, imageSettings))
		throw FileWriterException("Could not write image", filename);
}

void ImageFileWriter::writeMultispectral(MultispectralImage frame)
{
	QProcess process;
	QStringList arguments;
	QDir dir;
	QString filename;
	quint8 i;

	if(targetFile.endsWith(".tar"))
		dir = QDir(targetFile.left(targetFile.length() - 4));
	else
		dir = QDir(targetFile);

	writeMetadataFile(dir, frame);

	for (i = 0; i < frame.getChannelCount(); i++)
	{
		//create openCV image files for each channel
		filename = QString("%1/%2.png").arg(dir.absolutePath()).arg(frame.getWavebands().at(i));
		imwrite(filename.toStdString().c_str(), frame.getImageByChannelNumber(i),
				imageSettings);
	}

	//If requested, create an archive
	if(targetFile.endsWith(".tar"))
	{
#ifdef __linux__
		process.setProgram("tar");
		arguments << "-cf"
				  << dir.absolutePath().append(".tar")
				  << "-C"
				  << dir.absolutePath().append("/")
				  << ".";
#endif
#ifdef _WIN32
		process.setProgram("bsdtar");
		arguments << "-cf"
				  << dir.toNativeSeparators(dir.absolutePath().append(".tar"))
				  << "-C"
				  << dir.toNativeSeparators(dir.absolutePath().append("\\"))
				  << ".";
#endif
		//run process
		process.setArguments(arguments);
		process.start();
		if(!process.waitForStarted())
			throw FileWriterException("Error creating .tar archive!",targetFile);
		if(!process.waitForFinished())
			throw FileWriterException("Error creating .tar archive!",targetFile);

		//Delete the temporary directory
		dir.removeRecursively();
	}
}



