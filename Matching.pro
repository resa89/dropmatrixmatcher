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

SOURCES += main.cpp \
    prolabel.cpp \
    matching.cpp \
    greydrawing.cpp \
    dropmatrixmatcher.cpp \
    helpwindow.cpp

HEADERS  += \
    prolabel.h \
    dropmatrixmatcher.h \
    helpwindow.h

FORMS    += \
    dropmatrixmatcher.ui \
    helpwindow.ui

#-------------------------------------------------
# MacOS - libs
#-------------------------------------------------
mac: INCLUDEPATH = /opt/local/include #/usr/local/include (Version 3.0)

mac: LIBS += -L/opt/local/lib \#/usr/local/lib (Version 3.0)
    -lopencv_core \
    -lopencv_highgui \
    -lopencv_video \
    -lopencv_imgproc

mac: LIBS += -framework CoreFoundation

#-------------------------------------------------
# Windows32 - libs
#-------------------------------------------------
win32: INCLUDEPATH += C:/Users/thkocher/opencv/build/include
win32: DEPENDPATH += C:/Users/thkocher/opencv/build/include

win32:CONFIG(release, debug|release): LIBS += -LC:/Users/thkocher/opencv/build/x86/vc11/lib/ -lopencv_core249 -lopencv_highgui249 -lopencv_video249 -lopencv_imgproc249
else:win32:CONFIG(debug, debug|release): LIBS += -LC:/Users/thkocher/opencv/build/x86/vc11/lib/ -lopencv_core249d -lopencv_highgui249d -lopencv_video249d -lopencv_imgproc249d

#-------------------------------------------------
# Windows64 - libs
#-------------------------------------------------
win64: INCLUDEPATH += C:/openCV/opencv/build/include
win64: DEPENDPATH += C:/openCV/opencv/build/include

win64:CONFIG(release, debug|release): LIBS += -LC:/openCV/opencv/build/x64/vc12/lib/ -lopencv_core2410 -lopencv_highgui2410 -lopencv_video2410 -lopencv_imgproc2410
else:win64:CONFIG(debug, debug|release): LIBS += -LC:/openCV/opencv/build/x64/vc12/lib/ -lopencv_core2410d -lopencv_highgui2410d -lopencv_video2410d -lopencv_imgproc2410d

#-------------------------------------------------
# Icon for Mac
#-------------------------------------------------
mac: ICON = DMM_logo2.icns

#-------------------------------------------------
# Icon for Windows
#-------------------------------------------------
win32: RC_FILE = dmm.rc

win32: OTHER_FILES += \
    dmm.rc
