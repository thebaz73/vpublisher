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

void LifecycleManager::onExitStatus(int status)
{
    qDebug("Streaming worker exit status: %d", status);
}

void LifecycleManager::elaborate()
{
    start(0, 0);
}

void LifecycleManager::start(int adapter_no, int service_id)
{
#ifdef USING_DEVICE
    DeviceSettings settings;
    settings.setValue("delivery_system", 3);
    settings.setValue("frequency", 626000000);
    settings.setValue("bandwidth", 8000000);
    settings.setValue("timeout", 2000);
    settings.setValue("retries", 5);

    DTVDevice *device = m_deviceManager->getDevice(adapter_no);
    if(device->status() != DTVDevice::LOCKED && device->configure(&settings) == 0) {
        qDebug("Settings applied on device: %s", qPrintable(device->name()));
    }
    if(device->status() != DTVDevice::LOCKED) {
        qDebug("Settings applied on device: %s, but signal not locked.", qPrintable(device->name()));
    }
#endif
    StreamingWorker *worker = new StreamingWorker(this);
    connect(worker, SIGNAL(exitStatus(int)), this, SLOT(onExitStatus(int)));

    worker->setAdapterNumber(adapter_no);
#ifdef USING_DEVICE
    worker->setDevice(device);
#endif
    worker->initialize();
    worker->start();
    m_worker_map.insert(adapter_no, worker);
}

void LifecycleManager::stop(int adapter_no)
{
    if(m_worker_map.contains(adapter_no)) {
        StreamingWorker *worker = m_worker_map.value(adapter_no);
        worker->stopWorker();
    }
}

