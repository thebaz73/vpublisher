#ifndef LIFECYCLEMANAGER_H
#define LIFECYCLEMANAGER_H

#include <QObject>
#include <QTimer>
#include <QMap>
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
    void onExitStatus(int status);
    void elaborate();
    void start(int adapter_no, int service_id);
    void stop(int adapter_no);
private:
    DeviceManager *m_deviceManager;    
    QTimer m_timer;
    QMap<int, StreamingWorker*> m_worker_map;
};

#endif // LIFECYCLEMANAGER_H
