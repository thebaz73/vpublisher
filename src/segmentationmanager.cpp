#include "segmentationmanager.h"

SegmentationManager::SegmentationManager(QObject *parent) :
    QObject(parent),
    m_adapter_no(0),
    m_dtv_device(NULL)
{
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
