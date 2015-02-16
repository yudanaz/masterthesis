#-------------------------------------------------
#
# Project created by QtCreator 2015-01-29T14:06:30
#
#-------------------------------------------------

QT       += core gui
QT      += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SegmentationPreprocessor
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
    io.cpp \
    imagepreprocessor.cpp \
    crossbilateralfilterwrapper.cpp \
    helper.cpp \
    hog_crossspectralstereomatcher.cpp \
    locnorm_crossspectralstereomatcher.cpp \
    stereosgbm_hog.cpp

HEADERS  += mainwindow.h \
    io.h \
    imagepreprocessor.h \
    crossBilateralFilter/array.h \
    crossBilateralFilter/array_n.h \
    crossBilateralFilter/chrono.h \
    crossBilateralFilter/fast_lbf.h \
    crossBilateralFilter/geom.h \
    crossBilateralFilter/math_tools.h \
    crossBilateralFilter/mixed_vector.h \
    crossBilateralFilter/msg_stream.h \
    crossbilateralfilterwrapper.h \
    helper.h \
    hog_crossspectralstereomatcher.h \
    crossspectralstereomatcher.h \
    locnorm_crossspectralstereomatcher.h \
    stereosgbm_hog.h


FORMS    += mainwindow.ui

OTHER_FILES += \
    config/dummyDoc

#COPY CONFIG DIR FROM SOURCE TO BUILD DIR
copydata.commands = $(COPY_DIR) $$PWD/config $$OUT_PWD
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata
