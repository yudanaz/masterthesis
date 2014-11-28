#include "skincamconfig.h"

SkinCamConfig::SkinCamConfig()
{
	setDefaults();
}

SkinCamConfig::SkinCamConfig(QString filename)
{
	QString txt;
	QStringList values;
	bool convOk;
	int i;

	//set defaults in case anything is not configured in the file
	setDefaults();

	//open given config file
	QFile file(filename);

	if (file.open(QFile::ReadOnly | QFile::Text))
	{
		//create input text stream from file
		QTextStream in(&file);
		//read all lines until the end is reached
		while (!in.atEnd())
		{
			txt = in.readLine();

			if (txt.isEmpty())
				continue;

			if (txt.startsWith("#"))
				continue;

			if (txt.startsWith("Auto detect"))
			{
				cameraConfig.autoCameraSearch = txt.mid(txt.lastIndexOf(":") + 2).toInt(&convOk);
				if(!convOk)
					throw SkinCamException("Could not read parameter: camera auto detection!");
			}
			else if (txt.startsWith("IP"))
			{
				cameraConfig.IPadress = txt.mid(txt.lastIndexOf(":") + 2);
			}
			else if (txt.startsWith("Ringlight"))
			{
				cameraConfig.ringlightInterface = txt.mid(txt.lastIndexOf(":") + 2);
			}
			else if (txt.startsWith("Wavebands"))
			{
				cameraConfig.wavebands.config.clear();
				//add all given bands:
				quint8 enCount = 0;
				values = txt.mid(txt.lastIndexOf(":") + 2).split(";");
				for (i = 0; i < values.count(); i++)
				{
					if(!values.at(i).isEmpty())
					{
						bool enabled = values.at(i).section('=',1,1).toInt(&convOk);
						cameraConfig.wavebands.config.insert(
									values.at(i).section('=',0,0).toInt(&convOk),
									enabled );
						if(enabled)
							enCount++;
						if(!convOk)
							throw SkinCamException("Could not read parameter: available wavebands!");
					}
				}
				cameraConfig.wavebands.totalNumber = cameraConfig.wavebands.config.size();
				cameraConfig.wavebands.usedNumber = enCount;
			}
			else if (txt.startsWith("Frequency"))
			{
				cameraConfig.frequency = txt.mid(txt.lastIndexOf(":") + 2).toInt(&convOk);
				if(!convOk)
					throw SkinCamException("Could not read parameter: frequency!");
			}
			else if (txt.startsWith("Integration Time"))
			{
				cameraConfig.integrationTime = txt.mid(txt.lastIndexOf(":") + 2).toInt(&convOk);
				if(!convOk)
					throw SkinCamException("Could not read parameter: Integration Time!");
			}
			else if (txt.startsWith("Buffer"))
			{
				cameraConfig.bufferSize = txt.mid(txt.lastIndexOf(":") + 2).toInt(&convOk);
				if(!convOk)
					throw SkinCamException("Could not read parameter: Buffer Size!");
			}
			else if (txt.startsWith("FPN Ref"))
			{
				calibration.fpnRefFile = txt.mid(txt.lastIndexOf(":") + 2);
			}
			else if (txt.startsWith("FPN Enabled"))
			{
				calibration.applyFPNCal = txt.mid(txt.lastIndexOf(":") + 2).toInt(&convOk);
				if(!convOk)
					throw SkinCamException("Could not read parameter: apply FPN!");
			}
			else if (txt.startsWith("Subtract"))
			{
				calibration.subtractDark = txt.mid(txt.lastIndexOf(":") + 2).toInt(&convOk);
				if(!convOk)
					throw SkinCamException("Could not read parameter: Subtract Dark!");
			}
			else if (txt.startsWith("White Calibration"))
			{
				calibration.whiteCal.clear();
				values = txt.mid(txt.lastIndexOf(":") + 2).split(";");
				for (i = 0; i < values.count(); i++)
				{
					if(!values.at(i).isEmpty())
					{
						calibration.whiteCal.insert( values.at(i).section('=',0,0).toInt(&convOk),
													 values.at(i).section('=',1,1).toDouble(&convOk) );
						if(!convOk)
							throw SkinCamException("Could not read parameter: white calibration!");
					}
				}
			}
			else if (txt.startsWith("White Cal Enabled"))
			{
				calibration.applyWhiteCal = txt.mid(txt.lastIndexOf(":") + 2).toInt(&convOk);
				if(!convOk)
					throw SkinCamException("Could not read parameter: apply white cal!");
			}
			else if (txt.startsWith("Last"))
			{
				lastDir = txt.mid(txt.lastIndexOf(":") + 2);
			}
			else if (txt.startsWith("Video"))
			{
				videoTarget = txt.mid(txt.lastIndexOf(":") + 2);
			}
			else if (txt.startsWith("Stage Interface"))
			{
				stage.serialInt = txt.mid(txt.lastIndexOf(":") + 2);
			}
			else if (txt.startsWith("Stage Target"))
			{
				stage.targetFolder = txt.mid(txt.lastIndexOf(":") + 2);
			}
			else if (txt.startsWith("Stage Start"))
			{
				stage.start = txt.mid(txt.lastIndexOf(":") + 2).toInt(&convOk);
				if(!convOk)
					throw SkinCamException("Could not read parameter: Stage Start Pos!");
			}
			else if (txt.startsWith("Stage End"))
			{
				stage.end = txt.mid(txt.lastIndexOf(":") + 2).toInt(&convOk);
				if(!convOk)
					throw SkinCamException("Could not read parameter: Stage End Pos!");
			}
			else if (txt.startsWith("Stage Step"))
			{
				stage.stepwidth = txt.mid(txt.lastIndexOf(":") + 2).toInt(&convOk);
				if(!convOk)
					throw SkinCamException("Could not read parameter: Stage Stepwidth!");
			}
			else if (txt.startsWith("Stage Num"))
			{
				stage.numImg = txt.mid(txt.lastIndexOf(":") + 2).toInt(&convOk);
				if(!convOk)
					throw SkinCamException("Could not read parameter: Stage Num Images!");
			}
			else if (txt.startsWith("Stage Delay"))
			{
				stage.delay = txt.mid(txt.lastIndexOf(":") + 2).toInt(&convOk);
				if(!convOk)
					throw SkinCamException("Could not read parameter: Stage Delay!");
			}
			else if (txt.startsWith("Stage Inv"))
			{
				stage.inverted = txt.mid(txt.lastIndexOf(":") + 2).toInt(&convOk);
				if(!convOk)
					throw SkinCamException("Could not read parameter: Stage Inverted!");
			}
			else if (txt.startsWith("Stage Out MI"))
			{
				stage.mi = txt.mid(txt.lastIndexOf(":") + 2).toInt(&convOk);
				if(!convOk)
					throw SkinCamException("Could not read parameter: Stage Out MI!");
			}
			else if (txt.startsWith("Stage Out FC"))
			{
				stage.fc = txt.mid(txt.lastIndexOf(":") + 2).toInt(&convOk);
				if(!convOk)
					throw SkinCamException("Could not read parameter: Stage Out FC!");
			}
			else if (txt.startsWith("Stage Out SbS"))
			{
				stage.sbs = txt.mid(txt.lastIndexOf(":") + 2).toInt(&convOk);
				if(!convOk)
					throw SkinCamException("Could not read parameter: Stage Out SbS!");
			}
			else if (txt.startsWith("Stage Out Scaled"))
			{
				stage.scaled = txt.mid(txt.lastIndexOf(":") + 2).toInt(&convOk);
				if(!convOk)
					throw SkinCamException("Could not read parameter: Stage Out Scaled!");
			}
			else if (txt.startsWith("Contrast auto"))
			{
				contrastAutomatic = txt.mid(txt.lastIndexOf(":") + 2).toInt(&convOk);
				if(!convOk)
					throw SkinCamException("Could not read parameter: Contrast Automatic!");
			}
			else if (txt.startsWith("Contrast Spread"))
			{
				contrastValue = txt.mid(txt.lastIndexOf(":") + 2).toInt(&convOk);
				if(!convOk)
					throw SkinCamException("Could not read parameter: Contrast Value!");
			}
			else if (txt.startsWith("Contrast Offset"))
			{
				contrastOffset = txt.mid(txt.lastIndexOf(":") + 2).toInt(&convOk);
				if(!convOk)
					throw SkinCamException("Could not read parameter: Contrast Offset!");
			}
			else if (txt.startsWith("Skin by Quot"))
			{
				detectSkinByQuotient = txt.mid(txt.lastIndexOf(":") + 2).toInt(&convOk);
				if(!convOk)
					throw SkinCamException("Could not read parameter: Skin by Quotient!");
			}
			else if (txt.startsWith("Quotient"))
			{
				values = txt.mid(txt.lastIndexOf(":") + 2).split(";");
				for (i = 0; i < values.count(); i++)
				{
					if(!values.at(i).isEmpty())
					{
						QStringList filter = values.at(i).split("/");

						quotientFilter newFilter;
						newFilter.channel1 = filter.at(0).toInt(&convOk);
						newFilter.channel2 = filter.at(1).toInt(&convOk);
						newFilter.lowerThreshold = filter.at(2).toDouble(&convOk);
						newFilter.upperThreshold = filter.at(3).toDouble(&convOk);
						newFilter.minValue1 = filter.at(4).toInt(&convOk);
						newFilter.minValue2 = filter.at(5).toInt(&convOk);

						quotientFilters.append(newFilter);

						if(!convOk)
							throw SkinCamException("Could not read parameter: Quotient filters!");
					}
				}
			}
			else if (txt.startsWith("Show Mask"))
			{
				showMaskContours = txt.mid(txt.lastIndexOf(":") + 2).toInt(&convOk);
				if(!convOk)
					throw SkinCamException("Could not read parameter: Show Contours!");
			}
			else if (txt.startsWith("Morph"))
			{
				morphResult = txt.mid(txt.lastIndexOf(":") + 2).toInt(&convOk);
				if(!convOk)
					throw SkinCamException("Could not read parameter: Morph Contours!");
			}
			else if (txt.startsWith("Suppress"))
			{
				suppressMotion = txt.mid(txt.lastIndexOf(":") + 2).toInt(&convOk);
				if(!convOk)
					throw SkinCamException("Could not read parameter: Suppress Motion!");
			}
			else if (txt.startsWith("Motion"))
			{
				motionThreshold = txt.mid(txt.lastIndexOf(":") + 2).toInt(&convOk);
				if(!convOk)
					throw SkinCamException("Could not read parameter: Motion Threshold!");
			}
			else if (txt.startsWith("False Color"))
			{
				values = txt.mid(txt.lastIndexOf(":") + 2).split(";");
				for (i = 0; i < values.count(); i++)
				{
					if(!values.at(i).isEmpty())
					{
						falseColorChannels.append( values.at(i).toInt(&convOk) );
						if(!convOk)
							throw SkinCamException("Could not read parameter: False Color Channels!");
					}
				}
			}
			else if (txt.startsWith("Side by"))
			{
				values = txt.mid(txt.lastIndexOf(":") + 2).split(";");
				for (i = 0; i < values.count(); i++)
				{
					if(!values.at(i).isEmpty())
					{
						sideBySideChannels.append( values.at(i).toInt(&convOk) );
						if(!convOk)
							throw SkinCamException("Could not read parameter: Side by Side Channels!");
					}
				}
			}
		}
	}
	else
		throw SkinCamException("Error reading config file!");

	file.close();
}

void SkinCamConfig::setDefaults()
{
	contrastAutomatic	= false;
	contrastValue		= 230;
	contrastOffset		= 0;

	detectSkinByQuotient= false;
	detectSkinBySVM = false;

	edgeDetection			= false;
	edgeThreshold			= 20;

	estimateDistance = false;
	sharpMetric = metric_Sobel;
	sharpnessNbrhdSize = 7;
	distanceLowEnd = 1500;
	distanceHighEnd = 3005;

	showMaskContours	= true;
	morphResult			= true;
	suppressMotion		= false;
	motionThreshold		= 1500;

	lastDir.setPath( QDir::homePath() );
	videoTarget.setPath( lastDir.absolutePath().append("/out") );

#ifdef __linux
	stage.serialInt = "/dev/ttyUSB1";
#else
	stage.serialInt = "COM2";
#endif
	stage.start = 0;
	stage.end = 1507;
	stage.inverted = true;
	stage.stepwidth = 10;
	stage.numImg = 1;
	stage.delay = 500;
	stage.mi = true;
	stage.fc = false;
	stage.sbs = false;
	stage.scaled = false;
	stage.targetFolder = videoTarget.absolutePath();
}

void SkinCamConfig::writeToFile(QString filename)
{
	QString line;
	int i;
	QList<qint16> bands;

	//create path and target file
	QFile file(filename);

	//if file exists, it will be overwritten

	//Create config file at given path
	if (file.open(QFile::WriteOnly | QFile::Text))
	{
		QTextStream out(&file);
		out << "# Camera System Setup #" << endl;

		out << "Auto detect         : " << cameraConfig.autoCameraSearch << endl;

		out << "IP Address          : " << cameraConfig.IPadress << endl;

		out << "Ringlight Interface : " << cameraConfig.ringlightInterface << endl;

		out << "Wavebands           : ";
		line.clear();
		QMapIterator<qint16, bool> it(cameraConfig.wavebands.config);
		while(it.hasNext())
		{
			it.next();
			line.append(QString("%1=%2; ").arg(it.key()).arg(it.value()));
		}
		line.chop(2);
		out << line << endl;

		out << "Frequency           : " << cameraConfig.frequency << endl;

		out << "Integration Time    : " << cameraConfig.integrationTime << endl;

		out << "Buffer Size         : " << cameraConfig.bufferSize << endl;

		out << endl;
		out << "# Calibration Setup #" << endl;

		out << "FPN Reference File  : " << calibration.fpnRefFile << endl;

		out << "FPN Enabled         : " << calibration.applyFPNCal << endl;

		out << "Subtract Dark       : " << calibration.subtractDark << endl;

		out << "White Calibration   : ";
		line.clear();
		bands = calibration.whiteCal.keys();
		for (i = 0; i < calibration.whiteCal.size(); i++)
		{
			line.append(QString("%1=%2; ").arg(bands.at(i))
										  .arg(calibration.whiteCal[bands.at(i)],
											   0, 'f', 3));
		}
		line.chop(2);
		out << line << endl;

		out << "White Cal Enabled   : " << calibration.applyWhiteCal << endl;

		out << endl;
		out << "# GUI Settings #" << endl;

		out << "Last Directory      : " << lastDir.absolutePath() << endl;

		out << "Video Output Folder : " << videoTarget.absolutePath() << endl;

		out << endl;
		out << "# Stage Settings #" << endl;

		out << "Stage Interface     : " << stage.serialInt << endl;

		out << "Stage Target Folder : " << stage.targetFolder << endl;

		out << "Stage Start Pos     : " << stage.start << endl;

		out << "Stage End Pos       : " << stage.end << endl;

		out << "Stage Stepwidth     : " << stage.stepwidth << endl;

		out << "Stage Num Images    : " << stage.numImg << endl;

		out << "Stage Delay         : " << stage.delay << endl;

		out << "Stage Inverted      : " << stage.inverted << endl;

		out << "Stage Out MI        : " << stage.mi << endl;

		out << "Stage Out FC        : " << stage.fc << endl;

		out << "Stage Out SbS       : " << stage.sbs << endl;

		out << "Stage Out Scaled    : " << stage.scaled << endl;

		out << endl;
		out << "# Image Processing #" << endl;

		out << "Contrast automatic  : " << contrastAutomatic << endl;

		out << "Contrast Spread Rate: " << contrastValue << endl;

		out << "Contrast Offset     : " << contrastOffset << endl;

		out << "Skin by Quotients   : " << detectSkinByQuotient << endl;

		out << "Quotientfilters     : ";
		line.clear();
		for(i = 0; i < quotientFilters.size(); i++)
		{
			line.append(QString("%1/%2/%3/%4/%5/%6; ")
						.arg(quotientFilters.at(i).channel1)
						.arg(quotientFilters.at(i).channel2)
						.arg(quotientFilters.at(i).lowerThreshold)
						.arg(quotientFilters.at(i).upperThreshold)
						.arg(quotientFilters.at(i).minValue1)
						.arg(quotientFilters.at(i).minValue2));
		}
		line.chop(2);
		out << line << endl;

		out << "Show Mask Contours  : " << showMaskContours << endl;

		out << "Morph Contours      : " << morphResult << endl;

		out << "Suppress Motion     : " << suppressMotion << endl;

		out << "Motion Threshold    : " << motionThreshold << endl;

		out << endl;
		out << "# Image Display #" << endl;

		out << "False Color Channels: ";
		line.clear();
		for(i = 0; i < falseColorChannels.size(); i++)
		{
			line.append(QString("%1; ").arg(falseColorChannels.at(i)));
		}
		line.chop(2);
		out << line << endl;

		out << "Side by Side Ch.    : ";
		line.clear();
		for(i = 0; i < sideBySideChannels.size(); i++)
		{
			line.append(QString("%1; ").arg(sideBySideChannels.at(i)));
		}
		line.chop(2);
		out << line << endl;
	}
	else
	{
		throw new SkinCamException("Config file could not be created!");
	}

	file.close();
}
