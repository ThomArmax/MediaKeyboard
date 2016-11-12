#include "mediakeyboardreceiver.h"
#include "uinputwrapper.h"

#include <QSerialPort>
#include <QDebug>
#include <QCoreApplication>

#include <linux/uinput.h>

MediaKeyBoardReceiver::MediaKeyBoardReceiver(QObject *parent)
    : QObject(parent)
    , m_port(new QSerialPort(this))
    , m_uinputDev(new UInputWrapper(this))
{
    connect(m_port, SIGNAL(readyRead()), this, SLOT(onDataReceived()));

    m_uinputDev->openDevice();

    m_uinputDev->registerEvent(EV_KEY);
    m_uinputDev->registerEvent(EV_SYN);

    m_uinputDev->registerKey(KEY_VOLUMEUP);
    m_uinputDev->registerKey(KEY_VOLUMEDOWN);
    m_uinputDev->registerKey(KEY_PLAYPAUSE);

    m_uinputDev->createDevice("Armax Media Keyboard", UInputWrapper::BusVirtual, 1, 1, 1);
}

MediaKeyBoardReceiver::~MediaKeyBoardReceiver()
{
    m_port->close();
}

bool MediaKeyBoardReceiver::listen(const QString &port, const qint32 baudRate)
{
    m_port->setPortName(port);
    m_port->setBaudRate(baudRate);
    return m_port->open(QIODevice::ReadOnly);
}

void MediaKeyBoardReceiver::onDataReceived()
{
    QByteArray data = m_port->readAll();
    int keyCode = data.toHex().toInt(NULL, 16);

    m_uinputDev->sendKeyPressedAndReleasedEvent(keyCode);
    emit keyClicked(keyCode);
}
