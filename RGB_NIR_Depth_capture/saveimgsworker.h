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
	void saveImgs(RGBDNIR_MAP imgs);

private:
	QString getUniquePrefixFromDateAndTime();

};

#endif // SAVEIMGSWORKER_H
