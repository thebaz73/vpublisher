#include "lifecyclemanager.h"
#include <QDebug>
#include <QDir>

#include "configurationmanager.h"
#include "hls/hlsmanager.h"

#include "streamingworker.h"

LifecycleManager::LifecycleManager(QObject *parent) :
    ConfigurableManager(parent),
    m_deviceManager(NULL)
{
    onUpdateConfiguration();
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

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(elaborate()));
    m_timer.setSingleShot(true);
    m_timer.start(2000);
}

void LifecycleManager::setDeviceManager(DeviceManager *deviceManager)
{
    m_deviceManager = deviceManager;
}

void LifecycleManager::onUpdateConfiguration()
{
    //do nothing
}

void LifecycleManager::onExitStatus(WorkerExitStatus status)
{
    qDebug("Streaming worker exit status: %d", status);
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
    if(device->status() != DTVDevice::LOCKED) {
        qDebug("Settings applied on device: %s, but signal not locked.", qPrintable(device->name()));
    }
#endif
    m_worker = new StreamingWorker(this);
    connect(m_worker, SIGNAL(exitStatus(WorkerExitStatus)), this, SLOT(onExitStatus(WorkerExitStatus)));

    m_worker->setAdapterNumber(adapter_no);
#ifdef USING_DEVICE
    m_worker->setDevice(device);
#endif
    m_worker->initialize();
    m_worker->start();
}

