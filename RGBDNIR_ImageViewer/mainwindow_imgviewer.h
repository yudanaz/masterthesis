#ifndef MAINWINDOW_IMGVIEWER_H
#define MAINWINDOW_IMGVIEWER_H

#include <QMainWindow>

#include <QFileDialog>
#include <QDir>
#include <QDebug>
#include <QDirIterator>
#include <QStringList>
#include <QList>
#include <QStringList>
#include <QSharedPointer>
#include <QGraphicsScene>
#include <QImage>

namespace Ui {
class MainWindow_imgViewer;
}

class MainWindow_imgViewer : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow_imgViewer(QWidget *parent = 0);
	~MainWindow_imgViewer();

private slots:
	void on_pushButton_open_released();

	void on_pushButton_previous_released();

	void on_pushButton_next_released();

	void on_pushButton_markAsBad_released();

private:
	void showImage();
	QString makeFileURLInNewDir(QString url, QString dirFromHere);
	Ui::MainWindow_imgViewer *ui;
	QList<QString> imageURLs;
	int imgIndex;
	int imgMax;

	QString nm_nir;
	QString nm_rgb;
	QString nm_depth;
	QString nm_dark;

	QSharedPointer<QGraphicsScene> ptr_NIRScene;
	QSharedPointer<QGraphicsScene> ptr_RGBScene;
	QSharedPointer<QGraphicsScene> ptr_DepthScene;

};

#endif // MAINWINDOW_IMGVIEWER_H
