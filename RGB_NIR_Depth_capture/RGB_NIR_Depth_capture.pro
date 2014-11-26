#-------------------------------------------------
#
# Project created by QtCreator 2014-11-26T11:39:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RGB_NIR_Depth_capture
TEMPLATE = app

### openCV
LIBS += -lopencv_core -lopencv_imgproc -lopencv_highgui

### AVT Vimba
INCLUDEPATH += /opt/Vimba_1_3/
LIBS += -L/opt/Vimba_1_3/VimbaCPP/DynamicLib/x86_64bit -lVimbaC -lVimbaCPP
SOURCES += GoldeyeCamera.cpp
HEADERS += GoldeyeCamera.h
DEFINES += AVT_VIMBA

SOURCES += main.cpp\
		rgbnird_mainwindow.cpp

HEADERS  += rgbnird_mainwindow.h

FORMS    += rgbnird_mainwindow.ui
