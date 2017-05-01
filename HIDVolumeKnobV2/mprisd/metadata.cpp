#include "metadata.h"

static const QString ArtUrl         = "mpris:artUrl";
static const QString Length         = "mpris:length";
static const QString TrackId        = "mpris:trackid";
static const QString Album          = "xesam:album";
static const QString AlbumArtist    = "xesam:albumArtist";
static const QString Artist         = "xesam:artist";
static const QString AutoRating     = "xesam:autoRating";
static const QString DiscNumber     = "xesam:discNumber";
static const QString Title          = "xesam:title";
static const QString TrackNumber    = "xesam:trackNumber";
static const QString Url            = "xesam:url";

MetaData::MetaData()
    : artUrl(QString())
    , length(0)
    , trackId(QString())
    , album(QString())
    , albumArtist(QStringList())
    , artist(QString())
    , autoRating(0)
    , discNumber(0)
    , title(QString())
    , trackNumber(0)
    , url(QString())
{

}

MetaData MetaData::fromMap(const QVariantMap &map)
{
    MetaData metaData;
    metaData.artUrl         = map.value(ArtUrl, QString()).toString();
    metaData.length         = map.value(Length, 0).toInt();
    metaData.trackId        = map.value(TrackId, QString()).toString();
    metaData.album          = map.value(Album, QString()).toString();
    metaData.albumArtist    = map.value(AlbumArtist, QStringList()).toStringList();
    metaData.artist         = map.value(Artist, QString()).toString();
    metaData.autoRating     = map.value(AutoRating, 0).toDouble();
    metaData.discNumber     = map.value(DiscNumber, 0).toInt();
    metaData.title          = map.value(Title, QString()).toString();
    metaData.trackNumber    = map.value(TrackNumber, 0).toInt();
    metaData.url            = map.value(Url, QString()).toString();

    return metaData;
}

bool MetaData::operator ==(const MetaData& other)
{
    return (
                (artUrl == other.artUrl) &&
                (length == other.length) &&
                (trackId == other.trackId) &&
                (album == other.album) &&
                (albumArtist == other.albumArtist) &&
                (artist == other.artist) &&
                (autoRating == other.autoRating) &&
                (discNumber == other.discNumber) &&
                (title == other.title) &&
                (trackNumber == other.trackNumber) &&
                (url == other.url)
                );
}

QList<OLEDData> MetaData::toOledData() const
{
    QList<OLEDData> data;

    data << OLEDData(OLEDData::Artist, qPrintable(artist));
    data << OLEDData(OLEDData::Album, qPrintable(album));
    data << OLEDData(OLEDData::Track, qPrintable(title));

    return data;
}
