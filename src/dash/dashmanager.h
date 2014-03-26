#ifndef DASHMANAGER_H
#define DASHMANAGER_H

#include "../segmentationmanager.h"

class DashManager : public SegmentationManager
{
    Q_OBJECT
public:
    explicit DashManager(QObject *parent = 0);
    ~DashManager();

    virtual void startSegmentation();
    virtual void stopSegmentation();
public slots:
    virtual void onUpdateConfiguration();
};

#endif // DASHMANAGER_H
