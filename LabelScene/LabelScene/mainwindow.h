#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QKeyEvent>
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


	void on_horizontalSlider_sliderMoved(int position);

private:
	QString getOpenCVTypeName(int type);

	void keyPressEvent(QKeyEvent *e);
	void interpretKey(int key);

	Vec3d BGR2HSV(Vec3b bgr);
	Vec3b HSV2BGR(Vec3d hsv);

	Ui::MainWindow *ui;
	Mat imgOrig;
	Mat imgSuperpx1;
	Mat imgSuperpx2;
	Mat imgSuperpx3;
	Mat imgSuperpx4;
	Mat imgDraw;
	Mat imgLabels;
	int currentLabel;
	vector<int> labelsOnSuperpx1;
	vector<int> labelsOnSuperpx2;
	vector<int> labelsOnSuperpx3;
	vector<int> labelsOnSuperpx4;

	static int mouseX;
	static int mouseY;
	static bool drawing;
	static int width;
	static int height;
};

#endif // MAINWINDOW_H
