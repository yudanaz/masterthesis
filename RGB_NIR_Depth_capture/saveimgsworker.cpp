#include "saveimgsworker.h"
#include "vimbacammanager.h"
#include <QDir>

SaveImgsWorker::SaveImgsWorker(QObject *parent) :
	QObject(parent)
{
}


void SaveImgsWorker::saveImgs(RGBDNIR_MAP imgs)
{
	QMapIterator<RGBDNIR_captureType, Mat> i(imgs);

	QDir dir;
	if(!dir.exists(QDir::currentPath()+"/out")){ dir.mkdir(QDir::currentPath()+"/out"); }
	QString dateTime_str = getUniquePrefixFromDateAndTime();

	while(i.hasNext())
	{
		i.next();
		QString nm = QDir::currentPath() + "/out/" +
					 dateTime_str + "_" +
					 VimbaCamManager::getRGBDNIR_captureTypeString( (RGBDNIR_captureType)i.key() ) + ".png";
		imwrite(nm.toStdString().c_str(), i.value());
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
