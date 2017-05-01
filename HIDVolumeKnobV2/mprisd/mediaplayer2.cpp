#include "mediaplayer2.h"
#include "mediaplayer2interface.h"
#include "mediaplayer2propertiesinterface.h"

//_________________________________________________________________
/**
 * @brief MediaPlayer2::MediaPlayer2
 * @param service
 * @param parent
 */
MediaPlayer2::MediaPlayer2(QString service, QObject *parent)
    : QObject(parent)
    , m_playerInterface(new MediaPlayer2Interface(service, "/org/mpris/MediaPlayer2", QDBusConnection::sessionBus(), this))
    , m_propertiesInterface(new MediaPlayer2PropertiesInterface("org.freedesktop.DBus.Properties", "/org/mpris/MediaPlayer2", QDBusConnection::sessionBus(), this))
{
    timerEvent(NULL);
    startTimer(1000);
    setMetaData(m_playerInterface->metadata());
    connect(m_propertiesInterface, &MediaPlayer2PropertiesInterface::PropertiesChanged, this, &MediaPlayer2::onPropertiesChanged);
}

//_________________________________________________________________
/**
 * @brief Returns the meta data
 * @return the meta data
 */
MetaData MediaPlayer2::metaData() const
{
    return m_metaData;
}

//_________________________________________________________________
/**
 * @brief Sets the meta data
 *
 * The metaDataChanged() signal will be emitted if the
 * meta data changed
 * @param metaData
 * @sa metaData()
 */
void MediaPlayer2::setMetaData(const QVariantMap &metaData)
{
    MetaData convertedMetaData = MetaData::fromMap(metaData);
    if (m_metaData == convertedMetaData)
        return;

    m_metaData = convertedMetaData;
    emit metaDataChanged(m_metaData);
}

//_________________________________________________________________
/**
 * @brief MediaPlayer2::onPropertiesChanged
 * @param interfaceName
 * @param changedProperties
 * @param invalidatedProperties
 */
void MediaPlayer2::onPropertiesChanged(const QString &interfaceName, const QString &changedProperties, const QStringList &invalidatedProperties)
{
    qDebug() << Q_FUNC_INFO << interfaceName << changedProperties << invalidatedProperties;
}

//_________________________________________________________________
/**
 * @brief Timer event handler
 */
void MediaPlayer2::timerEvent(QTimerEvent */*event*/)
{
    QVariantMap metaData = m_playerInterface->metadata();
    setMetaData(metaData);
}
