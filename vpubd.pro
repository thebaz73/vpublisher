#-------------------------------------------------
#
# Project created by QtCreator 2014-03-22T11:55:04
#
#-------------------------------------------------

QT       += core dbus network

QT       -= gui

TARGET = vpubd
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

QMAKE_CXXFLAGS = -std=gnu++0x

########################################################################
## Da decommentare se si vuole compilare utilizzando come "Message Bus"
##  di DBUS il "Session Bus" invece del "System Bus"
DEFINES +=DBUS_USE_SESSION_BUS="1"
########################################################################

########################################################################
## Da decommentare se si vuole compilare nel sistema embedded
## DEFINES +=YOCTO_BUILD="1"
########################################################################

########################################################################
## Da decommentare se si vuole compilare nel sistema embedded
DEFINES +=USING_DEVICE="1"
########################################################################

########################################################################
## Da decommentare se si vuole compilare nel sistema embedded
##DEFINES +=USING_PIPE="1"
########################################################################

SOURCES += src/main.cpp \
    src/hls/hlsmanager.cpp \
    src/hls/segmenter.cpp \
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
    src/vpublishingservice_interface.cpp \
    src/configurationmanager.cpp \
    src/segmentationmanager.cpp \
    src/dash/dashmanager.cpp \
    src/configurablemanager.cpp \
    src/hls/m3u8generator.cpp \
    src/streamingworker.cpp \
    src/deploymentmanager.cpp \
    src/playlistmanager.cpp \
    src/deploy/deployer.cpp \
    src/deploy/deployerfactory.cpp \
    src/deploy/localdeployer.cpp

HEADERS += \
    src/hls/hlsmanager.h \
    src/hls/segmenter.h \
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
    src/vpublishingservice.h \
    src/configurationmanager.h \
    src/segmentationmanager.h \
    src/dash/dashmanager.h \
    src/configurablemanager.h \
    src/hls/m3u8generator.h \
    src/streamingworker.h \
    src/deploymentmanager.h \
    src/playlistmanager.h \
    src/deploy/deployer.h \
    src/deploy/deployerfactory.h \
    src/deploy/localdeployer.h



unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += libudev
unix: PKGCONFIG += x264

WORKSTATION = HOME
#WORKSTATION = OFFICE
equals(WORKSTATION, "HOME") {
    message("Using home environment");
    unix: PKGCONFIG += libavformat
    unix: PKGCONFIG += libavcodec
    unix: PKGCONFIG += libavutil
}
equals(WORKSTATION, "OFFICE") {
    message("Using office environment");
    unix:!macx: LIBS += -lbz2
    unix:!macx: LIBS += -lmp3lame
    unix:!macx: LIBS += -lfaad
    unix:!macx: LIBS += -lpthread
    unix:!macx: LIBS += -lavformat
    unix:!macx: LIBS += -lavcodec
    unix:!macx: LIBS += -lavutil
}

unix:!macx: LIBS += -lm
unix:!macx: LIBS += -lz

OTHER_FILES += \
    resources/dbus.sh \
    resources/vpubservice.xml

