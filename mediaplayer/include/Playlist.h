#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QObject>
#include <QVector>
#include "Track.h"

class Playlist:public QObject
{
    Q_OBJECT
public:
    // ---- playmode ----
    enum class PlayMode {
        LoopMode,
        SingleMode,
        RandomMode
    };
    Q_ENUM(PlayMode)

    // ---- constructor ----
    explicit Playlist(QObject *parent = nullptr);

    // ---- operations ----
    void addTrack(const Track &track); // add to the end of the list
    void insertTrack(int index, const Track &track);
    void removeTrack(int index);
    void moveTrack(int from, int to);
    void clear(); // clear the list
    

    // ---- getters ----
    QVector<Track> tracks() const; // return the copy of the track list
    Track currentTrack() const;
    int currentIndex() const;
    int count() const; //return the total number of tracks

    // ---- navigation ----
    Track next(); //return next track based on PlayMode and update the index
    Track prev();

    // ---- PlayMode ----
    PlayMode playMode() const;
    void setPlayMode(PlayMode mode);
    QString playModeString() const; //used in the UI

    // ---- search and filter ----
    QVector<Track> search(const QString &keyword) const;

signals:
    // when the current track is changed, emit this signal
    void currentTrackChanged(const Track &track);
    // when track list is changed, emit this signal
    void tracksChanged();

private:
    QVector<Track> m_tracks;
    QVector<int> m_history;
    int m_currentIndex = -1; // -1 means the list is emoty or no track is selected
    PlayMode m_playMode = PlayMode::LoopMode;
    int getRandomIndex() const; // used for RandomMode

};

#endif // PLAYLIST_H