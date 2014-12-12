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
INCLUDEPATH += /opt/Vimba_1_3/AVTImageTransform/Include/
LIBS += -L/opt/Vimba_1_3/AVTImageTransform/DynamicLib/x86_64bit -lAVTImageTransform

SOURCES += \
	vimbacammanager.cpp \
	rgb_nir_depth_capture.cpp \
	skincamlegacy/core/multispectralimage.cpp \
	skincamlegacy/core/skincamconfig.cpp \
	skincamlegacy/io/camerainterface.cpp \
	skincamlegacy/io/camerasystemconfig.cpp \
	skincamlegacy/io/goldeyeVimba.cpp \
	skincamlegacy/io/imagesource.cpp \
	frameobserver.cpp \
	prosilicavimba.cpp
HEADERS += \
	vimbacammanager.h \
	rgb_nir_depth_capture.h \
	skincamlegacy/core/multispectralimage.h \
	skincamlegacy/core/skincamconfig.h \
	skincamlegacy/core/skincamexception.h \
	skincamlegacy/io/camerainterface.h \
	skincamlegacy/io/camerasystemconfig.h \
	skincamlegacy/io/goldeyeVimba.h \
	skincamlegacy/io/imagesource.h \
	frameobserver.h \
	prosilicavimba.h

DEFINES += AVT_VIMBA

SOURCES += main.cpp

HEADERS  +=

FORMS    += rgbnird_mainwindow.ui
