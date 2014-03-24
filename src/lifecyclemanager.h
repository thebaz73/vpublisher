#ifndef LIFECYCLEMANAGER_H
#define LIFECYCLEMANAGER_H

#include <QObject>
#include <QTimer>
#include <QMutex>

#include "dev/devicemanager.h"
#include "segmentationmanager.h"

class LifecycleManager : public QObject
{
    Q_OBJECT
public:
    explicit LifecycleManager(QObject *parent = 0);
    ~LifecycleManager();

    void initialize();

    void setDeviceManager(DeviceManager *deviceManager);
signals:

public slots:
    void elaborate();
private:
    DeviceManager *m_deviceManager;
    SegmentationManager *m_segmentation_manager;
    QTimer m_timer;
    QString m_outdir;
};

#endif // LIFECYCLEMANAGER_H
