#include "lifecyclemanager.h"
#include <QDebug>
#include <QDir>

#include "configurationmanager.h"

LifecycleManager::LifecycleManager(QObject *parent) :
    QObject(parent),
    m_deviceManager(NULL),
    m_hlsManager(NULL)
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
    if(!m_hlsManager) {
        m_hlsManager = new HLSManager(this);
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

void LifecycleManager::setHlsManager(HLSManager *hlsManager)
{
    m_hlsManager = hlsManager;
}

void LifecycleManager::elaborate()
{
    int adapter_no = 0;
#ifdef USING_DEVICE
    DeviceSettings settings;
    settings.setValue("delivery_system", 3);
    settings.setValue("frequency", 482000000);
    settings.setValue("bandwidth", 8000000);

    if(m_deviceManager->configureDevice(adapter_no, &settings) == 0) {
        qDebug() << "Settings applied";
    }

    DTVDevice *device = m_deviceManager->getDevice(adapter_no);
    if(device->status() == DTVDevice::LOCKED) {
        m_hlsManager->setDtvDevice(device);

        m_hlsManager->setAdapterNumber(adapter_no);
        m_hlsManager->setSegmentLength(ConfigurationManager::instance()->value("hls","segment_length", 15).toInt());
        m_hlsManager->setTempDirectory(m_outdir);
        m_hlsManager->setFilenamePrefix(ConfigurationManager::instance()->value("hls","filename_prefix", "hsl_vpub").toString());
        m_hlsManager->setEncodingProfile(ConfigurationManager::instance()->value("hls","encoding_profile", "").toString());
        m_hlsManager->addPid(PMT_PID, 1300);
        m_hlsManager->addPid(VIDEO_PID, 1301);
        m_hlsManager->addPid(AUDIO_PID, 1302);

        m_hlsManager->doSegmentation();
    }
#endif
#ifdef USING_PIPE
    m_hlsManager->setAdapterNumber(adapter_no);
    m_hlsManager->setSegmentLength(ConfigurationManager::instance()->value("hls","segment_length").toInt());
    m_hlsManager->setTempDirectory(m_outdir);
    m_hlsManager->setFilenamePrefix(ConfigurationManager::instance()->value("hls","filename_prefix").toString());
    m_hlsManager->setEncodingProfile(ConfigurationManager::instance()->value("hls","encoding_profile").toString());
    m_hlsManager->setInputFilename(QString("/tmp/outputpipe%1.ts").arg(adapter_no));

    m_hlsManager->doSegmentation();
#endif
}
