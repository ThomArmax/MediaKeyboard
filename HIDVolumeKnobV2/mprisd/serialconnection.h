#ifndef SERIALCONNECTION_H
#define SERIALCONNECTION_H

#include <QObject>

class QSerialPort;

class SerialConnection : public QObject
{
    Q_OBJECT
public:
    explicit SerialConnection(QObject *parent = 0);

private:
    void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;

private:
    QSerialPort *m_pPort;
};

#endif // SERIALCONNECTION_H
