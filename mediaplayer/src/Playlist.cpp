#include "Playlist.h"
#include <QRandomGenerator>

// ---- constructor ----
Playlist::Playlist(QObject *parent):QObject(parent){}

// ---- operations ----
void Playlist::addTrack(const Track &track)
{
    bool wasEmpty = m_tracks.isEmpty();
    m_tracks.append(track);
    // if it's the first track, set it as current track
    if (wasEmpty) {
        m_currentIndex = 0;
        emit currentTrackChanged(m_tracks.first());
    }
    emit tracksChanged();
    m_history.clear(); //when the track list changes , the history should be cleared
}

void Playlist::insertTrack(int index, const Track &track)
{
    if (index < 0 || index > m_tracks.size()) return;
    bool wasEmpty = m_tracks.isEmpty();
    m_tracks.insert(index, track);
    // if it's the first track, set it as current track
    if (wasEmpty) {
        m_currentIndex = 0;
        emit currentTrackChanged(m_tracks.first());
    }
    // if index <= m_currentIndex, m_currentIndex should be increased by 1
    else if (index <= m_currentIndex && m_currentIndex != -1) {
        m_currentIndex++;
    }
    emit tracksChanged();
    m_history.clear(); //when the track list changes , the history should be cleared
}

void Playlist::removeTrack(int index)
{
    if (index < 0 || index >= m_tracks.size()) return;
    m_tracks.remove(index);

    if (m_tracks.isEmpty()) {
        m_currentIndex = -1;
        emit currentTrackChanged(Track()); // emit an empty Track
    }

    else if (index < m_currentIndex) {
        m_currentIndex--;
    }
    
    // if current track removed
    else if (index == m_currentIndex) {
        if (m_currentIndex < m_tracks.size()) {
            emit currentTrackChanged(m_tracks[m_currentIndex]); // play next track automatically
        }
        else {
            m_currentIndex = m_tracks.size() - 1;
            emit currentTrackChanged(m_tracks[m_currentIndex]); 
        }
    }
    emit tracksChanged();
    m_history.clear(); //when the track list changes , the history should be cleared
}

void Playlist::moveTrack(int from, int to)
{
    if (from < 0 || from >= m_tracks.size() || to < 0 || to >= m_tracks.size()) return;
    if (from == to) return;
    m_tracks.move(from, to);
    if (from == m_currentIndex) {
        m_currentIndex = to;
    }
    else if (from < m_currentIndex && to >= m_currentIndex){
        m_currentIndex--;
    }
    else if (from > m_currentIndex && to <= m_currentIndex) {
        m_currentIndex++;
    }
    emit tracksChanged();
    m_history.clear(); //when the track list changes , the history should be cleared
}

void Playlist::clear()
{
    m_tracks.clear();
    m_history.clear();
    m_currentIndex = -1;
    emit tracksChanged();
    emit currentTrackChanged(Track());
}

// ---- getters ----
QVector<Track> Playlist::tracks() const{return m_tracks;}

Track Playlist::currentTrack() const
{
    if (m_currentIndex < 0 || m_currentIndex >= m_tracks.size()) {
        return Track(); 
    }
    return m_tracks[m_currentIndex];
}

int Playlist::currentIndex() const{return m_currentIndex;}

int Playlist::count() const{return m_tracks.size();}

// ---- navigation ----
Track Playlist::next()
{
    if (m_tracks.isEmpty()) return Track();
    int nextIndex = m_currentIndex;
    switch (m_playMode)
    {
        case PlayMode::LoopMode: nextIndex = (m_currentIndex + 1) % m_tracks.size(); m_history.append(m_currentIndex); break; //loop!
        case PlayMode::SingleMode: return m_tracks[m_currentIndex]; //SingleMode doesn't change the history
        case PlayMode::RandomMode: nextIndex = getRandomIndex(); m_history.append(m_currentIndex); break;
    }
    if (m_history.size() > 100) {
        m_history.removeFirst();
    }// store at most 100 history
    m_currentIndex = nextIndex;
    emit currentTrackChanged(m_tracks[m_currentIndex]);
    return m_tracks[m_currentIndex];
}

Track Playlist::prev()
{
    if (m_tracks.isEmpty()) return Track();
    if (m_history.isEmpty()) return Track();
    m_currentIndex = m_history.last();
    m_history.removeLast();
    emit currentTrackChanged(m_tracks[m_currentIndex]);
    return m_tracks[m_currentIndex];
}

// ---- PlayMode ----
Playlist::PlayMode Playlist::playMode() const{return m_playMode;}

void Playlist::setPlayMode(PlayMode mode){m_playMode = mode;}

QString Playlist::playModeString() const
{
    switch (m_playMode) {
    case PlayMode::LoopMode:   return "列表循环";
    case PlayMode::SingleMode: return "单曲循环";
    case PlayMode::RandomMode: return "随机播放";
    }
    return "未知";
}

// ---- search and filter ----
QVector<Track> Playlist::search(const QString &keyword) const
{
    if (keyword.isEmpty()) return m_tracks;
    QVector<Track> results;
    for (const Track &t : m_tracks) {
        if (t.title().contains(keyword, Qt::CaseInsensitive) ||
            t.artist().contains(keyword, Qt::CaseInsensitive) ||
            t.album().contains(keyword, Qt::CaseInsensitive)) {
            results.append(t);
        }
    }
    return results;
}

// ---- private auxiliary method ----
int Playlist::getRandomIndex() const
{
    if (m_tracks.size() <= 1) return 0;
    int idx = QRandomGenerator::global() -> bounded(m_tracks.size());
    while (idx == m_currentIndex){
        idx = QRandomGenerator::global() -> bounded(m_tracks.size());
    }
    return idx;
}