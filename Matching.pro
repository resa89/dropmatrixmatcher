#-------------------------------------------------
#
# Project created by QtCreator 2013-12-29T10:48:12
#
#-------------------------------------------------

QT       += core gui

QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Matching
TEMPLATE = app

INCLUDEPATH = /usr/local/include

SOURCES += main.cpp \
    prolabel.cpp \
    matching.cpp \
    greydrawing.cpp \
    dropmatrixmatcher.cpp

HEADERS  += \
    prolabel.h \
    dropmatrixmatcher.h

LIBS += -L/usr/local/lib \
    -lopencv_core \
    -lopencv_highgui \
    -lopencv_video \
    -lopencv_imgproc

FORMS    += \
    dropmatrixmatcher.ui

ICON = DMM_logo.icns

