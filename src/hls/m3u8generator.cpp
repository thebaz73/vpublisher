#include "m3u8generator.h"
#include <QDir>
#include <QTextStream>
#include <QDebug>

#include "../configurationmanager.h"

M3U8Generator::M3U8Generator(QObject *parent) :
    PlaylistManager(parent),
    m_count_revision(0),
    m_max_media_segment_duration(0),
    m_media_segment_duration(0),
    m_segment_url("")
{
    m_template << "#EXTM3U\n" <<
                  "#EXT-X-TARGETDURATION:%1\n" <<
                  "#EXT-X-VERSION:3\n" <<
                  "#EXT-X-MEDIA-SEQUENCE:%1\n" <<
                  "#EXTINF:%1,\n" <<
                  "%1/%2\n";
    onUpdateConfiguration();
}

M3U8Generator::~M3U8Generator()
{
}

void M3U8Generator::writePlayList()
{    
    if(m_playlist.exists()) {
        m_playlist.remove();
    }
    m_playlist.open(QFile::ReadWrite);
    QTextStream stream(&m_playlist);
    stream << m_template.at(0);
    stream << m_template.at(1).arg(m_max_media_segment_duration);
    stream << m_template.at(2);
    stream << m_template.at(3).arg(++m_count_revision);
    for (int i = 0; i < m_file_names.size(); ++i) {
        stream << m_template.at(4).arg(m_file_lengths.at(i));
        stream << m_template.at(5).arg(m_segment_url).arg(m_file_names.at(i));
    }
    stream.flush();
    m_playlist.close();
}

void M3U8Generator::onUpdateConfiguration()
{
    PlaylistManager::onUpdateConfiguration();
    m_max_media_segment_duration = ConfigurationManager::instance()->value("hls", "segment_length", 15).toInt();
    m_media_segment_duration = ConfigurationManager::instance()->value("hls", "segment_length", 15).toInt();
    m_playlist_name = ConfigurationManager::instance()->value("hls", "playlist_name", "hsl_vpub.m3u8").toString();
    m_segment_url = ConfigurationManager::instance()->value("hls", "segment_url", "http://localhost:8080/vpub").toString();
    QDir tmp_dir(ConfigurationManager::instance()->value("hls","tmp_outdir", QDir::tempPath() + "/hls").toString());
    m_playlist.setFileName(tmp_dir.absoluteFilePath(m_playlist_name));
}
