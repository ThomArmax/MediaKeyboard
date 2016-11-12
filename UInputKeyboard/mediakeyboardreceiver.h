#ifndef MEDIAKEYBOARDRECEIVER_H
#define MEDIAKEYBOARDRECEIVER_H

#include <QObject>

class QSerialPort;
class UInputWrapper;

/**
 * @brief The MediaKeyBoardReceiver is in charge of receiving
 * data from the serial port and generates the keyboards events
 * functions of received data
 */
class MediaKeyBoardReceiver : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief MediaKeyBoardReceiver's constructor
     * @param parent parent QObject
     */
    explicit MediaKeyBoardReceiver(QObject *parent = 0);
    /**
     * @brief MediaKeyBoardReceiver's destructor
     *
     * Frees allocated resources and close the serial port
     */
    ~MediaKeyBoardReceiver();

    /**
     * @brief Listens the serial port
     * @param port Serial port to be listened
     * @param baudRate Serial port's bauderate
     * @return true if succeeded; false otherwise
     */
    bool listen(const QString &port, const qint32 baudRate);

signals:
    /**
     * @brief Emitted when a key is received
     * @param keyCode Key's code received
     */
    void keyClicked(int keyCode);

private slots:
    void onDataReceived();

private:
    QSerialPort *m_port;
    UInputWrapper *m_uinputDev;
};

#endif // MEDIAKEYBOARDRECEIVER_H
