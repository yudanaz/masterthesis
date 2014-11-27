#include "rgb_nir_depth_capture.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	RGB_NIR_Depth_Capture w;
	w.show();

	return a.exec();
}
