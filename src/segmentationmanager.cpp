#include "segmentationmanager.h"

SegmentationManager::SegmentationManager(Type type, QObject *parent) :
    QObject(parent),
    m_type(type),
    m_adapter_no(0),
    m_dtv_device(NULL)
{
}

SegmentationManager::Type SegmentationManager::type() const
{
    return m_type;
}

int SegmentationManager::adapterNumber() const
{
    return m_adapter_no;
}

void SegmentationManager::setAdapterNumber(int adapterNumber)
{
    m_adapter_no = adapterNumber;
}

void SegmentationManager::setDtvDevice(DTVDevice *dtv_device)
{
    m_dtv_device = dtv_device;
}

int SegmentationManager::exitCode() const
{
    return m_exit_code;
}

void SegmentationManager::addPid(PidType type, int pid)
{
    pids.insert(type, pid);
}

void SegmentationManager::onSegmenterExitCode(int exitCode)
{
    m_exit_code = exitCode;
    emit finished();
}
