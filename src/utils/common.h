#ifndef COMMON_H
#define COMMON_H

#include <QStringList>
#include <QDebug>
#include <linux/dvb/frontend.h>

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
#endif // COMMON_H
