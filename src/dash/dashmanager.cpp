#include "dashmanager.h"
#include <QDir>

#include "../configurationmanager.h"


DashManager::DashManager(QObject *parent) :
    SegmentationManager(MPEG_DASH, parent)
{
}

DashManager::~DashManager()
{
}

void DashManager::doSegmentation()
{

}

unsigned int DashManager::currentSegmentIndex() const
{
    return 0;
}
