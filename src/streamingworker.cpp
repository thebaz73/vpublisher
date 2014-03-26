#include "streamingworker.h"

#include <QDir>

#include "hls/hlsmanager.h"
#include "hls/m3u8generator.h"

#include "configurationmanager.h"

StreamingWorker::StreamingWorker(QObject *parent) :
    QThread(parent),
    m_segmentation_manager(NULL)
{
}

StreamingWorker::~StreamingWorker()
{
}

void StreamingWorker::initialize()
{
    m_deployment_manager = new DeploymentManager(this);

    if(ConfigurationManager::instance()->value("hls_streaming", false).toBool()) {
        m_segmentation_manager = new HLSManager(this);
        m_playlist_manager = new M3U8Generator(this);
        m_deployment_manager->setSourceFolder(ConfigurationManager::instance()->value("hls","tmp_outdir", QDir::tempPath() + "/hls").toString());
    }

    connect(m_segmentation_manager, SIGNAL(finished()), this, SLOT(processSegmentationFinished()));
    connect(m_segmentation_manager, SIGNAL(segmentChanged(QString, double)), m_playlist_manager, SLOT(updatePlayList(QString, double)));
    connect(m_playlist_manager, SIGNAL(playListUpdated(QString,QList<QString>,QString)), m_deployment_manager, SLOT(deploy(QString,QList<QString>,QString)));
#ifdef USING_DEVICE
    if(m_device->status() == DTVDevice::LOCKED) {
        m_segmentation_manager->setDtvDevice(m_device);
    }
#endif

    m_segmentation_manager->setAdapterNumber(m_adapterNumber);
//    m_segmentation_manager->addPid(PMT_PID, 66);
//    m_segmentation_manager->addPid(VIDEO_PID, 69);
//    m_segmentation_manager->addPid(AUDIO_PID, 68);
    m_segmentation_manager->addPid(PMT_PID, 1300);
    m_segmentation_manager->addPid(VIDEO_PID, 1301);
    m_segmentation_manager->addPid(AUDIO_PID, 1302);
}

int StreamingWorker::adapterNumber() const
{
    return m_adapterNumber;
}

void StreamingWorker::setAdapterNumber(int adapterNumber)
{
    m_adapterNumber = adapterNumber;
}

DTVDevice *StreamingWorker::device() const
{
    return m_device;
}

void StreamingWorker::setDevice(DTVDevice *device)
{
    m_device = device;
}

void StreamingWorker::stopWorker()
{
    m_segmentation_manager->stopSegmentation();
}

void StreamingWorker::run()
{
#ifdef USING_DEVICE
    if(m_device->status() == DTVDevice::LOCKED) {
        m_segmentation_manager->startSegmentation();
    }
    else {
        qDebug("Segmentation manager not started, device not locked");
        emit exitStatus(NO_ERROR);
    }
#endif
#ifdef USING_PIPE
    m_segmentation_manager->startSegmentation();
#endif
}

void StreamingWorker::processSegmentationFinished()
{
    int exitCode = m_segmentation_manager->exitCode();
    if(exitCode != 0) {
        qCritical("Segmentation manager unexpectedly finished (error code=%d).", exitCode);
        emit exitStatus(SEGMENTER_ERROR);
    }
    else {
        qDebug("Segmentation manager finished");
        emit exitStatus(NO_ERROR);
    }
}

