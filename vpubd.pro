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
    src/dev/dtvdevice.cpp \
    src/dvbapi/dvbaudio.cpp \
    src/dvbapi/dvbca.cpp \
    src/dvbapi/dvbdemux.cpp \
    src/dvbapi/dvbfe.cpp \
    src/dvbapi/dvbnet.cpp \
    src/dvbapi/dvbvideo.cpp \
    src/app/qtlocalpeer.cpp \
    src/app/qtlockedfile_unix.cpp \
    src/app/qtlockedfile_win.cpp \
    src/app/qtlockedfile.cpp \
    src/vpubdaemon.cpp \
    src/vpublishingservice.cpp \
    src/vpublishingservice_interface.cpp

HEADERS += \
    src/hsl/hlsmanager.h \
    src/hsl/segmenter.h \
    src/dev/devicemanager.h \
    src/dev/devicesettings.h \
    src/lifecyclemanager.h \
    src/utils/sleeper.h \
    src/dev/dtvdevice.h \
    src/utils/common.h \
    src/dvbapi/dvbaudio.h \
    src/dvbapi/dvbca.h \
    src/dvbapi/dvbdemux.h \
    src/dvbapi/dvbfe.h \
    src/dvbapi/dvbnet.h \
    src/dvbapi/dvbvideo.h \
    src/app/qtlocalpeer.h \
    src/app/qtlockedfile.h \
    src/vpubdaemon.h \
    src/vpublishingservice_interface.h \
    src/vpublishingservice.h

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += libudev

OTHER_FILES += \
    resources/dbus.sh \
    resources/vpubservice.xml
