#include "lifecyclemanager.h"
#include <QDebug>
#include <QDir>

#include "configurationmanager.h"
#include "hsl/hlsmanager.h"

LifecycleManager::LifecycleManager(QObject *parent) :
    QObject(parent),
    m_deviceManager(NULL),
    m_segmentation_manager(NULL)
{
}

LifecycleManager::~LifecycleManager()
{
}

void LifecycleManager::initialize()
{
    qDebug() << "Initializing lifecycle manager...";
#ifdef USING_DEVICE
    Q_ASSERT(m_deviceManager);
#endif
    if(!m_segmentation_manager) {
        m_segmentation_manager = new HLSManager(this);
        connect(m_segmentation_manager, SIGNAL(finished()), this, SLOT(processSegmentationFinished()));
    }
    m_outdir = ConfigurationManager::instance()->value("hls","tmp_outdir", "/tmp/hls").toString();
    QDir dir(m_outdir);
    if(!dir.exists()) {
        dir.mkpath(m_outdir);
    }

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(elaborate()));
    m_timer.setSingleShot(true);
    m_timer.start(2000);
}

void LifecycleManager::setDeviceManager(DeviceManager *deviceManager)
{
    m_deviceManager = deviceManager;
}

void LifecycleManager::elaborate()
{
    int adapter_no = 0;
#ifdef USING_DEVICE
    DeviceSettings settings;
    settings.setValue("delivery_system", 3);
    settings.setValue("frequency", 482000000);
    settings.setValue("bandwidth", 8000000);

    DTVDevice *device = m_deviceManager->getDevice(adapter_no);
    if(device->status() != DTVDevice::LOCKED && device->configure(&settings) == 0) {
        qDebug("Settings applied on device: %s", qPrintable(device->name()));
    }

    if(device->status() == DTVDevice::LOCKED) {
        m_segmentation_manager->setDtvDevice(device);

        m_segmentation_manager->setAdapterNumber(adapter_no);
        ((HLSManager *)m_segmentation_manager)->setSegmentLength(ConfigurationManager::instance()->value("hls","segment_length", 15).toInt());
        ((HLSManager *)m_segmentation_manager)->setTempDirectory(m_outdir);
        ((HLSManager *)m_segmentation_manager)->setFilenamePrefix(ConfigurationManager::instance()->value("hls","filename_prefix", "hsl_vpub").toString());
        ((HLSManager *)m_segmentation_manager)->setEncodingProfile(ConfigurationManager::instance()->value("hls","encoding_profile", "").toString());
        m_segmentation_manager->addPid(PMT_PID, 1300);
        m_segmentation_manager->addPid(VIDEO_PID, 1301);
        m_segmentation_manager->addPid(AUDIO_PID, 1302);

        m_segmentation_manager->doSegmentation();
    }
#endif
#ifdef USING_PIPE
    m_segmentation_manager->setAdapterNumber(adapter_no);
    ((HLSManager *)m_segmentation_manager)->setSegmentLength(ConfigurationManager::instance()->value("hls","segment_length").toInt());
    ((HLSManager *)m_segmentation_manager)->setTempDirectory(m_outdir);
    ((HLSManager *)m_segmentation_manager)->setFilenamePrefix(ConfigurationManager::instance()->value("hls","filename_prefix").toString());
    ((HLSManager *)m_segmentation_manager)->setEncodingProfile(ConfigurationManager::instance()->value("hls","encoding_profile").toString());
    ((HLSManager *)m_segmentation_manager)->setInputFilename(QString("/tmp/outputpipe%1.ts").arg(adapter_no));
    m_segmentation_manager->addPid(PMT_PID, 1300);
    m_segmentation_manager->addPid(VIDEO_PID, 1301);
    m_segmentation_manager->addPid(AUDIO_PID, 1302);

    m_segmentation_manager->doSegmentation();
#endif
}

void LifecycleManager::processSegmentationFinished()
{
    int exitCode = m_segmentation_manager->exitCode();
    if(exitCode != 0) {
        qCritical("Segmentation manager unexpectedly finished (error code=%d). Rescheduling...", exitCode);
        m_timer.setSingleShot(true);
        m_timer.start(2000);
    }
    else {
        qDebug("Segmentation manager finished");
    }
}
