#include "quickwebcamcapture.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QuickWebCamCapture w;
	w.show();

	return a.exec();
}
