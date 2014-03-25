#ifndef SEGMENTER_H
#define SEGMENTER_H

#include <QObject>
#include <QMutex>
#include <QWaitCondition>

#ifdef __cplusplus
extern "C" {
#endif
#include <libavformat/avformat.h>
#ifdef __cplusplus
}
#endif

#include "../utils/common.h"
#include "../dev/dtvdevice.h"

class Segmenter : public QObject
{
    Q_OBJECT
public:
    explicit Segmenter(QObject *parent = 0);
    ~Segmenter();

    void configure(int adapterNo, config_info config);

    DTVDevice *dtvDevice() const;
    void setDtvDevice(DTVDevice *dtvDevice);

    int doRead(quint8 *buf, int buf_size);
    qint64 doSeek(qint64 offset, int whence);

    int exitCode() const;

signals:
    void finished();
    void reportExitCode(int exitCode);
    void segmentIndexChanged(int segmentIndex, double segmentLength);

public slots:
    void process();
    void suspend();
    void resume();
    void stop();
private:
    bool m_running;
    bool m_suspend;
    int m_exit_code;
    QWaitCondition m_suspend_condition;
    QMutex m_suspend_mutex;
    int m_adapter_no;
    bool m_video_started;
    config_info m_config;

    AVInputFormat *m_input_format;
    AVIOContext *m_pb;
    AVFormatContext *m_input_context;
    AVOutputFormat *m_output_format;
    AVFormatContext *m_output_context;

    int m_video_index;
    int m_audio_index;

    AVStream *m_video_stream;
    AVStream *m_audio_stream;

    unsigned int m_first_segment;
    unsigned int m_last_segment;
    unsigned int m_current_segment_index;

    unsigned char *m_buffer;
    int m_input_fd;
    int m_dvr_fd;
    int m_dmx_fd;

    DTVDevice *m_dtv_device;

    AVStream *addOutputStream(AVFormatContext *output_format_context, AVStream *input_stream);

    int openPipe();
    int openDevice();
    void closeDevice();
    int configureDevice();
    int openInputContext();
    int openOutputContext();

    int decode();
    int writeSegments();
    void finalize();

    void avPrintError(int err);
};

#endif // SEGMENTER_H
