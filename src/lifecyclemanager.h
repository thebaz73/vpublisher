#ifndef LIFECYCLEMANAGER_H
#define LIFECYCLEMANAGER_H

#include <QObject>
#include <QTimer>
#include <QMutex>

#include "dev/devicemanager.h"
#include "hsl/hlsmanager.h"

class LifecycleManager : public QObject
{
    Q_OBJECT
public:
    explicit LifecycleManager(QObject *parent = 0);
    ~LifecycleManager();

    void initialize();

    void setDeviceManager(DeviceManager *deviceManager);
    void setHlsManager(HLSManager *hlsManager);

signals:

public slots:
    void elaborate();
private:
    DeviceManager *m_deviceManager;
    HLSManager *m_hlsManager;
    QTimer m_timer;
};

#endif // LIFECYCLEMANAGER_H
