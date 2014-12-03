#ifndef INOUT_H
#define INOUT_H

#include<QList>
#include<QString>
#include<QStringList>
#include<QList>
#include<QDir>
#include <QProcess>

#include "opencv2/opencv.hpp"

using namespace cv;

class InOut
{
public:
	InOut();
	static QList<Mat> getImagesFromTarFile(QString source);

private:
	static double spreadFactor;		/*!<Spread Factor used to convert images do 8-bit.*/
};

#endif // INOUT_H
