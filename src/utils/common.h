#ifndef COMMON_H
#define COMMON_H

#include <QStringList>
#include <QDebug>
#include <linux/dvb/frontend.h>

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/time.h>

#define ERROR                   0
#define NOTICE                  1
#define INFO                    2
#define DEBUG                   3

#define print(x, y, z, fmt, arg...) do {				\
    if (z) {							\
        if	((x > ERROR) && (x > y))			\
            vprint("%s: " fmt "\n", __func__ , ##arg);	\
        else if	((x > NOTICE) && (x > y))			\
            vprint("%s: " fmt "\n",__func__ , ##arg);	\
        else if ((x > INFO) && (x > y))				\
            vprint("%s: " fmt "\n", __func__ , ##arg);	\
        else if ((x > DEBUG) && (x > y))			\
            vprint("%s: " fmt "\n", __func__ , ##arg);	\
    } else {							\
        if (x > y)						\
            vprint(fmt, ##arg);				\
    }								\
} while(0)

#define PAT_PID                     0
#define NUM_PID                     8192
#define TS_PACKET_SIZE              188
#define TS_ENVELOPE_SIZE            256
#define READ_TS_PACKET_SIZE         512
#define TS_SYNCH_BYTE               0x47
#define SEGMENTER_PKT_BUF_SIZE		188*174
#define MAX_ANALYZE_DURATION        2147483647      //1000000
#define MAX_INDEX_SIZE              2147483647      //4*1024*1024

enum PidType {PMT_PID = 0, VIDEO_PID, AUDIO_PID};

typedef struct config_info_s
{
  const char    *input_filename;
  int           segment_length;
  const char    *temp_directory;
  const char    *filename_prefix;
  const char    *encoding_profile;
  u_int16_t     pmt_pid;
  u_int16_t     video_pid;
  u_int16_t     audio_pid;
  size_t        dev_input_buffer;
} config_info;

typedef struct capabilities_s {
    fe_caps_t   cap;
    QString     value;
} capabilities_t;

static capabilities_t lookup_table[] = {
    {FE_IS_STUPID,"FE_IS_STUPID"},
    {FE_CAN_INVERSION_AUTO,"FE_CAN_INVERSION_AUTO"},
    {FE_CAN_FEC_1_2,"FE_CAN_FEC_1_2"},
    {FE_CAN_FEC_2_3,"FE_CAN_FEC_2_3"},
    {FE_CAN_FEC_3_4,"FE_CAN_FEC_3_4"},
    {FE_CAN_FEC_4_5,"FE_CAN_FEC_4_5"},
    {FE_CAN_FEC_5_6,"FE_CAN_FEC_5_6"},
    {FE_CAN_FEC_6_7,"FE_CAN_FEC_6_7"},
    {FE_CAN_FEC_7_8,"FE_CAN_FEC_7_8"},
    {FE_CAN_FEC_8_9,"FE_CAN_FEC_8_9"},
    {FE_CAN_FEC_AUTO,"FE_CAN_FEC_AUTO"},
    {FE_CAN_QPSK,"FE_CAN_QPSK"},
    {FE_CAN_QAM_16,"FE_CAN_QAM_16"},
    {FE_CAN_QAM_32,"FE_CAN_QAM_32"},
    {FE_CAN_QAM_64,"FE_CAN_QAM_64"},
    {FE_CAN_QAM_128,"FE_CAN_QAM_128"},
    {FE_CAN_QAM_256,"FE_CAN_QAM_256"},
    {FE_CAN_QAM_AUTO,"FE_CAN_QAM_AUTO"},
    {FE_CAN_TRANSMISSION_MODE_AUTO,"FE_CAN_TRANSMISSION_MODE_AUTO"},
    {FE_CAN_BANDWIDTH_AUTO,"FE_CAN_BANDWIDTH_AUTO"},
    {FE_CAN_GUARD_INTERVAL_AUTO,"FE_CAN_GUARD_INTERVAL_AUTO"},
    {FE_CAN_HIERARCHY_AUTO,"FE_CAN_HIERARCHY_AUTO"},
    {FE_CAN_8VSB,"FE_CAN_8VSB"},
    {FE_CAN_16VSB,"FE_CAN_16VSB"},
    {FE_HAS_EXTENDED_CAPS,"FE_HAS_EXTENDED_CAPS"},
    {FE_CAN_TURBO_FEC,"FE_CAN_TURBO_FEC"},
    {FE_CAN_2G_MODULATION,"FE_CAN_2G_MODULATION"},
    {FE_NEEDS_BENDING,"FE_NEEDS_BENDING"},
    {FE_CAN_RECOVER,"FE_CAN_RECOVER"},
    {FE_CAN_MUTE_TS,"FE_CAN_MUTE_TS"}
};


static inline QStringList capabilities(fe_caps_t  caps) {
    QStringList list;
    for (int i = 0; i < 30; ++i) {
        if((caps & lookup_table[i].cap) == lookup_table[i].cap) {
            list << lookup_table[i].value;
        }

    }
    return list;
}

static inline void vprint(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
}

static inline int time_after(struct timeval oldtime, uint32_t delta_ms)
{
    // calculate the oldtime + add on the delta
    uint64_t oldtime_ms = (oldtime.tv_sec * 1000) + (oldtime.tv_usec / 1000);
    oldtime_ms += delta_ms;

    // calculate the nowtime
    struct timeval nowtime;
    gettimeofday(&nowtime, 0);
    uint64_t nowtime_ms = (nowtime.tv_sec * 1000) + (nowtime.tv_usec / 1000);

    // check
    return nowtime_ms > oldtime_ms;
}

#endif // COMMON_H
