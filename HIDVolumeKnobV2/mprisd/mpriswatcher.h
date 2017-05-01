#ifndef MPRISWATCHER_H
#define MPRISWATCHER_H

#include <QObject>
#include <QList>
#include "metadata.h"

class MediaPlayer2;
class SerialConnection;

/**
 * @brief Watch for new org.mpris.MediaPlayer2
 */
class MPRISWatcher : public QObject
{
    Q_OBJECT
public:
    explicit MPRISWatcher(QObject *parent = 0);

public slots:
    bool start();
    bool stop();

private slots:
    void onServiceRegistered(const QString &service);
    void onServiceUnregistered(const QString &service);
    void onMetaDataChanged(const MetaData &metaData);

private:
    SerialConnection* m_serialConnection;
    QList<MediaPlayer2*> m_mediaPlayers;
};

#endif // MPRISWATCHER_H
