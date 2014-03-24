#include "segmenter.h"
#include <cstdio>
#include <fcntl.h>

int io_read(void *opaque, uint8_t *buf, int buf_size)
{
    Segmenter *segmenter = reinterpret_cast<Segmenter *>(opaque);

    return segmenter->doRead(buf, buf_size);
}

int64_t io_seek(void *opaque, int64_t offset, int whence)
{
    Segmenter *segmenter = reinterpret_cast<Segmenter *>(opaque);

    return segmenter->doSeek(offset, whence);
}

Segmenter::Segmenter(QObject *parent) :
    QObject(parent),
    m_running(true),
    m_suspend(false),
    m_exit_code(0),
    m_input_format(NULL),
    m_pb(NULL),
    m_input_context(NULL),
    m_output_format(NULL),
    m_output_context(NULL),
    m_video_index(-1),
    m_audio_index(-1),
    m_video_stream(NULL),
    m_audio_stream(NULL),
    m_first_segment(0),
    m_last_segment(0),
    m_dtv_device(NULL)
{
    /*
     * Input Format context init
     */
    m_buffer=(unsigned char*)av_malloc(SEGMENTER_PKT_BUF_SIZE + FF_INPUT_BUFFER_PADDING_SIZE);
    if(!m_buffer) {
        qCritical("Unable to allocate input buffer");
    }

    /* set end of buffer to 0 (this ensures that no overreading happens for damaged mpeg streams) */
    memset(m_buffer + SEGMENTER_PKT_BUF_SIZE, 0, FF_INPUT_BUFFER_PADDING_SIZE);

    av_log_set_level(AV_LOG_FATAL);
    av_register_all();
}

Segmenter::~Segmenter()
{
}

void Segmenter::configure(int adapterNo, config_info config)
{
    m_config = config;
    m_config.dev_input_buffer = 1024 * 1024;
    m_adapter_no = adapterNo;
}

DTVDevice *Segmenter::dtvDevice() const
{
    return m_dtv_device;
}

void Segmenter::setDtvDevice(DTVDevice *dtv_device)
{
    m_dtv_device = dtv_device;
}

int Segmenter::doRead(quint8 *buf, int buf_size)
{
#ifdef USING_PIPE
    int packet_number = buf_size / TS_PACKET_SIZE;
    int packet_count = 0;

    u_int8_t packet[TS_PACKET_SIZE];
    while(packet_count <= packet_number) {
        int i_ret = read(m_input_fd, packet, TS_PACKET_SIZE);
        if(i_ret < 188 || i_ret == -1) break;
        if(packet[0] != 0x47) {
            qWarning("Not synched");
        }
        else {
            u_int16_t pid = ((u_int16_t)(packet[1] & 0x1f) << 8) + packet[2];
            if(pid == 0) {
                packet_count++
                memcpy(&buf[packet_count * TS_PACKET_SIZE], packet, TS_PACKET_SIZE);
            }
            else {
                for (int i = 0; i < m_config.num_filtered_pid; ++i) {
                    packet_count++
                    if(pid == m_config.filtered_pid[i]) {
                        memcpy(&buf[packet_count * TS_PACKET_SIZE], packet, TS_PACKET_SIZE);
                    }
                }
            }
        }
    }

    return packet_count * TS_PACKET_SIZE;
#endif
#ifdef USING_DEVICE
    int numReadPackets;
    int packet_count;
    int packet_number = buf_size / TS_PACKET_SIZE;

    unsigned char *packet;
    unsigned char* buffer;
    do {
        buffer = m_dtv_device->readData(m_input_fd, &numReadPackets, buf_size);
    }
    while(buffer == NULL);

    for(packet_count = 0; packet_count < packet_number; packet_count++) {
        packet = buffer + (packet_count * TS_ENVELOPE_SIZE);
        u_int16_t pid = ((u_int16_t)(packet[1] & 0x1f) << 8) + packet[2];
        if(pid == PAT_PID || pid == m_config.pmt_pid || pid == m_config.video_pid || pid == m_config.audio_pid) {
            memcpy(&buf[packet_count * TS_PACKET_SIZE], packet, TS_PACKET_SIZE);
        }
    }

    return packet_count * TS_PACKET_SIZE;
#endif
}

qint64 Segmenter::doSeek(qint64 offset, int whence)
{
    Q_UNUSED(offset);
    Q_UNUSED(whence);

    return -1;
}

int Segmenter::exitCode() const
{
    return m_exit_code;
}

void Segmenter::process()
{
#ifdef USING_PIPE
    qDebug("Opening pipe: %s...\n", m_config.input_filename);
    m_exit_code = openPipe();
#endif

#ifdef USING_DEVICE
    qDebug("Opening device: %s...\n", qPrintable(m_dtv_device->name()));
    m_exit_code = openDevice();
    m_exit_code = ((m_exit_code==0) ? configureDevice() : m_exit_code);
#endif

    if(!m_exit_code) {
        qDebug("Inizializing input...\n");
        m_exit_code = openInputContext();
        qDebug("Inizializing output...\n");
        m_exit_code += openOutputContext();


        if(!m_exit_code && m_running) {
            qDebug("Decoding...\n");
            m_exit_code = decode();
            if(!m_exit_code) {
                qDebug("Writing segments...\n");
                m_exit_code += writeSegments();
                qDebug("Finalizing...\n");
                finalize();
            }
        }
    }    
#ifdef USING_DEVICE
    closeDevice();
#endif
    emit reportExitCode(m_exit_code);
    emit finished();
}

void Segmenter::suspend()
{
    this->m_suspend = true;
}

void Segmenter::resume()
{
    this->m_suspend = false;
    m_suspend_condition.wakeAll();
}

void Segmenter::stop()
{
    if(m_suspend) {
        resume();
    }
    m_running = false;
}


AVStream *Segmenter::addOutputStream(AVFormatContext *output_format_context, AVStream *input_stream)
{
    AVCodecContext *input_codec_context;
    AVCodecContext *output_codec_context;
    AVStream *output_stream;

    output_stream = avformat_new_stream(output_format_context, 0);
    if (!output_stream)
    {
        qWarning("Segmenter error: Could not allocate stream\n");
        return NULL;
    }

    input_codec_context = input_stream->codec;
    output_codec_context = output_stream->codec;

    output_codec_context->codec_id = input_codec_context->codec_id;
    output_codec_context->codec_type = input_codec_context->codec_type;
    output_codec_context->codec_tag = input_codec_context->codec_tag;
    output_codec_context->bit_rate = input_codec_context->bit_rate;
    output_codec_context->extradata = input_codec_context->extradata;
    output_codec_context->extradata_size = input_codec_context->extradata_size;

    if(av_q2d(input_codec_context->time_base) * input_codec_context->ticks_per_frame > av_q2d(input_stream->time_base) && av_q2d(input_stream->time_base) < 1.0/1000)
    {
        output_codec_context->time_base = input_codec_context->time_base;
        output_codec_context->time_base.num *= input_codec_context->ticks_per_frame;
    }
    else
    {
        output_codec_context->time_base = input_stream->time_base;
    }

    switch (input_codec_context->codec_type)
    {
    case AVMEDIA_TYPE_AUDIO:
        output_codec_context->channel_layout = input_codec_context->channel_layout;
        output_codec_context->sample_rate = input_codec_context->sample_rate;
        output_codec_context->channels = input_codec_context->channels;
        output_codec_context->frame_size = input_codec_context->frame_size;
        if ((input_codec_context->block_align == 1 && input_codec_context->codec_id == CODEC_ID_MP3) || input_codec_context->codec_id == CODEC_ID_AC3)
        {
            output_codec_context->block_align = 0;
        }
        else
        {
            output_codec_context->block_align = input_codec_context->block_align;
        }
        break;
    case AVMEDIA_TYPE_VIDEO:
        output_codec_context->pix_fmt = input_codec_context->pix_fmt;
        output_codec_context->width = input_codec_context->width;
        output_codec_context->height = input_codec_context->height;
        output_codec_context->has_b_frames = input_codec_context->has_b_frames;

        if (output_format_context->oformat->flags & AVFMT_GLOBALHEADER)
        {
            output_codec_context->flags |= CODEC_FLAG_GLOBAL_HEADER;
        }
        break;
    default:
        break;
    }

    return output_stream;
}

void Segmenter::outputTransferCommand(const unsigned int first_segment, const unsigned int last_segment, const int end, const char *encoding_profile)
{
    char buffer[1024 * 10];
    memset(buffer, 0, sizeof(char) * 1024 * 10);

    sprintf(buffer, "%d, %d, %d, %s", first_segment, last_segment, end, encoding_profile);

    qDebug("Segmenter: %s\n\r", buffer);
}

int Segmenter::openPipe()
{
    m_input_fd = open(m_config.input_filename, O_RDONLY);
    if(m_input_fd < 0) {
        return 1;
    }

    return 0;
}

int Segmenter::openDevice()
{
    int retValue = 0;
    m_dvr_fd = m_dtv_device->openDvbdvr(true, false);
    if(m_dvr_fd < 0) {
        retValue = 1;
    }
    m_input_fd = m_dvr_fd;

    return retValue;
}

void Segmenter::closeDevice() {
    if(m_dvr_fd > 0) {
        m_dtv_device->closeDvbdvr(m_dvr_fd);
    }
    if(m_dmx_fd > 0) {
        m_dtv_device->stopDvbdemux(m_dmx_fd);
        m_dtv_device->closeDvbdemux(m_dmx_fd);
    }
}

int Segmenter::configureDevice()
{
    int retValue = 0;
    if(m_dtv_device->setDvbBuffer(m_dvr_fd, m_config.dev_input_buffer) != 0) {
        retValue = 1;
    }

    m_dmx_fd = m_dtv_device->openDvbdemux(true);
    if(m_dmx_fd < 0) {
        m_dtv_device->closeDvbdemux(m_dmx_fd);
        retValue = 2;
    }
    else if(m_dtv_device->addPidFilter(m_dmx_fd, 0, true) < 0) {
        retValue = 3;
    }

    return retValue;
}

int Segmenter::openInputContext()
{
    m_input_format = av_find_input_format("mpegts");
    if (!m_input_format) {
      qWarning("Segmenter error: Could not find MPEG-TS demuxer\n");
      return 1;
    }

    m_input_context = avformat_alloc_context();
    if (!m_input_context) {
      qWarning("Segmenter error: Could not allocated input context");
      return 2;
    }

    m_input_context->pb = avio_alloc_context(m_buffer, SEGMENTER_PKT_BUF_SIZE, 0, this, &io_read, NULL, &io_seek);
    if(!m_input_context->pb) {
        qDebug("Unable to allocate AVIO context");
        return 3;
    }
    m_input_context->max_analyze_duration = MAX_ANALYZE_DURATION;
    m_input_context->max_index_size = MAX_INDEX_SIZE;

    int ret = avformat_open_input(&m_input_context, "dummy", m_input_format, NULL);
    if (ret != 0) {
      qWarning("Segmenter error: Could not open input file, make sure it is an mpegts file: %d\n", ret);
      avPrintError(ret);
      return 4;
    }

    if (avformat_find_stream_info(m_input_context, NULL) < 0) {
      qWarning("Segmenter error: Could not read stream information\n");
      return 3;
    }

    return 0;
}

int Segmenter::openOutputContext()
{
    m_output_format = av_guess_format("mpegts", NULL, NULL);
    if (!m_output_format) {
        qWarning("Segmenter error: Could not find MPEG-TS muxer\n");
        return 1;
    }

    m_output_context = avformat_alloc_context();
    if (!m_output_context) {
      qWarning("Segmenter error: Could not allocated output context");
      return 2;
    }
    m_output_context->oformat = m_output_format;

    return 0;
}

int Segmenter::decode()
{
    m_video_index = -1;
    m_audio_index = -1;

    for (uint i = 0; i < m_input_context->nb_streams && (m_video_index < 0 || m_audio_index < 0); i++) {
      switch (m_input_context->streams[i]->codec->codec_type) {
        case AVMEDIA_TYPE_VIDEO:
          m_video_index = i;
          m_input_context->streams[i]->discard = AVDISCARD_NONE;
          m_video_stream = addOutputStream(m_output_context, m_input_context->streams[i]);
          if(!m_video_stream) return 1;
          break;
        case AVMEDIA_TYPE_AUDIO:
          m_audio_index = i;
          m_input_context->streams[i]->discard = AVDISCARD_NONE;
          m_audio_stream = addOutputStream(m_output_context, m_input_context->streams[i]);
          if(!m_audio_stream) return 1;
          break;
        default:
          m_input_context->streams[i]->discard = AVDISCARD_ALL;
          break;
      }
    }

//    if (av_set_parameters(output_context, NULL) < 0) {
//        qWarning("Segmenter error: Invalid output format parameters\n");
//        return 1;
//    }

    av_dump_format(m_output_context, 0, m_config.filename_prefix, 1);

    if(m_video_index >= 0)
    {
      AVCodec *codec = avcodec_find_decoder(m_video_stream->codec->codec_id);
      if (!codec) {
        qWarning("Segmenter error: Could not find video decoder, key frames will not be honored\n");
        return 2;
      }

      if (avcodec_open2(m_video_stream->codec, codec, NULL) < 0) {
        qWarning("Segmenter error: Could not open video decoder, key frames will not be honored\n");
        return 3;
      }
    }

    return 0;
}

int Segmenter::writeSegments()
{
    char *output_filename = (char *)malloc(sizeof(char) * (strlen(m_config.temp_directory) + 1 + strlen(m_config.filename_prefix) + 10));
    if (!output_filename) {
        qWarning("Segmenter error: Could not allocate space for output filenames\n");
        return 1;
    }

    unsigned int output_index = 1;
    snprintf(output_filename, strlen(m_config.temp_directory) + 1 + strlen(m_config.filename_prefix) + 10, "%s/%s-%05u.ts", m_config.temp_directory, m_config.filename_prefix, output_index++);

    int ret = avio_open(&m_output_context->pb, output_filename, AVIO_FLAG_WRITE);
    if (ret < 0) {
        qWarning("Segmenter error: Could not open '%s'\n", output_filename);
        return 1;
    }

    ret = avformat_write_header(m_output_context, NULL);
    if (ret != 0) {
        qWarning("Segmenter error: Could not write mpegts header to first output file\n");
        return 1;
    }

    m_first_segment = 1;
    m_last_segment = 0;

    int retValue = 0;
    double prev_segment_time = 0;
    int decode_done;
    do {
        if(m_suspend) {
            m_suspend_condition.wait(&m_suspend_mutex);
        }

        double segment_time;
        AVPacket packet;
        av_init_packet(&packet);

        decode_done = av_read_frame(m_input_context, &packet);
        if (decode_done < 0) {
            break;
        }
        if (av_dup_packet(&packet) < 0) {
            qWarning("Segmenter error: Could not duplicate packet");
            av_free_packet(&packet);
            retValue = 1;
            break;
        }

        if (packet.stream_index == m_video_index && (packet.flags & AV_PKT_FLAG_KEY)) {
            segment_time = (double)m_video_stream->pts.val * m_video_stream->time_base.num / m_video_stream->time_base.den;
        }
        else if (m_video_index < 0) {
            segment_time = (double)m_audio_stream->pts.val * m_audio_stream->time_base.num / m_audio_stream->time_base.den;
        }
        else {
            segment_time = prev_segment_time;
        }

        // done writing the current file?
        if (segment_time - prev_segment_time >= m_config.segment_length) {
            avio_flush(m_output_context->pb);
            avio_close(m_output_context->pb);

            outputTransferCommand(m_first_segment, ++m_last_segment, 0, m_config.encoding_profile);

            snprintf(output_filename, strlen(m_config.temp_directory) + 1 + strlen(m_config.filename_prefix) + 10, "%s/%s-%05u.ts", m_config.temp_directory, m_config.filename_prefix, output_index++);
            qDebug("Segmenter: Writing output to %s", output_filename);
            if (avio_open(&m_output_context->pb, output_filename, AVIO_FLAG_WRITE) < 0) {
                qWarning("Segmenter error: Could not open '%s'\n", output_filename);
                retValue = 2;
                break;
            }

            prev_segment_time = segment_time;
        }

        int ret = av_interleaved_write_frame(m_output_context, &packet);
        if (ret < 0) {
            //qDebug("Segmenter error: Could not write frame of stream: (error_code=%d)\n", ret);
        }
        else if (ret > 0) {
            qDebug("Segmenter info: End of stream requested\n");
            av_free_packet(&packet);
            break;
        }

        av_free_packet(&packet);
    } while (!decode_done && m_running);

    av_write_trailer(m_output_context);

    return retValue;
}

void Segmenter::finalize()
{
    qDebug("Segmenter info: Executing cleaning procedures\n");
    if (m_video_index >= 0)  {
        avcodec_close(m_video_stream->codec);
    }

    for(uint i = 0; i < m_output_context->nb_streams; i++)  {
        av_freep(&m_output_context->streams[i]->codec);
        av_freep(&m_output_context->streams[i]);
    }

    avio_close(m_output_context->pb);
    av_free(m_output_context);

    outputTransferCommand(m_first_segment, ++m_last_segment, 1, m_config.encoding_profile);
}

void Segmenter::avPrintError(int err)
{
    char errbuf[128];
    char *buf = errbuf;

    if (av_strerror(err, buf, sizeof(*buf)) < 0)
        buf = strerror(AVUNERROR(err));

    qDebug("AV error[%d]: %s\n", err, buf);
}
