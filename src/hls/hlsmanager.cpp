#include "hlsmanager.h"
#include <QThread>
#include <QDebug>
#include <QDir>

#include "../configurationmanager.h"

HLSManager::HLSManager(QObject *parent) :
    SegmentationManager(APPLE_HLS, parent),
    m_segmenter(NULL)
{
    onUpdateConfiguration();
}

HLSManager::~HLSManager()
{
}

void HLSManager::startSegmentation()
{
    qDebug() << "Initializing HSL manager...";
    config_info config;
    memset(&config, 0, sizeof(config_info));

    config.segment_length = m_segmentLength;
    config.temp_directory = (char *)malloc(sizeof(qPrintable(m_tempDirectory)));
    strcpy((char *)config.temp_directory, qPrintable(m_tempDirectory));
    config.filename_prefix = (char *)malloc(sizeof(qPrintable(m_filenamePrefix)));
    strcpy((char *)config.filename_prefix, qPrintable(m_filenamePrefix));
    config.encoding_profile = (char *)malloc(sizeof(qPrintable(m_encodingProfile)));
    strcpy((char *)config.encoding_profile, qPrintable(m_encodingProfile));
    config.input_filename = (char *)malloc(sizeof(qPrintable(m_inputFilename)));
    strcpy((char *)config.input_filename, qPrintable(m_inputFilename));

    config.pmt_pid = pids.value(PMT_PID);
    config.video_pid = pids.value(VIDEO_PID);
    config.audio_pid = pids.value(AUDIO_PID);

    m_segmenter = new Segmenter();
    m_segmenter->configure(m_adapter_no, config);
#ifdef USING_DEVICE
    m_segmenter->setDtvDevice(m_dtv_device);
#endif
    connect(m_segmenter, SIGNAL(segmentIndexChanged(int,double)), this, SLOT(onSegmentIndexChanged(int,double)));

    QThread* thread = new QThread();
    m_segmenter->moveToThread(thread);
    connect(thread, SIGNAL(started()), m_segmenter, SLOT(process()));
    connect(m_segmenter, SIGNAL(reportExitCode(int)), this, SLOT(onSegmenterExitCode(int)));
    connect(m_segmenter, SIGNAL(finished()), thread, SLOT(quit()));
    connect(m_segmenter, SIGNAL(finished()), m_segmenter, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

void HLSManager::stopSegmentation()
{
    m_segmenter->stop();
}

void HLSManager::onUpdateConfiguration()
{
    m_outdir = ConfigurationManager::instance()->value("hls","tmp_outdir", "/tmp/hls").toString();
    QDir dir(m_outdir);
    if(!dir.exists()) {
        dir.mkpath(m_outdir);
    }
    setTempDirectory(m_outdir);
    setSegmentLength(ConfigurationManager::instance()->value("hls","segment_length", 15).toInt());
    setFilenamePrefix(ConfigurationManager::instance()->value("hls","filename_prefix", "hls_vpub").toString());
    setEncodingProfile(ConfigurationManager::instance()->value("hls","encoding_profile", "128K").toString());
    setInputFilename(ConfigurationManager::instance()->value("hls","input_filename", "../vpubd/etc/test%1.ts").toString().arg(m_adapter_no));
}
