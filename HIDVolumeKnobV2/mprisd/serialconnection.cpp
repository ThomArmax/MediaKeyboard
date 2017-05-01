#include "serialconnection.h"

#include <QSerialPort>
#include <QTime>
#include <QDebug>

//_________________________________________________________________
/**
 * @brief Constuct a new SerialConnection
 * @param parent parent QObject
 */
SerialConnection::SerialConnection(QObject *parent)
    : QObject(parent)
    , m_pPort(new QSerialPort(this))
{
    m_pPort->setBaudRate(9600);
    m_pPort->setPortName("/dev/ttyACM0");

    bool opened = m_pPort->open(QSerialPort::ReadWrite);
    Q_ASSERT(opened);

    connect(m_pPort, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
}

//_________________________________________________________________
/**
 * @brief Sends the given QByateArray
 * @param data data to be sent
 * @return true if succeeded; false otherwise
 */
bool SerialConnection::send(const QByteArray &data)
{
    int toSend = data.size();
    bool success = m_pPort->write(data) == toSend;
    m_pPort->flush();
    return success;
}

//_________________________________________________________________
/**
 * @brief Sends the given OLEDData
 * @param oledData OLEDData to be sent
 * @return true if succeeded; false otherwise
 */
bool SerialConnection::send(const OLEDData &oledData)
{
    char *encodedData = NULL;
    int dataSize = -1;

    if (oledData.encode(&encodedData, &dataSize))
    {
        QByteArray dataArray(encodedData);
        send(dataArray);
        free(encodedData);
        return true;
    }

    return false;
}

//_________________________________________________________________
/**
 * @brief Sends the given buffer
 * @param data data to be sent
 * @param size data' size
 * @return true if succeeded; false otherwise
 */
bool SerialConnection::send(const char *data, const int size)
{
    return m_pPort->write(data) == size;
}

//_________________________________________________________________
/**
 * @brief Called when data are received on the serial port
 */
void SerialConnection::onReadyRead()
{
    qDebug() << m_pPort->readAll();
}
