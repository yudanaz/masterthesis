#include "mainwindow.h"
#include "ui_mainwindow.h"

#define IMGTYPES "*.jpg *.png *.ppm"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    io(this)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::on_pushButton_released()
{
    QStringList rgbs = io.getFileNames("Open RGB calib images", IMGTYPES);
    QStringList irs = io.getFileNames("Open IR calib images", IMGTYPES);
    QStringList nirs;
    preproc.calib(rgbs, nirs, irs, Size(9,6));
}
