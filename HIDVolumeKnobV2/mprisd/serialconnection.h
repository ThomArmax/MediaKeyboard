#ifndef SERIALCONNECTION_H
#define SERIALCONNECTION_H

#include <QObject>
#include "oleddata.h"

class QSerialPort;

class SerialConnection : public QObject
{
    Q_OBJECT
public:
    explicit SerialConnection(QObject *parent = 0);

public slots:
    bool send(const QByteArray &data);
    bool send(const char* data, const int size);
    bool send(const OLEDData &oledData);

private slots:
    void onReadyRead();

private:
    void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;

private:
    QSerialPort *m_pPort;
};

#endif // SERIALCONNECTION_H
