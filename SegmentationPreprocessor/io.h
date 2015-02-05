#ifndef IO_H
#define IO_H

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
using namespace cv;

class IO
{
public:
    IO(QWidget* parent);
    QString getFileName(QString msg2user, QString extension);
    QString getSaveFile(QString msg2user, QString extension);
    QStringList getFileNames(QString msg2user, QString extension);

    static void print1ChMatrixToConsole(Mat mat);
    static QString getOpenCVTypeName(int type);

private:
    void saveLastDir();
    QString lastDir;
    QWidget* parent;
};

#endif // IO_H
