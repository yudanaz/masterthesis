#include "io.h"
#include <QTextStream>
#include <QFile>

IO::IO(QWidget *parent)
    : parent(parent),
      lastDir(QDir::homePath())
{
    //try to get last directory
    QFile file("config/lastDir.txt");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        QString s = in.readLine();
        if(s != ""){ lastDir = s; }
        file.close();
    }
}

QString IO::getFileName(QString msg2user, QString extension)
{
    QString fileName = QFileDialog::getOpenFileName(parent, msg2user, lastDir, extension);
    if(fileName == ""){ return NULL; }
    lastDir = QFileInfo(fileName).path();
    saveLastDir();
    return fileName;
}

QString IO::getSaveFile(QString msg2user, QString extension)
{
    QString fileName = QFileDialog::getSaveFileName(parent, msg2user, lastDir, extension);
    if(fileName == ""){ return NULL; }
    lastDir = QFileInfo(fileName).path();
    saveLastDir();
    if(!fileName.contains(extension.remove("*"))){ fileName.append(extension); }
    return fileName;
}

QStringList IO::getFileNames(QString msg2user, QString extension)
{
    QStringList fileNames = QFileDialog::getOpenFileNames(parent, msg2user, lastDir, extension);
    if(fileNames.length() == 0) { return fileNames; }
    lastDir = QFileInfo(fileNames.first()).absolutePath();
    saveLastDir();
    return fileNames;
}


void IO::print1ChMatrixToConsole(Mat mat)
{
    QString s;
    int type = mat.type();
    for (int y = 0; y < mat.rows; ++y)
    {
        s = "";
        for (int x = 0; x < mat.cols; ++x)
        {
            switch(type)
            {
            case CV_8UC1:
                s += QString::number(mat.at<uchar>(y,x)) + "\t";
                break;
            case CV_16UC1:
                s += QString::number(mat.at<ushort>(y,x)) + "\t";
                break;
            case CV_32FC1:
                s += QString::number(mat.at<float>(y,x)) + "\t";
                break;
            case CV_64FC1:
                s += QString::number(mat.at<double>(y,x)) + "\t";
                break;
            default:
                qDebug() << "matrix format not supported";
                return;
            }
        }
        qDebug() << s;
    }
    qDebug() << "===";
}


QString IO::getOpenCVTypeName(int type)
{
    QString r;

    uchar depth = type & CV_MAT_DEPTH_MASK;
    uchar chans = 1 + (type >> CV_CN_SHIFT);

    switch ( depth ) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
    }

    r += "C";
    r += (chans+'0');

    return r;
}

void IO::saveLastDir()
{
    QFile file("config/lastDir.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out << lastDir << "\n";
        file.close();
    }
}

