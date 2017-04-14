QT += core dbus serialport
QT -= gui

CONFIG += c++11 link_pkgconfig
#PKGCONFIG = dbusextended-qt5

#LIBS += -ldbusextended-qt5
#LIBS += -lmpris-qt5

TARGET = mprisd
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    serialconnection.cpp

HEADERS += \
    serialconnection.h
