QT += core dbus serialport
QT -= gui

CONFIG += c++11 link_pkgconfig
#PKGCONFIG = dbusextended-qt5

#LIBS += -ldbusextended-qt5
#LIBS += -lmpris-qt5

include(common.pri)

TARGET = mprisd
CONFIG += console
CONFIG -= app_bundle

DEFINES += QT_BUILD

TEMPLATE = app

SOURCES += main.cpp \
    serialconnection.cpp \
    mediaplayer2interface.cpp \

HEADERS += \
    serialconnection.h \
    mediaplayer2interface.h \

OTHER_FILES += \
    org.mpris.MediaPlayer2.xml \
    common.pri \
