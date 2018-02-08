QT += core
QT -= gui

CONFIG += c++11

TARGET = opencv
CONFIG += console
CONFIG -= app_bundle

INCLUDEPATH += /usr/local/include/opencv
LIBS += -lopencv_core \
 -lopencv_videoio \
 -lopencv_imgcodecs \
 -lopencv_imgproc \
 -lopencv_calib3d \
 -lopencv_video \
 -lopencv_features2d \
 -lopencv_ml \
 -lopencv_highgui \
 -lopencv_objdetect

TEMPLATE = app

SOURCES += main.cpp \
    signclassifier.cpp \
    signtrainer.cpp \
    roifinder.cpp \
    roadsignsdetection.cpp

HEADERS += \
    signclassifier.h \
    signtrainer.h \
    roifinder.h \
    roadsignsdetection.h
