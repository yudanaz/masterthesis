#ifndef MAINWINDOW_IMGVIEWER_H
#define MAINWINDOW_IMGVIEWER_H

#include <QMainWindow>

namespace Ui {
class MainWindow_imgViewer;
}

class MainWindow_imgViewer : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow_imgViewer(QWidget *parent = 0);
	~MainWindow_imgViewer();

private:
	Ui::MainWindow_imgViewer *ui;
};

#endif // MAINWINDOW_IMGVIEWER_H
