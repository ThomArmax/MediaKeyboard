#ifndef METADATA_H
#define METADATA_H

#include <QVariantMap>
#include "oleddata.h"

/**
 * @brief Stores the meta data from a org.mpris.MediaPlayer2
 */
class MetaData
{
public:
    MetaData();
    static MetaData fromMap(const QVariantMap &map);
    QList<OLEDData> toOledData() const;

    bool operator==(const MetaData& other);

    QString artUrl;             //!< @brief mpris:artUrl
    quint64 length;             //!< @brief mpris:length
    QString trackId;            //!< @brief mpris:trackid
    QString album;              //!< @brief xesam:album
    QStringList albumArtist;    //!< @brief xesam:albumArtist
    QString artist;             //!< @brief xesam:artist
    double autoRating;          //!< @brief xesam:autoRating
    int discNumber;             //!< @brief xesam:discNumber
    QString title;              //!< @brief xesam:title
    int trackNumber;            //!< @brief xesam:trackNumber
    QString url;                //!< @brief xesam:url
};

#endif // METADATA_H
