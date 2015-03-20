#-------------------------------------------------
#
# Project created by QtCreator 2015-03-19T12:05:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LabelScene
TEMPLATE = app

#OPENCV
INCLUDEPATH += /usr/local/include/opencv
LIBS += -L/usr/local/lib
LIBS += -lopencv_core
LIBS += -lopencv_imgproc
LIBS += -lopencv_highgui
#LIBS += -lopencv_ml
#LIBS += -lopencv_video
LIBS += -lopencv_features2d
LIBS += -lopencv_calib3d
LIBS += -lopencv_objdetect
#LIBS += -lopencv_contrib
#LIBS += -lopencv_legacy
#LIBS += -lopencv_flann
LIBS += -lopencv_nonfree

SOURCES += main.cpp\
		mainwindow.cpp \
    seeds/seeds2.cpp

HEADERS  += mainwindow.h \
    seeds/seeds2.h \
    seeds/seedsHelper.h

FORMS    += mainwindow.ui
