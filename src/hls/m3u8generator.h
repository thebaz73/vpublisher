#ifndef M3U8GENERATOR_H
#define M3U8GENERATOR_H

#include <QObject>
#include <QStringList>
#include <QFile>
#include "../playlistmanager.h"

class M3U8Generator : public PlaylistManager
{
    Q_OBJECT
public:
    explicit M3U8Generator(QObject *parent = 0);
    ~M3U8Generator();

    virtual void writePlayList();
public slots:
    virtual void onUpdateConfiguration();
private:
    int m_count_revision;
    QStringList m_template;
    int m_max_media_segment_duration;
    int m_media_segment_duration;
    QString m_segment_url;
};

#endif // M3U8GENERATOR_H
