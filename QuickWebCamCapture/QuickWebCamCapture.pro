#-------------------------------------------------
#
# Project created by QtCreator 2014-12-08T14:27:11
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QuickWebCamCapture
TEMPLATE = app

INCLUDEPATH += /usr/local/include/opencv
LIBS += -L/usr/local/lib
LIBS += -lopencv_core
LIBS += -lopencv_imgproc
LIBS += -lopencv_highgui
#LIBS += -lopencv_ml
#LIBS += -lopencv_video
#LIBS += -lopencv_features2d
#LIBS += -lopencv_calib3d
#LIBS += -lopencv_objdetect
#LIBS += -lopencv_contrib
#LIBS += -lopencv_legacy
#LIBS += -lopencv_flann
LIBS += -lopencv_nonfree

SOURCES += main.cpp\
		quickwebcamcapture.cpp

HEADERS  += quickwebcamcapture.h

FORMS    += quickwebcamcapture.ui
