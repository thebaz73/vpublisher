#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <QObject>
#include <QMap>
#include <QTimer>
#include <QMutex>

#include "../configurablemanager.h"
#include "dtvdevice.h"
#include "devicesettings.h"

class DeviceManager : public ConfigurableManager
{
    Q_OBJECT
public:
    enum Status {PERMANENT_FAILURE = 0, NO_DEVICE, DEVICE_FOUND};
    explicit DeviceManager(QObject *parent = 0);
    ~DeviceManager();

    void initialize();

    Status status() const;
    QList<DTVDevice *> devices() const;
    DTVDevice *getDevice(int adapterNumber) const;
signals:

private slots:
    void onUpdateConfiguration();
    void analyzeSysFS();
//    void analyzeCamera();
private:
    QString m_subsystem;
    QString m_system_bus;
    QString m_device_type;
    Status m_status;
    QTimer m_dvb_device_timer;
    QMap<int, DTVDevice *> m_devices;
    QMutex m_device_mutex;
//    QTimer m_v4l_device_timer;

    void addDevice(QString node_path, QString vendor_id, QString product_id, QString manufacturer, QString product, QString serial, QString version, QString bus_num, QString dev_path);
    int createAdapterNumber(QString node_path) const;
};

#endif // DEVICEMANAGER_H
