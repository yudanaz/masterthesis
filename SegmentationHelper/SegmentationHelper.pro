#-------------------------------------------------
#
# Project created by QtCreator 2014-10-29T10:18:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SegmentationHelper
TEMPLATE = app

INCLUDEPATH += /usr/local/include/opencv
LIBS += -L/usr/local/lib
LIBS += -lopencv_core
LIBS += -lopencv_imgproc
LIBS += -lopencv_highgui
#LIBS += -lopencv_ml
#LIBS += -lopencv_video
LIBS += -lopencv_features2d
LIBS += -lopencv_calib3d
#LIBS += -lopencv_objdetect
#LIBS += -lopencv_contrib
#LIBS += -lopencv_legacy
#LIBS += -lopencv_flann
#LIBS += -lopencv_nonfree

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

OTHER_FILES +=
