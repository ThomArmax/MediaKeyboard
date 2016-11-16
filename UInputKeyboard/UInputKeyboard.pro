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
    sysvchmod.target = sysvchmod
    sysvchmod.commands = $$system("chmod +x $${OUT_PWD}/uinputkeyboard.sh")

    sysvconfig.target = sysvconfig
    sysvconfig.commands = $$system("sed 's%@PREFIX@%$${PREFIX}%g' $${_PRO_FILE_PWD_}/uinputkeyboard.sh.in > $${OUT_PWD}/uinputkeyboard.sh")
    sysvconfig.depends = sysvchmod

    sysv.files = $${OUT_PWD}/uinputkeyboard.sh
    sysv.path = /etc/init.d/
    sysv.depends = sysvconfig

    updaterc.target = update-rc
    updaterc.path = /etc/init.d/
    updaterc.commands = update-rc.d uinputkeyboard.sh defaults

    INSTALLS += target sysv updaterc
    QMAKE_EXTRA_TARGETS = updaterc sysvchmod sysvconfig
}

DISTFILES += \
    uinputkeyboard.sh.in \
    README.md \
