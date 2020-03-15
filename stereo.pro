#-------------------------------------------------
#
# Project created by QtCreator 2018-01-23T11:41:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = stereo
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    camera.cpp \
    detector.cpp \
    sgbmmatching.cpp \
    simpledemo.cpp \
    matcher.cpp \
    helper.cpp

HEADERS  += mainwindow.h \
    camera.h \
    detector.h \
    sgbmmatching.h \
    simpledemo.h \
    matcher.h \
    helper.h

FORMS    += mainwindow.ui

INCLUDEPATH += /usr/local/opencv320/include \
                /usr/local/opencv320/include/opencv \
                /usr/local/opencv320/include/opencv2

LIBS += /usr/local/opencv320/lib/libopencv_*



INCLUDEPATH += /home/cvpr/caffe/include  /home/cvpr/caffe/build/src
LIBS += -L/home/cvpr/caffe/build/lib
LIBS += -lcaffe

INCLUDEPATH += /usr/local/cuda/include
LIBS += -L/usr/local/cuda/lib64
LIBS += -lcudart -lcublas -lcurand

LIBS += -lglog -lgflags -lprotobuf -lboost_system -lboost_thread -llmdb -lleveldb -lstdc++ -lcudnn -lcblas -latlas
