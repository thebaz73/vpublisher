#ifndef LIFECYCLEMANAGER_H
#define LIFECYCLEMANAGER_H

#include <QObject>
#include <QTimer>
#include <QMutex>

#include "configurablemanager.h"
#include "dev/devicemanager.h"
#include "streamingworker.h"

class LifecycleManager : public ConfigurableManager
{
    Q_OBJECT
public:
    explicit LifecycleManager(QObject *parent = 0);
    ~LifecycleManager();

    void initialize();

    void setDeviceManager(DeviceManager *deviceManager);
signals:

public slots:
    void onUpdateConfiguration();
    void onExitStatus(WorkerExitStatus status);
    void elaborate();
private:
    DeviceManager *m_deviceManager;
    StreamingWorker *m_worker;
    QTimer m_timer;
};

#endif // LIFECYCLEMANAGER_H
