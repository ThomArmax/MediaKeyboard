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
    sysv.files = $${_PRO_FILE_PWD_}/uinputkeyboard.sh
    sysv.path = /etc/init.d/
    updaterc.target = update-rc
    updaterc.path = /etc/init.d/
    updaterc.commands = update-rc.d uinputkeyboard.sh defaults
    INSTALLS += target sysv updaterc
    QMAKE_EXTRA_TARGETS = updaterc
}

DISTFILES += \
    uinputkeyboard.sh
