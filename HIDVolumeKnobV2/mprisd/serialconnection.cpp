#include "serialconnection.h"

#include <QSerialPort>
#include <QTime>
#include <QDebug>

SerialConnection::SerialConnection(QObject *parent)
    : QObject(parent)
    , m_pPort(new QSerialPort(this))
{
    m_pPort->setBaudRate(9600);
    m_pPort->setPortName("/dev/ttyACM0");

    bool opened = m_pPort->open(QSerialPort::ReadWrite);
    Q_ASSERT(opened);

    startTimer(1000);
}

void SerialConnection::timerEvent(QTimerEvent */*event*/)
{
    QString data(QTime::currentTime().toString("hh:mm:ss zzz"));
    qDebug() << data;
    m_pPort->write(data.toUtf8());
    m_pPort->flush();
}
