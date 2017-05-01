#ifndef MEDIAPLAYER2_H
#define MEDIAPLAYER2_H

#include <QObject>
#include <QVariantMap>

#include "metadata.h"

class MediaPlayer2Interface;
class MediaPlayer2PropertiesInterface;

/**
 * @brief Represents a org.mpris.MediaPlayer2
 */
class MediaPlayer2 : public QObject
{
    Q_OBJECT
public:
    explicit MediaPlayer2(const QString iface, QObject *parent = 0);
    MetaData metaData() const;

signals:
    void metaDataChanged(const MetaData &metaData);

private slots:
    void onPropertiesChanged(const QString &interfaceName, const QString &changedProperties, const QStringList &invalidatedProperties);

private:
    void setMetaData(const QVariantMap &metaData);
    void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;

private:
    MediaPlayer2Interface* m_playerInterface;
    MediaPlayer2PropertiesInterface* m_propertiesInterface;
    MetaData m_metaData;
};

#endif // MEDIAPLAYER2_H
