#include "saveimgsworker.h"
#include "vimbacammanager.h"
#include <QDir>

SaveImgsWorker::SaveImgsWorker(QObject *parent) :
	QObject(parent)
{
}


void SaveImgsWorker::saveImgs(RGBDNIR_MAP imgs, bool RGB_img, bool NIR_DarkImg, bool NIR_channels, bool kinect_depth, bool kinect_ir, bool kinect_rgb)
{
	QMapIterator<RGBDNIR_captureType, Mat> i(imgs);

	QDir dir;
	if(!dir.exists(QDir::currentPath()+"/out")){ dir.mkdir(QDir::currentPath()+"/out"); }
	QString dateTime_str = getUniquePrefixFromDateAndTime();

	Mat nir970, nir1300, nir1550;

	while(i.hasNext())
	{
		i.next();

		//check if this image is supposed to be save to disk
		bool save = false;
		RGBDNIR_captureType type = i.key();
		switch (type)
		{
			case RGB: if(RGB_img) save = true; break;
			case NIR_Dark: if(NIR_DarkImg) save = true; break;
			case Kinect_Depth: if(kinect_depth) save = true; break;
			case Kinect_IR: if(kinect_ir) save = true; break;
			case Kinect_RGB: if(kinect_rgb) save = true; break;

			//only save nir channels later, when all three channels have been joined
			case NIR_970: if(NIR_channels){ nir970 = i.value(); } break;
			case NIR_1300: if(NIR_channels){ nir1300 = i.value(); } break;
			case NIR_1550: if(NIR_channels){ nir1550 = i.value(); } break;
			default: break;
		}

		if(save)
		{
			QString nm = QDir::currentPath() + "/out/" +
						 dateTime_str + "_" +
						 VimbaCamManager::getRGBDNIR_captureTypeString( (RGBDNIR_captureType)i.key() ) + ".png";
			imwrite(nm.toStdString().c_str(), i.value());
		}
	}

	//save multichannel if desired
	if(NIR_channels && nir970.cols != 0 && nir1300.cols != 0 && nir1550.cols != 0)
	{
		Mat nirMultiCh;
		std::vector<Mat> nirChs;
		nirChs.push_back(nir970);
		nirChs.push_back(nir1300);
		nirChs.push_back(nir1550);
		merge(nirChs, nirMultiCh);
		QString nm = QDir::currentPath() + "/out/" + dateTime_str + "_NIR_MultiCh.png";
		imwrite(nm.toStdString().c_str(), nirMultiCh);
	}
}

QString SaveImgsWorker::getUniquePrefixFromDateAndTime()
{
	QDateTime now = QDateTime::currentDateTime();
	return QString::number(now.date().year())
		   + QString("%1").arg(now.date().month(), 2, 10, QChar('0'))
		   + QString("%1").arg(now.date().day(), 2, 10, QChar('0')) + "_"
		   + QString("%1").arg(now.time().hour(), 2, 10, QChar('0'))
		   + QString("%1").arg(now.time().minute(), 2, 10, QChar('0'))
		   + QString("%1").arg(now.time().second(), 2, 10, QChar('0'))
		   + QString("%1").arg(now.time().msec()/10, 2, 10, QChar('0'));
}
