#include "lifecyclemanager.h"
#include <QDebug>
#include <QDir>

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
    m_outdir = QDir::tempPath()+"/hls";
    QDir dir(QDir::tempPath()+"/hls");
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

        DTVDevice *device = m_deviceManager->getDevice(adapter_no);
        m_hlsManager->setDtvDevice(device);

        m_hlsManager->setAdapterNumber(adapter_no);
        m_hlsManager->setSegmentLength(15);
        m_hlsManager->setTempDirectory(m_outdir);
        m_hlsManager->setFilenamePrefix("hls_vpub");
        m_hlsManager->setEncodingProfile("ep_128k");
        m_hlsManager->addPid(PMT_PID, 1300);
        m_hlsManager->addPid(VIDEO_PID, 1301);
        m_hlsManager->addPid(AUDIO_PID, 1302);

        m_hlsManager->initialize();
    }
#endif
#ifdef USING_PIPE
    m_hlsManager->setAdapterNumber(adapter_no);
    m_hlsManager->setSegmentLength(15);
    m_hlsManager->setTempDirectory(m_outdir);
    m_hlsManager->setFilenamePrefix("hls_vpub");
    m_hlsManager->setEncodingProfile("ep_128k");
    m_hlsManager->setInputFilename(QString("/tmp/outputpipe%1.ts").arg(adapter_no));

    m_hlsManager->initialize();
#endif
}
