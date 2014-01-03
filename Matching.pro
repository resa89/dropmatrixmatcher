#-------------------------------------------------
#
# Project created by QtCreator 2013-12-29T10:48:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Matching
TEMPLATE = app

INCLUDEPATH = /usr/local/include

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

LIBS += -lm -lopencv_core -lopencv_highgui -lopencv_video -lopencv_imgproc

FORMS    += mainwindow.ui
