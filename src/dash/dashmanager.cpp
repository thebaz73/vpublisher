#include "dashmanager.h"
#include <QDir>

#include "../configurationmanager.h"


DashManager::DashManager(QObject *parent) :
    SegmentationManager(MPEG_DASH, parent)
{
    onUpdateConfiguration();
}

DashManager::~DashManager()
{
}

void DashManager::startSegmentation()
{

}

void DashManager::stopSegmentation()
{

}

void DashManager::onUpdateConfiguration()
{

}
