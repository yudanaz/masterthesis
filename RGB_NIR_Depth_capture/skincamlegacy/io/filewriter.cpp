#include "filewriter.h"

FileWriter::FileWriter()
{
	targetFile = QDir::homePath().append("/out/");
}

FileWriter::FileWriter(QString file)
{
	targetFile = file;
}

FileWriter::FileWriter(QString file, QList<qint16> wavebands)
{
	targetFile = file;

	encodeMetadata(wavebands);
}

FileWriter::~FileWriter()
{
	;
}

void FileWriter::encodeMetadata(QList<qint16> wavebands)
{
	filenameMetadata = "(";
	for (int i = 0; i < wavebands.size(); i++)
	{
		filenameMetadata.append(QString("%1,").arg(wavebands.at(i)));
	}
	filenameMetadata.chop(1); //remove last ","
	filenameMetadata.append(")");
}

void FileWriter::writeMetadataFile(QDir targetDir, MultispectralImage frame, quint8 fps)
{
	//create path
	QFile metaFile(QString("%1/metadata.txt").arg(targetDir.absolutePath()));

	//Create a new directory on <path> where all following files
	//will be placed temporarily and store metadata in text file
	if ((targetDir.exists() || targetDir.mkdir(targetDir.absolutePath()))
			&& metaFile.open(QFile::WriteOnly | QFile::Text))
	{
		QTextStream out(&metaFile);
		out << "## Image Sequence ##" << endl;
		out << "Channels: " << frame.getChannelCount() << endl;
		out << "Width: " << frame.getWidth() << endl;
		out << "Height: " << frame.getHeight() << endl;
		out << "Depth: " << frame.getDepth() << endl;
		out << "Wavebands: ";

		for (quint8 i = 0; i < frame.getChannelCount(); i++)
		{
			//write wavebands to text file
			out << frame.getWavebands().at(i) << ",";
		}
		out << endl;

		out << "FPN Corr: " << frame.getFPNCorrected() << endl;
		out << "White Cal: " << frame.getWhiteCalibrated() << endl;
		out << "Dark Sub: " << frame.getDarkSubtracted() << endl;

		if(fps > 0)
		{
			out << "Framerate: " << fps << endl;
		}
	}
	else
	{
		throw FileWriterException("Path or metadata.txt could not be created",
								  targetFile);
	}

	metaFile.close();
}
