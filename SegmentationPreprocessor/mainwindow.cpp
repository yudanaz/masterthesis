#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "hog_crossspectralstereomatcher.h"
#include <QDir>
#include <QDesktopServices>

#define IMGTYPES "*.jpg *.png *.ppm"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	nir_url(""), rgb_url(""), depth_url(""),
	ui_stereoMparams(new StereoMatching_params),
	io(this),
	preproc(parent)
{
	ui->setupUi(this);
//    preproc.OutputImageSize(200, 160); //for test purposes
	//preproc.OutputImageSize(320, 256); // ~ 636x508 (native goldeye res.) * 0,5
	preproc.set_CSstereoType(crossSpectrSt_HOG);
}

MainWindow::~MainWindow()
{
	delete ui;
	delete ui_stereoMparams;

}

void MainWindow::preprocessImages(Mat &rgb_out, Mat &depth_out, Mat &depthStereo_out, Mat &nir_out)
{
//	qDebug() << IO::getOpenCVTypeName(depth.type());
//	depth.convertTo(depth, CV_8U, 255/2047);
//	imshow("kinect depth", depth);
//	cvWaitKey();

	destroyAllWindows();

	//set HOG parameters through static function (not very elegant, i know, but wtf...)
	preproc.setParameters(ui_stereoMparams->getParams());

	//set some options
	bool normDepth = ui->actionNormalize_Depth->isChecked();
	bool makeSkinBinary = ui->actionMake_Skine_Binary_Image->isChecked();
	bool makeCSStereo = ui->actionMake_Cross_Spectral_Stereo->isChecked();
	bool rgbRegDist = ui->actionDistort->isChecked();
	bool rgbregTPSpline = ui->actionThin_plate_spline->isChecked();
	preproc.setOptions(normDepth, makeSkinBinary, makeCSStereo, rgbRegDist, rgbregTPSpline);

	preproc.preproc(rgb, nir, depth, rgb_out, nir_out, depthStereo_out, depth_out);
}

void MainWindow::preprocessAndShow()
{
	Mat rgb_, depth_, depthStereo_, nir_;
	preprocessImages(rgb_, depth_, depthStereo_, nir_);

	//show the images
	bool makeCSStereo = ui->actionMake_Cross_Spectral_Stereo->isChecked();

	imshow("RGB", rgb_);
	imshow("NIR", nir_);
	if(makeCSStereo){ imshow("cross-spectral Stereo", depthStereo_); }
	imshow("Kinect Depth", depth_);

	imwrite("RGB.png", rgb_);
	imwrite("NIR.png", nir_);
	if(makeCSStereo){ imwrite("Depth_CSStereo.png", depthStereo_); }
	imwrite("Depth_Kinect.png", depth_);
}

bool MainWindow::loadImagesGroup(QString img_NIR)
{
	//get the rgb and depth image belonging to this nir image
	nir_url = img_NIR;
	rgb_url = img_NIR;
	rgb_url = rgb_url.remove("NIR_MultiCh.png").append("RGB.png");
	depth_url = img_NIR;
	depth_url = depth_url.remove("NIR_MultiCh.png").append("Kinect_Depth.png");

	//load images
	if(rgb_url == "" || depth_url == "" || img_NIR == "" ){ return false; }
	rgb = imread(rgb_url.toStdString(), IMREAD_COLOR);
	depth = imread(depth_url.toStdString(), IMREAD_ANYDEPTH);
	nir = imread(img_NIR.toStdString(), IMREAD_COLOR);

	return true;
}


void MainWindow::savePreprocImages(QString outDir, Mat &rgb, Mat &depth, Mat &depthStereo, Mat &nir)
{
	QString nir_url_out = outDir + "/" + nir_url.split("/").last().remove(".png").append("_p.png");
	QString rgb_url_out = outDir + "/" + rgb_url.split("/").last().remove(".png").append("_p.png");
	QString depth_url_out = outDir + "/" + depth_url.split("/").last().remove(".png").append("_p.png");
	QString depthCSStereo_url_out = outDir + "/" + depth_url.split("/").last().remove("Kinect_Depth.png").append("CSStereo.png");

	imwrite(nir_url_out.toStdString(), nir);
	imwrite(rgb_url_out.toStdString(), rgb);
	imwrite(depth_url_out.toStdString(), depth);
	if(ui->actionMake_Cross_Spectral_Stereo->isChecked())
	{
		imwrite(depthCSStereo_url_out.toStdString(), depthStereo);
	}
}

/*************************************************************************************************
 * BUTTON SLOTS
 *************************************************************************************************/

void MainWindow::on_pushButton_calibCams_released()
{
	QStringList rgbs = io.getFileNames("Open images for RGB camera calibration", IMGTYPES);
	QStringList nirs = io.getFileNames("Open images for NIR camera calibration", IMGTYPES);
	QStringList irs = io.getFileNames("Open images for IR camera calibration", IMGTYPES);

	if(rgbs.length() == 0 || nirs.length() == 0 || irs.length() == 0 )
	{
		QMessageBox::information(this, "Error", "Calibration images missing for one of the cameras", QMessageBox::Ok);
		return;
	}

	preproc.calibCams(rgbs, nirs, irs, Size(9,6));
}

void MainWindow::on_pushButton_calibCamRig_released()
{
	if(!preproc.cams_are_calibrated())
	{
		QMessageBox::information(this, "Error", "Cameras not calibrated individually yet", QMessageBox::Ok);
		return;
	}

	QStringList rgbs = io.getFileNames("Open RGB images for RGB-to-NIR camera rig calibration", IMGTYPES);
	QStringList nirs_rgb  = io.getFileNames("Open NIR images for RGB-to-NIR camera rig calibration", IMGTYPES);
	QStringList irs = io.getFileNames("Open IR images for IR-to-NIR camera rig calibration", IMGTYPES);
	QStringList nirs_ir = io.getFileNames("Open NIR images for IR-to-NIR camera rig calibration", IMGTYPES);

	if(rgbs.length() == 0 || nirs_rgb.length() == 0 || irs.length() == 0 || nirs_ir.length() == 0 )
	{
		QMessageBox::information(this, "Error", "Calibration images missing for one of the cameras", QMessageBox::Ok);
		return;
	}

	if(rgbs.length() != nirs_rgb.length()  || irs.length() != nirs_ir.length())
	{
		QMessageBox::information(this, "Error", "Not same amount of calibration images for camera pairs", QMessageBox::Ok);
		return;
	}

	preproc.calibRig(rgbs, nirs_rgb, irs, nirs_ir, Size(9,6));

}

void MainWindow::on_pushButton_preproc_released()
{
	if(!preproc.rig_is_calibrated())
	{
		QMessageBox::information(this, "Error", "Camera rig not calibrated yet", QMessageBox::Ok);
		return;
	}

//	QString img_rgb = io.getFileName("Select RGB image", IMGTYPES);
//	QString img_nir = io.getFileName("Select NIR image", IMGTYPES);
//	QString img_depth = io.getFileName("Select Depth image", IMGTYPES);

	QString img_nir = io.getFileName("Select NIR image", "*NIR_MultiCh.png");
	QString img_rgb = img_nir;
	img_rgb = img_rgb.remove("NIR_MultiCh.png").append("RGB.png");
	QString img_depth = img_nir;
	img_depth = img_depth.remove("NIR_MultiCh.png").append("Kinect_Depth.png");


	if(img_rgb == "" || img_depth == "" || img_nir == "" ){ return; }

	rgb = imread(img_rgb.toStdString(), IMREAD_COLOR);
	depth = imread(img_depth.toStdString(), IMREAD_ANYDEPTH);
	nir = imread(img_nir.toStdString(), IMREAD_COLOR);
//	qDebug() << "NIR type" << IO::getOpenCVTypeName(nir.type());
//	qDebug() << "Depth type" << IO::getOpenCVTypeName(depth.type());

	preprocessAndShow();
}

void MainWindow::on_pushButton_batchProc_released()
{
	if(!preproc.rig_is_calibrated())
	{
		QMessageBox::information(this, "Error", "Camera rig not calibrated yet", QMessageBox::Ok);
		return;
	}

	QStringList imgs_nir = io.getFileNames("Select NIR image", "*NIR_MultiCh.png");

	//get directory and check if output directory exists, if not, create
	QString thisDir = QFileInfo(imgs_nir.first()).absoluteDir().absolutePath();
	QString outDir = thisDir+"/preprocessed";
	if( !QDir().exists(outDir) ){ QDir().mkdir(outDir); }

	//make progress bar dialog
	QProgressDialog progress("Batch-preprocessing images", "Cancel", 0, imgs_nir.size(), this);
	progress.setMinimumWidth(300); progress.setMinimumDuration(50); progress.setValue(1);
	int i = 1;

	//process all selected images
	foreach(QString img_nir, imgs_nir)
	{
		//load images into class member variables
		bool success = loadImagesGroup(img_nir);
		if(!success){ continue; }
		QCoreApplication::processEvents(); //make qt app responsive

		//preprocess and save images
		Mat rgb_, depth_, depthStereo_, nir_;
		preprocessImages(rgb_, depth_, depthStereo_, nir_);
		savePreprocImages(outDir, rgb_, depth_, depthStereo_, nir_);

		QCoreApplication::processEvents(); //make qt app responsive
		progress.setValue(i++);
	}
	progress.setValue(imgs_nir.size());
}

void MainWindow::on_pushButton_reproc_released()
{
	if(rgb.cols == 0) return;
	if(depth.cols == 0) return;
	if(nir.cols == 0) return;
	preprocessAndShow();
}

void MainWindow::on_pushButton_save_released()
{
	QString sv = io.getSaveFile("Select file for saving camera parameters", "*.camparam");
	if(sv == "") { return; }
	preproc.saveAll(sv);
}

void MainWindow::on_pushButton_load_released()
{
	QString sv = io.getFileName("Select camera parameters file", "*.camparam");
	if(sv == "") { return; }
	preproc.loadAll(sv);
}

/*************************************************************************************************
 * MENU SLOTS
 *************************************************************************************************/

void MainWindow::on_actionStereoM_Params_triggered()
{
	ui_stereoMparams->show();
}

void MainWindow::on_pushButton_openFomeFolder_released()
{
	QDir d;
	QDesktopServices::openUrl(d.absolutePath());
}


