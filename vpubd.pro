#-------------------------------------------------
#
# Project created by QtCreator 2014-03-22T11:55:04
#
#-------------------------------------------------

QT       += core dbus multimedia

QT       -= gui

TARGET = vpubd
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += src/main.cpp \
    src/hsl/hlsmanager.cpp \
    src/hsl/segmenter.cpp \
    src/dev/devicemanager.cpp \
    src/dev/devicesettings.cpp \
    src/lifecyclemanager.cpp \
    src/utils/sleeper.cpp \
    src/dev/dtvdevice.cpp

HEADERS += \
    src/hsl/hlsmanager.h \
    src/hsl/segmenter.h \
    src/dev/devicemanager.h \
    src/dev/devicesettings.h \
    src/lifecyclemanager.h \
    src/utils/sleeper.h \
    src/dev/dtvdevice.h \
    src/utils/common.h

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += libudev
