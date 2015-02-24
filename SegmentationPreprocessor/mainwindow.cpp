#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "hog_crossspectralstereomatcher.h"

#define IMGTYPES "*.jpg *.png *.ppm"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    ui_stereoMparams(new StereoMatching_params),
    io(this),
    preproc(parent)
{
    ui->setupUi(this);
//    preproc.OutputImageSize(200, 160); //for test purposes
    preproc.OutputImageSize(320, 256); // ~ 636x508 (native goldeye res.) * 0,5
    preproc.set_CSstereoType(crossSpectrSt_HOG);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete ui_stereoMparams;

}

void MainWindow::preprocessImages()
{
    Mat rgb_, depth_, depthStereo_, nir_;

    //set HOG parameters through static function (not very elegant, i know, but wtf...)
    preproc.setParameters(ui_stereoMparams->getParams());

    preproc.preproc(rgb, nir, depth, rgb_, nir_, depthStereo_, depth_);


    //for now...
//    equalizeHist(nir_, nir_);
    imshow("RGB", rgb_);
    imshow("NIR", nir_);
    imshow("cross-spectral Stereo", depthStereo_);
    imshow("Kinect Depth", depth_);
//    imwrite("rectified_rgb.png", rgb_);
//    imwrite("rectified_nir.png", nir_);
}




/*************************************************************************************************
 * BUTTON SLOTS
 *************************************************************************************************/

void MainWindow::on_pushButton_calibCams_released()
{
    QStringList rgbs = io.getFileNames("Open images for RGB camera calibration", IMGTYPES);
    QStringList irs = io.getFileNames("Open images for IR camera calibration", IMGTYPES);
    QStringList nirs = io.getFileNames("Open images for NIR camera calibration", IMGTYPES);

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

    QStringList rgbs = io.getFileNames("Open RGB images for camera rig calibration", IMGTYPES);
    QStringList irs = io.getFileNames("Open IR calib images", IMGTYPES);
    QStringList nirs  = io.getFileNames("Open NIR images for camera rig calibration", IMGTYPES);

    if(rgbs.length() == 0 || nirs.length() == 0 || irs.length() == 0 )
    {
        QMessageBox::information(this, "Error", "Calibration images missing for one of the cameras", QMessageBox::Ok);
        return;
    }

    if(rgbs.length() != nirs.length()  || rgbs.length() != irs.length() || irs.length() != nirs.length())
    {
        QMessageBox::information(this, "Error", "Not same amount of calibration images for each camera", QMessageBox::Ok);
        return;
    }

    preproc.calibRig(rgbs, nirs, irs, Size(9,6));

}

void MainWindow::on_pushButton_preproc_released()
{
    if(!preproc.rig_is_calibrated())
    {
        QMessageBox::information(this, "Error", "Camera rig not calibrated yet", QMessageBox::Ok);
        return;
    }

    QString img_rgb = io.getFileName("Select RGB image", IMGTYPES);
    QString img_depth = io.getFileName("Select Depth image", "*.png");
    QString img_nir = io.getFileName("Select NIR image", IMGTYPES);

    if(img_rgb == "" || img_depth == "" || img_nir == "" ){ return; }

    rgb = imread(img_rgb.toStdString(), IMREAD_ANYCOLOR);
    depth = imread(img_depth.toStdString(), IMREAD_ANYDEPTH);
    nir = imread(img_nir.toStdString(), IMREAD_GRAYSCALE);

    preprocessImages();
}

void MainWindow::on_pushButton_reproc_released()
{
    if(rgb.cols == 0) return;
    if(depth.cols == 0) return;
    if(nir.cols == 0) return;
    preprocessImages();
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
