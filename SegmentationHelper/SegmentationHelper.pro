#-------------------------------------------------
#
# Project created by QtCreator 2014-10-29T10:18:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SegmentationHelper
TEMPLATE = app

LIBS += -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_calib3d \

SOURCES += main.cpp\
		mainwindow.cpp \
    seeds/seeds2.cpp \
    slic/slic.cpp \
    segmentfelsenzwalb/segmentation.cpp

HEADERS  += mainwindow.h \
    seeds/seeds2.h \
    seeds/seedsHelper.h \
    slic/slic.h \
    segmentfelsenzwalb/convolve.h \
    segmentfelsenzwalb/disjoint-set.h \
    segmentfelsenzwalb/filter.h \
    segmentfelsenzwalb/image.h \
    segmentfelsenzwalb/imconv.h \
    segmentfelsenzwalb/imutil.h \
    segmentfelsenzwalb/misc.h \
    segmentfelsenzwalb/pnmfile.h \
    segmentfelsenzwalb/segment-graph.h \
    segmentfelsenzwalb/segment-image.h \
    segmentfelsenzwalb/segmentation.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    segmentfelsenzwalb/Makefile
