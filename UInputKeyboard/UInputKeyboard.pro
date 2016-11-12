QT += core serialport
QT -= gui

CONFIG += c++11

TARGET = UInputKeyboard
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    mediakeyboardreceiver.cpp \
    uinputwrapper.cpp

HEADERS += \
    mediakeyboardreceiver.h \
    uinputwrapper.h

isEmpty(PREFIX) {
    PREFIX = /usr/local
}

target.path = $$PREFIX/bin

unix {
    INSTALLS += target
}
