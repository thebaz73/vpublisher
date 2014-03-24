#include "dashmanager.h"

DashManager::DashManager(QObject *parent) :
    SegmentationManager(MPEG_DASH, parent)
{
}

DashManager::~DashManager()
{
}

unsigned int DashManager::currentSegmentIndex() const
{
    return 0;
}

void DashManager::doSegmentation()
{

}
