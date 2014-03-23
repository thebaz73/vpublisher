#include "vpublishingservice.h"

VPublishingService::VPublishingService(QObject *parent) :
    QObject(parent),
    m_lifecycle_manager(NULL)
{
}

VPublishingService::~VPublishingService()
{
}

void VPublishingService::setLifecycleManager(LifecycleManager *lifecycleManager)
{
    this->m_lifecycle_manager = lifecycleManager;
}

void VPublishingService::initialize()
{
    Q_ASSERT(m_lifecycle_manager);
}

void VPublishingService::updateStatus(qint64 timestamp)
{
    emit status(QString::number(timestamp));
}
