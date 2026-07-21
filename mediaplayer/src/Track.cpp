#include "Track.h"
#include <QFileInfo>

// ---- constructors ----
Track::Track(){}

Track::Track(const QString &filePath):m_filePath(filePath)
{
    QFileInfo info(filePath);
    m_title = info.baseName();
}

// ---- getters ----
QString Track::filePath() const{return m_filePath;}
QString Track::title() const{return m_title;}
QString Track::artist() const{return m_artist;}
QString Track::album() const{return m_album;}
int Track::durationMs() const{return m_durationMs;}
int Track::lyricsOffsetMs() const { return m_lyricsOffsetMs;}

// ---- setters ----
void Track::setTitle(const QString &title){m_title = title;}
void Track::setArtist(const QString &artist) { m_artist = artist; }
void Track::setDurationMs(int ms) { m_durationMs = ms; }
void Track::setLyricsOffsetMs(int offsetMs) { m_lyricsOffsetMs = offsetMs; }

// ---- auxiliary tool ----
QString Track::durationString() const
{
    int total_second = m_durationMs / 1000;
    int minute = total_second / 60;
    int second = total_second % 60;
    return QString("%1:%2").arg(minute, 2, 10, QChar('0')).arg(second, 2, 10, QChar('0'));
}