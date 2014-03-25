#ifndef PLAYLISTMANAGER_H
#define PLAYLISTMANAGER_H

#include <QObject>
#include <QString>
#include <QList>
#include <QFile>

#include "configurablemanager.h"


class PlaylistManager : public ConfigurableManager
{
    Q_OBJECT
public:
    explicit PlaylistManager(QObject *parent = 0);
    ~PlaylistManager();

    QFile *playlist();
    virtual void writePlayList() = 0;


signals:
    void playListUpdated(QString playListName, QList<QString> fileNames, QString removedFileName);
public slots:
    virtual void onUpdateConfiguration();
    void updatePlayList(QString fileName, double segmentLength);
protected:
    int m_max_filesize;
    QString m_playlist_name;
    QFile m_playlist;
    QList<QString> m_file_names;
    QList<double> m_file_lengths;
};

#endif // PLAYLISTMANAGER_H
