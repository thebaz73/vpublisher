#include "playlistmanager.h"
#include <QDebug>

#include "configurationmanager.h"

PlaylistManager::PlaylistManager(QObject *parent) :
    ConfigurableManager(parent),
    m_max_filesize(5),
    m_playlist_name("")
{
    onUpdateConfiguration();
}

PlaylistManager::~PlaylistManager()
{
}

QFile *PlaylistManager::playlist()
{
    return &m_playlist;
}

void PlaylistManager::onUpdateConfiguration()
{
    m_max_filesize = ConfigurationManager::instance()->value("max_filesize", 5).toInt();
}

void PlaylistManager::updatePlayList(QString fileName, double segmentLength)
{
    QString removed = "";
    m_file_names.append(fileName);
    m_file_lengths.append(segmentLength);
    qDebug("Play List update, file added: %s", qPrintable(fileName));
    if(m_file_names.size() > m_max_filesize) {
        removed = m_file_names.takeFirst();
        m_file_lengths.removeLast();
        qDebug("Play List update, file removed: %s", qPrintable(removed));
    }
    if(m_file_names.size() == m_max_filesize) {
        writePlayList();
        emit playListUpdated(m_playlist.fileName(), m_file_names, removed);
    }
}
