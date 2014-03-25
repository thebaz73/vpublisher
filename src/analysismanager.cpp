#include "analysismanager.h"

AnalysisManager::AnalysisManager(QObject *parent) :
    ConfigurableManager(parent)
{
}

AnalysisManager::~AnalysisManager()
{
}

void AnalysisManager::initialize()
{
}

int AnalysisManager::adapterNumber() const
{
    return m_adapterNumber;
}

void AnalysisManager::setAdapterNumber(int adapterNumber)
{
    m_adapterNumber = adapterNumber;
}

DTVDevice *AnalysisManager::device() const
{
    return m_device;
}

void AnalysisManager::setDevice(DTVDevice *device)
{
    m_device = device;
}

void AnalysisManager::onUpdateConfiguration()
{
}
