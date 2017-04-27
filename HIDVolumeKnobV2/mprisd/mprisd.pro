QT += core dbus serialport
QT -= gui

CONFIG += c++11 link_pkgconfig
#PKGCONFIG = dbusextended-qt5

#LIBS += -ldbusextended-qt5
#LIBS += -lmpris-qt5

TARGET = mprisd
CONFIG += console
CONFIG -= app_bundle

DEFINES += QT_BUILD

TEMPLATE = app

INCLUDEPATH += ../

SOURCES += main.cpp \
    serialconnection.cpp \
    ../oleddata.cpp

HEADERS += \
    serialconnection.h \
    ../oleddata.h