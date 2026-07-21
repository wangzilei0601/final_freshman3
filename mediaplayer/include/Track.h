#ifndef TRACK_H
#define TRACK_H

#include <QString>
#include <QMetaType>

// a collection that stores all the information of a track
class Track
{
public:
    // ---- constructer ----
    Track();  // default constructor
    Track(const QString &filePath);  // constructor with file path

    // ---- getter ----//
    QString filePath() const; // get the file path of the track
    QString title() const; // get the title of the track
    QString artist() const; // get the artist of the track
    QString album() const; // get the album of the track
    int durationMs() const; // get the duration of the track in milliseconds
    int lyricsOffsetMs() const; // get the lyrics offset of the track in milliseconds
    
    // ---- setter ---- //
    void setTitle(const QString &title);
    void setArtist(const QString &artist);
    void setAlbum(const QString &album);
    void setDurationMs(int durationMs);
    void setLyricsOffsetMs(int lyricsOffsetMs);

    // ----auxiliary tool ---- //
    QString durationString() const; // return the duration of the track(like 3:20)

private:
    // ---- private data ---- //
    QString m_filePath = "No File Path";
    QString m_title = "Unknown Title";
    QString m_artist = "Unknown Artist";
    QString m_album = "Unknown Album";
    int m_durationMs = 0;
    int m_lyricsOffsetMs = 0;
};

Q_DECLARE_METATYPE(Track)

#endif // TRACK_H