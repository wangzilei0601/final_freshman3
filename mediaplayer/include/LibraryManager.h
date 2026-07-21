#ifndef LIBRARYMANAGER_H
#define LIBRARYMANAGER_H

#include <QObject>
#include <QVector>
#include "Track.h"

class LibraryManager : public QObject
{
    Q_OBJECT
public:
    // ---- constructor ----
    explicit LibraryManager(QObject *parent = nullptr);

    // ---- scan ----
    void scanDirectory(const QString &path);

    // ---- visit tracks ----
    QVector<Track> tracks() const;
    void setTracks(const QVector<Track> &tracks);

    // ---- memory ----
    bool saveToFile(const QString &filePath) const; // save tracks to JSON file
    bool loadFromFile(const QString &filePath); // load tracks from JSON file
    QString defaultStoragePath() const;  // return default storage path

signals:
    void scanProgress(int current, int total); // used for UI progress bar
    void scanFinished(const QVector<Track> &tracks); 

private:
    QVector<Track> m_tracks;
    QStringList m_supportedExtensions; // supported extensions

    Track extractMetadata(const QString &filePath); //extract meta data from file
    bool isAudioFile(const QString &filePath) const; // check if the file is a supported audio file
};

#endif // LIBRARYMANAGER_H