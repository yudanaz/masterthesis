#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "io.h"
#include "imagepreprocessor.h"

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
    void on_pushButton_calibCams_released();

    void on_pushButton_preproc_released();

    void on_pushButton_save_released();

    void on_pushButton_load_released();

    void on_pushButton_calibCamRig_released();

private:
    Ui::MainWindow *ui;
    IO io;
    ImagePreprocessor preproc;
};

#endif // MAINWINDOW_H
