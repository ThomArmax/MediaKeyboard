QT += core dbus serialport
QT -= gui

CONFIG += c++11

include(common.pri)

TARGET = mprisd
CONFIG += console
CONFIG -= app_bundle

DEFINES += QT_BUILD

TEMPLATE = app

SOURCES += main.cpp \
    serialconnection.cpp \
    mediaplayer2interface.cpp \
    mediaplayer2propertiesinterface.cpp \
    metadata.cpp

HEADERS += \
    serialconnection.h \
    mediaplayer2interface.h \
    mediaplayer2propertiesinterface.h \
    metadata.h

OTHER_FILES += \
    org.mpris.MediaPlayer2.xml \
    common.pri \
