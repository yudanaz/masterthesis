#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "opencv2/opencv.hpp"

using namespace cv;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void on_btn_load_released();
	void drawAndShow();
	static void mouseCallback(int event, int x, int y, int flags, void *param);
	void doMouseCallback(int event, int x, int y, int flags);


private:
	Ui::MainWindow *ui;
	Mat imgOrig;
	Mat imgSuperpx;
	Mat imgDraw;
};

#endif // MAINWINDOW_H
