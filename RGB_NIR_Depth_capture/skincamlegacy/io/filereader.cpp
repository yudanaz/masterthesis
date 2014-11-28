#include "filereader.h"

FileReader::FileReader()
{
	darkGiven = true;
}

FileReader::FileReader(calibrationSettings settings) : ImageSource(settings)
{
	darkGiven = true;
}

FileReader::~FileReader()
{
	//Nothing to do here - we did not create objects dynamically
}

void FileReader::initializeFrame(Mat srcImage, QString srcName)
{
	//for this type of image source, we assume that FPN correction,
	//white reference calibration and dark subtraction have already bin performed
	myFrame.setFPNCorrected(true);
	myFrame.setWhiteCalibrated(true);
	myFrame.setDarkSubtracted(true);
	sourceIsFPNCorr = true;
	sourceIsWhiteCal = true;
	sourceIsDarkSub = true;

	//if this is a false color image (3 channels in source):
	if (srcImage.channels() == 3)
	{
		myFrame.setDepth(8);

		//decode metadata from string
		try
		{
			wavebandsFromMetadata = decodeFilenameMetadata(srcName);
		}
		catch (invalid_argument e)
		{
			qDebug() << "Warning: " << e.what();

			//use default wavebands for false color (just numbering)
			wavebandsFromMetadata << 1 << 2 << 3;
		}

		//check for correct number of wavebands:
		if (wavebandsFromMetadata.size() != 3)
		{
			throw FileReaderException("Wrong number of channels for false "\
									  "color image!", srcName);
		}

		myFrame.setSize(srcImage.cols, srcImage.rows);
	}
	else //this means we have a side by side image
	{
		if(srcImage.depth() == CV_8U)
			myFrame.setDepth(8);
		else if(srcImage.depth() == CV_16U)
			myFrame.setDepth(16);
		else
			throw FileReaderException("Source image has incompatible color depth");

		//decode metadata from string
		try
		{
			wavebandsFromMetadata = decodeFilenameMetadata(srcName);
		}
		catch (invalid_argument e)
		{
			qDebug() << "Warning: " << e.what();

			//use default wavebands for four wavebands (just numbering)
			wavebandsFromMetadata << 1 << 2 << 3 << 4;
		}

		//size depends on number of wavebands:
		//up to 3 bands are in one row:
		if (wavebandsFromMetadata.size() <= 3)
		{
			myFrame.setSize(srcImage.cols/wavebandsFromMetadata.size(), srcImage.rows);
		}
		//4 to 6 bands are in two rows:
		else if (wavebandsFromMetadata.size() <= 6)
		{
			myFrame.setSize(srcImage.cols/((wavebandsFromMetadata.size()+1)/2), srcImage.rows/2);
		}
		else
			throw FileReaderException("Too many channels", srcName);
	}

	//prepend dark channel, if it is not part of the bands
	if(wavebandsFromMetadata.at(0) != 0)
	{
		darkGiven = false;
		wavebandsFromMetadata.prepend(0);
	}
}

void FileReader::decodeCVImage (Mat srcImage)
{
	if ((srcImage.channels() != 3) && (srcImage.channels() != 1))
	{
		throw FileReaderException("Wrong number of channels");
	}

	if (srcImage.channels() == 3)
	{
		decodeFalseColorImage(srcImage);
	}
	else
	{
		decodeSideBySideImage(srcImage);
	}
}

void FileReader::decodeFalseColorImage (Mat srcImage)
{
	vector<Mat> rgb(3);

	try
	{
		//add dark channel:
		myFrame.setChannelImage(0, Mat::zeros(srcImage.rows, srcImage.cols,
											  CV_8UC1));
		//copy RGB images into channels 1 to 3
		//Attention: as false color RGB representation is reversed, we have to turn
		//it around when setting the channel for the multispectral image!!
		split(srcImage, rgb);
		myFrame.setChannelImage(wavebandsFromMetadata.at(1), rgb.at(2));
		myFrame.setChannelImage(wavebandsFromMetadata.at(2), rgb.at(1));
		myFrame.setChannelImage(wavebandsFromMetadata.at(3), rgb.at(0));
	}
	catch (ImageSizeException e)
	{
		throw FileReaderException(QString("Error decoding false color image: ")
								  .arg(e.getMessage()));
	}
	catch (out_of_range e)
	{
		throw FileReaderException("Error decoding false color image: "\
								  "Multispectral Image not properly initialized!");
	}
}

void FileReader::decodeSideBySideImage(Mat srcImage)
{
	quint8 row, nRows, col, nCols;
	quint8 nCh;
	quint8 darkCor = 0;

	nCh = wavebandsFromMetadata.length();

	//note that we will get one image less if the dark image is not given
	if (!darkGiven)
	{
		nCh--;
		darkCor = 1;
	}

	//2 and 3 channels: in a horizontal row
	if (nCh <= 3)
	{
		nRows = 1;
		nCols = nCh;
	}
	//4 to 6 channels: in an array with height 2
	else if (nCh <= 6)
	{
		nRows = 2;
		nCols = (nCh+1) / 2;
	}
	else
		throw FileReaderException("Too many channels");

	//decode image content

	//convert color to gray if RGB input
	if(srcImage.channels() > 1) {
		cvtColor(srcImage, srcImage, CV_BGR2GRAY);
	}

	//run through columns and rows of the side by side image and extract slices:
	for (col = 0; col < nCols; col++)
	{
		for (row = 0; row < nRows; row++)
		{
			myFrame.setChannelImage(wavebandsFromMetadata.at(darkCor + col + row),
									Mat(srcImage, Rect(myFrame.getWidth() * col,
													   myFrame.getHeight() * row,
													   myFrame.getWidth(),
													   myFrame.getWidth())));
		}
	}
}

QList<qint16> FileReader::decodeFilenameMetadata(QString source)
{
	qint8 metaStart, metaEnd;
	QStringList dataTxt;
	QList<qint16> dataInt;
	bool ok;

	//Format: "(<channel_1>,<channel_2>,...,<channel_n>)", e.g. "(1060,1300,1550)"
	metaStart = source.lastIndexOf('(');
	metaEnd = source.lastIndexOf(')');
	if ((metaStart >= 0) && (metaEnd > 0))
	{
		//Metadata is the text without braces, separated by comma
		dataTxt = source.mid(metaStart + 1, metaEnd - metaStart -1).split(',');

		while (!dataTxt.isEmpty())
		{
			dataInt.append(dataTxt.first().toInt(&ok));
			if (!ok)
			{
				throw invalid_argument("Metadata invalid");
			}
			dataTxt.removeFirst();
		}
	}
	else
	{
		throw invalid_argument("No Metadata given!");
	}

	return dataInt;
}

void FileReader::decodeMultispectralMetadata(QString source)
{
	QString txt;
	int i;
	bool convOk;

	quint8 channels = 0;
	quint16 width = 0;
	quint16 height = 0;
	quint8 depth = 0;

	wavebandsFromMetadata.clear();

	QFile metaFile(source);

	if (metaFile.open(QFile::ReadOnly | QFile::Text))
	{
		//create input text stream from file
		QTextStream in(&metaFile);
		//read all lines until the end is reached
		while (!in.atEnd())
		{
			txt = in.readLine();

			if (txt.isEmpty())
				continue;

			if (txt.startsWith("Channels"))
			{
				channels = txt.mid(txt.lastIndexOf(":") + 1).toInt(&convOk);
				if(!convOk)
					throw FileReaderException("Could not read number of channels!",
											  source);
			}
			else if (txt.startsWith("Width"))
			{
				width = txt.mid(txt.lastIndexOf(":") + 1).toInt(&convOk);
				if(!convOk)
					throw FileReaderException("Could not read width!", source);
			}
			else if (txt.startsWith("Height"))
			{
				height = txt.mid(txt.lastIndexOf(":") + 1).toInt(&convOk);
				if(!convOk)
					throw FileReaderException("Could not read height!", source);
			}
			else if (txt.startsWith("Depth"))
			{
				depth = txt.mid(txt.lastIndexOf(":") + 1).toInt(&convOk);
				if(!convOk)
					throw FileReaderException("Could not read depth!", source);
			}
			else if (txt.startsWith("Wavebands"))
			{
				QStringList bands = txt.mid(txt.lastIndexOf(":") + 1).split(",");
				for (i = 0; i < bands.count(); i++)
				{
					if(!bands.at(i).isEmpty())
					{
						wavebandsFromMetadata.append( bands.at(i).toInt(&convOk) );
						if(!convOk)
							throw FileReaderException("Could not read wavebands!",
													  source);
					}
				}
			}
			else if (txt.startsWith("FPN"))
			{
				sourceIsFPNCorr = txt.mid(txt.lastIndexOf(":") + 1).toInt(&convOk);
				if(!convOk)
					throw FileReaderException("Could not read FPN status!", source);
			}
			else if (txt.startsWith("White"))
			{
				sourceIsWhiteCal = txt.mid(txt.lastIndexOf(":") + 1).toInt(&convOk);
				if(!convOk)
					throw FileReaderException("Could not read White Calibration status!", source);
			}
			else if (txt.startsWith("Dark"))
			{
				sourceIsDarkSub = txt.mid(txt.lastIndexOf(":") + 1).toInt(&convOk);
				if(!convOk)
					throw FileReaderException("Could not read Dark Subtraction status!", source);
			}
			else if (txt.startsWith("Framerate"))
			{
				myFPS = txt.mid(txt.lastIndexOf(":") + 1).toInt(&convOk);
				if(!convOk)
					throw FileReaderException("Could not read framerate!", source);
			}
		}
	}
	else
		throw FileReaderException("Error reading metadata!", source);

	metaFile.close();

	//check metadata for completeness
	if(!(channels > 0) || !(width > 0) || !(height > 0) || !(depth > 0)
			|| !(wavebandsFromMetadata.size() == channels))
		throw FileReaderException("Error reading metadata!", source);

	myFrame.setSize(width, height);
	myFrame.setDepth(depth);
	myFrame.setFPNCorrected(sourceIsFPNCorr);
	myFrame.setWhiteCalibrated(sourceIsWhiteCal);
	myFrame.setDarkSubtracted(sourceIsDarkSub);
}
