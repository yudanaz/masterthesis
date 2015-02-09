#ifndef SAVEIMGSWORKER_H
#define SAVEIMGSWORKER_H

#include <QObject>
#include "cammanager.h"

class SaveImgsWorker : public QObject
{
	Q_OBJECT
public:
	explicit SaveImgsWorker(QObject *parent = 0);

signals:

public slots:
	void saveImgs(RGBDNIR_MAP imgs, bool RGB_img, bool NIR_DarkImg, bool NIR_channels, bool kinect_depth, bool kinect_ir, bool kinect_rgb);

private:
	QString getUniquePrefixFromDateAndTime();

};

#endif // SAVEIMGSWORKER_H
