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
protected:
    virtual unsigned int currentSegmentIndex() const;
signals:
public slots:
};

#endif // DASHMANAGER_H
