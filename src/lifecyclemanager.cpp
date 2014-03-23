#include "lifecyclemanager.h"
#include <QDebug>

LifecycleManager::LifecycleManager(QObject *parent) :
    QObject(parent)
{
}

LifecycleManager::~LifecycleManager()
{

}

void LifecycleManager::initialize()
{
    qDebug() << "Initializing lifecycle manager...";
    Q_ASSERT(m_deviceManager);
    Q_ASSERT(m_hlsManager);

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
    DeviceSettings settings;
    settings.setValue("delivery_system", 3);
    settings.setValue("frequency", 714000000);
    settings.setValue("bandwidth", 8000000);

    if(m_deviceManager->configureDevice(0, &settings) == 0) {
        qDebug() << "Settings applied";
    }
}


