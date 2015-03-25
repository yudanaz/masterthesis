#include "mainwindow_imgviewer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow_imgViewer w;
	w.show();

	return a.exec();
}
