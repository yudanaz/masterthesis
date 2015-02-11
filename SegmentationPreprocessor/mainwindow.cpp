#include "mainwindow.h"
#include "ui_mainwindow.h"

#define IMGTYPES "*.jpg *.png *.ppm"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    io(this),
    preproc(parent)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::on_pushButton_calibCams_released()
{
    QStringList rgbs = io.getFileNames("Open images for RGB camera calibration", IMGTYPES);
    QStringList irs;// = io.getFileNames("Open images for IR camera calibration", IMGTYPES);
    QStringList nirs = io.getFileNames("Open images for NIR camera calibration", IMGTYPES);

    if(rgbs.length() == 0 || nirs.length() == 0 /*|| irs.length() == 0*/ )
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
    QStringList irs;// = io.getFileNames("Open IR calib images", IMGTYPES);
    QStringList nirs  = io.getFileNames("Open NIR images for camera rig calibration", IMGTYPES);

    if(rgbs.length() == 0 || nirs.length() == 0 /*|| irs.length() == 0*/ )
    {
        QMessageBox::information(this, "Error", "Calibration images missing for one of the cameras", QMessageBox::Ok);
        return;
    }

    if(rgbs.length() != nirs.length()  /*|| rgbs.length() != irs.length() || irs.length() != nirs.length()*/)
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

    QString img_rgb = io.getFileName("Select RGB image", "*.png");
//    QString img_depth = io.getFileName("Select Depth image", "*.png");
    QString img_nir = io.getFileName("Select NIR image", "*.png");

    if(img_rgb == "" /*|| img_depth == "" */|| img_nir == "" ){ return; }

    Mat rgb = imread(img_rgb.toStdString(), IMREAD_ANYCOLOR);
    Mat depth;// = imread(img_depth.toStdString(), IMREAD_ANYDEPTH);
    Mat nir = imread(img_nir.toStdString(), IMREAD_UNCHANGED);

    Mat rgb_, depth_, depthStereo_, nir_;
    preproc.preproc(rgb, nir, depth, rgb_, nir_, depthStereo_, depth_);

    imshow("RGB", rgb_);
    imshow("NIR", nir_);

}

void MainWindow::on_pushButton_save_released()
{
    QString sv = io.getSaveFile("Select file for saving camera parameters", "*.camparam");
    preproc.saveAll(sv);
}

void MainWindow::on_pushButton_load_released()
{
    QString sv = io.getFileName("Select camera parameters file", "*.camparam");
    preproc.loadAll(sv);
}


