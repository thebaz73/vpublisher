#ifndef DASHMANAGER_H
#define DASHMANAGER_H

#include "../segmentationmanager.h"

class DashManager : public SegmentationManager
{
    Q_OBJECT
public:
    explicit DashManager(QObject *parent = 0);
    ~DashManager();

    virtual unsigned int currentSegmentIndex() const;
    virtual void doSegmentation();
signals:

public slots:

};

#endif // DASHMANAGER_H
