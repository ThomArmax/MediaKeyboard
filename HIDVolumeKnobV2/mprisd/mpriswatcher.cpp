#include "mpriswatcher.h"
#include "mediaplayer2.h"
#include "serialconnection.h"

#include <QtDBus>
#include <QDebug>

#define ORG_MPRIS_MEDIAPLAYER2 "org.mpris.MediaPlayer2"

//_________________________________________________________________
/**
 * @brief Constructs a new MPRISWatcher
 * @param parent parent object
 */
MPRISWatcher::MPRISWatcher(QObject *parent)
    : QObject(parent)
    , m_serialConnection(new SerialConnection(this))
{
}

//_________________________________________________________________
/**
 * @brief Starts the watcher which will 'listen' for new services
 * on session bus
 * @return true if succeeded; false otherwise.
 * @sa stop()
 */
bool MPRISWatcher::start()
{
    if (!QDBusConnection::sessionBus().isConnected()) {
        fprintf(stderr, "Cannot connect to the D-Bus session bus.\n"
                        "To start it, run:\n"
                        "\teval `dbus-launch --auto-syntax`\n");
        return false;
    }
    connect(QDBusConnection::sessionBus().interface(), &QDBusConnectionInterface::serviceRegistered, this, &MPRISWatcher::onServiceRegistered);
    connect(QDBusConnection::sessionBus().interface(), &QDBusConnectionInterface::serviceUnregistered, this, &MPRISWatcher::onServiceUnregistered);

    QDBusReply<QStringList> reply = QDBusConnection::sessionBus().interface()->registeredServiceNames();

    if (!reply.isValid()) {
        qDebug() << "Error:" << reply.error().message();
        return false;
    }

    foreach (QString name, reply.value()) {
        if (name.contains(ORG_MPRIS_MEDIAPLAYER2))
            onServiceRegistered(name);
    }

    return true;
}

//_________________________________________________________________
/**
 * @brief Stops the watcher
 * @return true if succeeded; false otherwise
 */
bool MPRISWatcher::stop()
{
    QDBusConnection::sessionBus().interface()->disconnect(this);
    return true;
}

//_________________________________________________________________
/**
 * @brief Called when a new service is registred
 * @param service registred service's name
 */
void MPRISWatcher::onServiceRegistered(const QString &service)
{
    if (!service.contains(ORG_MPRIS_MEDIAPLAYER2))
        return;

    qDebug() << Q_FUNC_INFO << service;
    MediaPlayer2 *mp = new MediaPlayer2(service, this);
    connect(mp, &MediaPlayer2::metaDataChanged, this, &MPRISWatcher::onMetaDataChanged);
    onMetaDataChanged(mp->metaData());
    m_mediaPlayers << mp;
}

//_________________________________________________________________
/**
 * @brief Called when a service is unregistred
 * @param service unregistred service's name
 */
void MPRISWatcher::onServiceUnregistered(const QString &service)
{
    qDebug() << Q_FUNC_INFO << service;
}

//_________________________________________________________________
/**
 * @brief Called when the metadata from a MediaPlayer2 changed
 * @param metaData new meta data
 */
void MPRISWatcher::onMetaDataChanged(const MetaData &metaData)
{
    QList<OLEDData> oledDatas = metaData.toOledData();
    for (int i = 0; i < oledDatas.size(); i++)
        m_serialConnection->send(oledDatas.at(i));
}
