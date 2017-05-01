#ifndef METADATA_H
#define METADATA_H

#include <QVariantMap>
#include "oleddata.h"

class MetaData
{
public:
    MetaData();
    static MetaData fromMap(const QVariantMap &map);
    QList<OLEDData> toOledData() const;

    bool operator==(const MetaData& other);

    QString artUrl;             // "mpris:artUrl" QString
    quint64 length;             // "mpris:length" qulonglong
    QString trackId;            // "mpris:trackid" QString
    QString album;              // "xesam:album" QString
    QStringList albumArtist;    // "xesam:albumArtist
    QString artist;             // "xesam:artist
    double autoRating;          // "xesam:autoRating
    int discNumber;             // "xesam:discNumber
    QString title;              // "xesam:title
    int trackNumber;            // "xesam:trackNumber
    QString url;                // "xesam:url" QString
};

#endif // METADATA_H
