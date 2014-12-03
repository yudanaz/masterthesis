#include "inout.h"

//static members
double InOut::spreadFactor = 1 / 226.0;
//endof static members

InOut::InOut()
{
}

QList<Mat> InOut::getImagesFromTarFile(QString source)
{
	QList<Mat> result;
	QProcess process;
	QStringList arguments;
	QDir tempDir;
	QString txt;
	QString filename;
	int i;

	QList<qint16> waveBands;
	waveBands.append(935);
	waveBands.append(1060);
	waveBands.append(1300);
	waveBands.append(1550);

	//cif not .tar-arquive, return empty list
	if (!source.endsWith(".tar"))
	{
		return result;
	}

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
	//qDebug() << "Computing: " << tempDir.absolutePath();

	//set external program and arguments according to current OS
#ifdef __linux__
	//Linux with tar:
	process.setProgram("tar");
	arguments << "-xf" << source << "-C" << tempDir.absolutePath();
#endif
	//run process
	process.setArguments(arguments);
	process.start();

	process.waitForStarted();
	process.waitForFinished();

	//load waveband images and add them to result
	for (i = 0; i < waveBands.size(); i++)
	{
		txt = QString("%1/%3.png").arg(tempDir.absolutePath())
			  .arg(waveBands[i]);
		Mat img = imread(txt.toStdString().c_str(), CV_LOAD_IMAGE_UNCHANGED);
		img.convertTo(img, CV_8U, spreadFactor); //convert to 8-bit
		result.append(img);
	}

	//delete temporary files and folders (without checking for errors)
	tempDir.removeRecursively();

	return result;
}
