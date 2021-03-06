#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QtWidgets/QFileDialog>
#include <QDir>
#include <QList>
#include <QStringList>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QXmlStreamReader>
#include <QProgressDialog>
#include <QTime>
#include <QMessageBox>
#include <QInputDialog>

#include <opencv2/opencv.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/nonfree/features2d.hpp>
//#include "opencv2/calib3d/calib3d.hpp"
//#include "opencv2/core/core.hpp"
//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/features2d/features2d.hpp"

#include "cameracalibration.h"

using namespace cv;

class Polygon
{
public:
	Polygon(){}
	Polygon(int i, int c, QList<Point> pts){ index = i; color = c; points = pts; }
	Polygon clone(){ return Polygon(index, color, points); }
	int index;
	int color;
	QList<Point> points;

	static bool polygonLessThan(const Polygon &p1, const Polygon &p2)
	{
		return p1.index < p2.index;
	}
};



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

	/*Just a test to see if the git commit and pull/push really works*/

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void on_btn_makeLabelImgs_released();

	void on_btn_seed_released();

	void on_pushButton_seedAgain_released();


	void on_btn_slic_released();

	void on_pushButton_slicAgain_released();

	void on_btn_felsenzwalb_released();

	void on_pushButton_felsenzwalbAgain_released();

	void on_btn_surf_released();

	void on_pushButton_surfAgain_released();

	void on_btn_calibSingle_released();

	void on_btn_undistSingle_released();

	void on_btn_calibGoldeye_released();

	void on_btn_undistGoldeye_released();

	void on_btn_undistLOAD_released();

	void on_btn_calibStereo_released();

	void on_btn_calibStereoGRBNIR_released();

	void on_btn_undistStereo_released();

	void on_btn_undistStereoRGBNIR_released();

	void on_btn_stereoVision_released();

	void on_pushButton_stereoAgain_released();

	void on_btn_alignImgs_released();

	void on_slider_prefilterSize_sliderMoved(int position);

	void on_slider_prefilterCAP_sliderMoved(int position);

	void on_slider_SADwindow_sliderMoved(int position);

	void on_slider_minDisp_sliderMoved(int position);

	void on_slider_dispRange_sliderMoved(int position);

	void on_slider_textureThresh_sliderMoved(int position);

	void on_slider_speckleWindow_sliderMoved(int position);

	void on_slider_speckleRange_sliderMoved(int position);

	void on_slider_uniqueness_sliderMoved(int position);

	void on_btn_saveParams_released();

	void on_btn_loadParams_released();

	void on_btn_homogenMake_released();

	void on_btn_homogenLoad_released();

	void on_btn_homogenApply_released();

	void on_checkBox_useSGBM_clicked();

	void on_pushButton_test_released();


	void on_btn_makeImgPatches_released();

	void on_btn_makeTrainVal_released();

	void on_btn_makeHomographyMatrix_released();

	void on_btn_registerImages_released();

	void on_btn_thinOut_released();

	void on_pushButton_released();

	void on_pushButton_accuracy_released();

	void on_pushButton_superpxAvg_released();

    void on_btn_colorLabels_released();

    void on_btn_showFilterKernels_released();

    void on_btn_avgLogs_released();

private:
	void makeLabelImages(QStringList fileNames);
	void makeSeedsSuperpixels(QString fileName);
	void makeSlicSuperpixels(QString fileName);
	void makeFelsenzwalbSuperpixels(QString fileName);
	void makeDisparityImage(Mat leftImgCol, Mat rightImgCol);
	void makeSurfFeatures(QString fileName);
	void undistortStereo(bool isRGB_NIR_Stereo);
    Mat improveLabePredictionlWithSuperpixels(Mat &nir, Mat &prediction, int whichAlg, int labelAmount);



	Ui::MainWindow *ui;
	vector< vector<int> > colorMap;
	vector< QStringList > objectMap;
	QString lastDir;
	QString lastSeedsFilename;
	QString lastSlicFilename;
	QString lastFelsenzwalbFilename;
	QString lastStereoFileNameL;
	QString lastStereoFileNameR;
	Mat lastStereoFileR;
	Mat lastStereoFileL;
	QString lastFeatureFileName;

	CameraCalibration camCalib;
};

#endif // MAINWINDOW_H
