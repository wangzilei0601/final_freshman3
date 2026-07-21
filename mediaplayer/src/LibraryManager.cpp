#include "LibraryManager.h"
#include <QDir>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QMediaPlayer>
#include <QEventLoop>
#include <QTimer>
#include <QStandardPaths>
#include <QDebug>
#include <QMediaMetaData>
#include <QDirIterator>

// ---- constructor ----
LibraryManager::LibraryManager(QObject* parent): QObject(parent){
    m_supportedExtensions << "mp3" << "flac" << "wav" << "m4a" << "aac";
}

// ---- private auxiliary methods ----
Track LibraryManager::extractMetadata(const QString &filePath)
{
    Track track(filePath);
    QFileInfo info(filePath);

    //read duration and labels from file by using QMediaPlayer
    QMediaPlayer player;
    QEventLoop loop;
    bool metaloaded = false;

    // when duration changed,it means the metadata is loaded
    connect(&player, &QMediaPlayer::durationChanged, [&](qint64 duration) {
        track.setDurationMs(static_cast<int>(duration));
        metaloaded = true;
        loop.quit();
    });

    //set a timeout to 3 seconds to prevent infinite loop
    QTimer::singleShot(3000, &loop, [&](){
        if (!metaloaded){
            qDebug() << "Metadata timeout for file: " << filePath;
            loop.quit();
        }
    });

    // start loading
    player.setSource(QUrl::fromLocalFile(filePath));

    //wait for the metadata to be loaded
    loop.exec();

    if (metaloaded){
        QMediaMetaData meta = player.metaData();

        QVariant title = meta.value(QMediaMetaData::Title);
        if (title.isValid() && !title.toString().isEmpty()) {
            track.setTitle(title.toString());
        }

        QVariant artist = meta.value(QMediaMetaData::Author);
        if (artist.isValid() && !artist.toString().isEmpty()) {
            track.setArtist(artist.toString());
        }

        QVariant album = meta.value(QMediaMetaData::AlbumTitle);
        if (album.isValid() && !album.toString().isEmpty()) {
            track.setAlbum(album.toString());
        }

    }

    return track;
}

bool LibraryManager::isAudioFile(const QString &filePath) const
{
    if (filePath.isEmpty()) return false;
    QFileInfo fileInfo(filePath);
    return m_supportedExtensions.contains(fileInfo.suffix().toLower());
}

// ---- scan ----
void LibraryManager::scanDirectory(const QString &path)
{
    if (path.isEmpty()) return;
    QDir dir(path);
    if (!dir.exists()) {
        qDebug() << "Directory does not exist:" << path;
        return;
    }

    QVector<Track> scannedTracks;
    //filter and get the files under the directory path
    QDirIterator it(path, QDir::Files | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);

    int count = 0;
    while(it.hasNext()){
        it.next();
        QString filePath = it.filePath();
        if (isAudioFile(filePath)) {
            Track track = extractMetadata(filePath);
            scannedTracks.append(track);
            count++;
            if (count % 5 == 0){
                emit scanProgress(count, -1);//-1 means unknown total number of files
                //total is remained for showing percentage of the progress,but it costs a lot of time, so I don't use it. 
            }
        }
    }

    QSet<QString> existingPaths;
    existingPaths.reserve(m_tracks.size()); // preallocate space for efficiency
    for (const Track &t : m_tracks) {
        existingPaths.insert(t.filePath());
    }
    for (const Track &t : scannedTracks) {
    if (!existingPaths.contains(t.filePath())) {
        m_tracks.append(t);
        existingPaths.insert(t.filePath()); // prevent duplicates
    }
    }

    emit scanFinished(m_tracks);
}

// ---- visit tracks ----
QVector<Track> LibraryManager::tracks() const{return m_tracks;}

void LibraryManager::setTracks(const QVector<Track> &tracks){m_tracks = tracks;}

// ---- memory ----
bool LibraryManager::saveToFile(const QString &filePath) const
{
    QJsonArray tracksArray;
    for (const Track &t : m_tracks) {
        QJsonObject obj;
        obj["filePath"] = t.filePath();
        obj["title"] = t.title();
        obj["artist"] = t.artist();
        obj["album"] = t.album();
        obj["durationMs"] = t.durationMs();
        obj["lyricsOffsetMs"] = t.lyricsOffsetMs();
        tracksArray.append(obj);
    }

    QJsonDocument doc(tracksArray);
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)){
        qDebug() << "Failed to open file for writing:" << filePath;
        return false;
    }
    file.write(doc.toJson());
    file.close();
    return true;
}

bool LibraryManager::loadFromFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.exists()) return false;
    if (!file.open(QIODevice::ReadOnly)) return false;

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isArray()) return false;

    QJsonArray arr = doc.array();
    QVector<Track> loadedTracks;
    for (const QJsonValue &val : arr) {
        QJsonObject obj = val.toObject();
        Track t(obj["filePath"].toString());
        t.setTitle(obj["title"].toString());
        t.setArtist(obj["artist"].toString());
        t.setAlbum(obj["album"].toString());
        t.setDurationMs(obj["durationMs"].toInt());
        t.setLyricsOffsetMs(obj["lyricsOffsetMs"].toInt());
        loadedTracks.append(t);
    }
    
    m_tracks = loadedTracks;
    return true;
}

QString LibraryManager::defaultStoragePath() const
{
    // use the directory recommended by Qt
    QString appData = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(appData);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    return appData + "/library.json";
}
