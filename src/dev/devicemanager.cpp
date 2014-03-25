#include "devicemanager.h"
#include <libudev.h>
#include <QMutexLocker>
#include <QDebug>
#include <linux/dvb/version.h>

#include "../configurationmanager.h"

#define INCIPIT "/dev/dvb/adapter"

DeviceManager::DeviceManager(QObject *parent) :
    ConfigurableManager(parent)
{
    if(DVB_API_VERSION == 5 && DVB_API_VERSION_MINOR == 4) {
        qDebug("Using DVB API v%d.%d", DVB_API_VERSION, DVB_API_VERSION_MINOR);
    }
    else {
        qFatal("Wrong DVB API. Current version: v%d.%d", DVB_API_VERSION, DVB_API_VERSION_MINOR);
    }
    onUpdateConfiguration();
}

DeviceManager::~DeviceManager()
{
}

void DeviceManager::initialize()
{
    qDebug() << "Initializing device manager...";
    connect(&m_dvb_device_timer, SIGNAL(timeout()), this, SLOT(analyzeSysFS()));
    m_dvb_device_timer.setSingleShot(true);
    m_dvb_device_timer.start();
//    connect(&m_v4l_device_timer, SIGNAL(timeout()), this, SLOT(analyzeCamera()));
//    m_v4l_device_timer.setSingleShot(true);
//    m_v4l_device_timer.start();
}
DeviceManager::Status DeviceManager::status() const
{
    return m_status;
}

QList<DTVDevice *> DeviceManager::devices() const
{
    return m_devices.values();
}

DTVDevice *DeviceManager::getDevice(int adapterNumber) const
{
    DTVDevice *device = NULL;
    if(m_devices.contains(adapterNumber)) {
        device = m_devices.value(adapterNumber);
    }

    return device;
}

void DeviceManager::onUpdateConfiguration()
{
    m_subsystem = ConfigurationManager::instance()->value("subsystem", "dvb").toString();
    m_system_bus = ConfigurationManager::instance()->value("system_bus", "usb").toString();
    m_device_type = ConfigurationManager::instance()->value("device_type", "usb_device").toString();
}

void DeviceManager::analyzeSysFS()
{
    QMutexLocker l(&m_device_mutex);
    struct udev *udev = udev_new();
    if(!udev) {
        m_status = PERMANENT_FAILURE;
        m_dvb_device_timer.start(1000);
        qCritical("Cannot allocate UDev system");
    }
    else {
        m_devices.clear();
        struct udev_enumerate *enumerate = udev_enumerate_new(udev);
        int ret = udev_enumerate_add_match_subsystem(enumerate, qPrintable(m_subsystem));
        ret += udev_enumerate_scan_devices(enumerate);
        if(!ret) {
            struct udev_list_entry *devices = udev_enumerate_get_list_entry(enumerate);

            struct udev_list_entry *dev_list_entry;
            udev_list_entry_foreach(dev_list_entry, devices) {
                const char *path = udev_list_entry_get_name(dev_list_entry);
                struct udev_device *dev = udev_device_new_from_syspath(udev, path);

                const char * ch_ptr = udev_device_get_devnode(dev);
                if(ch_ptr) {
                    QString node_path = QString::fromLatin1(ch_ptr);

                    dev = udev_device_get_parent_with_subsystem_devtype(dev, qPrintable(m_system_bus), qPrintable(m_device_type));
                    if (dev) {
                        m_status = DEVICE_FOUND;
                        QString vendor_id = QString::fromLatin1(udev_device_get_sysattr_value(dev,"idVendor"));
                        QString product_id = QString::fromLatin1(udev_device_get_sysattr_value(dev,"idProduct"));
                        QString manufacturer = QString::fromLatin1(udev_device_get_sysattr_value(dev,"manufacturer"));
                        QString product = QString::fromLatin1(udev_device_get_sysattr_value(dev,"product"));
                        QString serial = "";
                        const char *ch_ptr_serial = udev_device_get_sysattr_value(dev,"serial");
                        if(ch_ptr_serial) {
                            serial = QString::fromLatin1(ch_ptr_serial);
                        }
                        QString version = "";
                        const char *ch_ptr_version = udev_device_get_sysattr_value(dev,"version");
                        if(ch_ptr_version) {
                            version = QString::fromLatin1(ch_ptr_version);
                        }
                        QString bus_num = QString::fromLatin1(udev_device_get_sysattr_value(dev,"busnum"));
                        QString dev_path = QString::fromLatin1(udev_device_get_sysattr_value(dev,"devpath"));

                        addDevice(node_path, vendor_id, product_id, manufacturer, product, serial, version, bus_num, dev_path);
                    }
                }
                udev_device_unref(dev);
            }
        }
        /* Free the enumerator object */
        udev_enumerate_unref(enumerate);

        if(m_devices.size() == 0) {
            m_status = NO_DEVICE;
            m_dvb_device_timer.start(1000);
            qWarning("No device found");
        }
    }
    udev_unref(udev);
}

void DeviceManager::addDevice(QString node_path, QString vendor_id, QString product_id, QString manufacturer, QString product, QString serial, QString version, QString bus_num, QString dev_path)
{
    DTVDevice *device;
    int adapterNumber = createAdapterNumber(node_path);
    if(!m_devices.contains(adapterNumber)) {
        qDebug("New device found: %s - %s", qPrintable(manufacturer), qPrintable(product));
        device = new DTVDevice(this);
        m_devices.insert(adapterNumber, device);
        device->setAdapterNumber(adapterNumber);
        device->setVendorId(vendor_id);
        device->setProductId(product_id);
        device->setManufacturer(manufacturer);
        device->setProduct(product);
        device->setSerial(serial);
        device->setVersion(version);
        device->setBusNumber(bus_num);
        device->setDevPath(dev_path);
    }
    else {
        device = m_devices.value(adapterNumber);
    }

    device->addNodePath(node_path);
}


int DeviceManager::createAdapterNumber(QString node_path) const
{
    if(!node_path.contains(QString::fromLatin1(INCIPIT))) {
        return -1;
    }
    unsigned found = node_path.lastIndexOf("/\\");
    QString adapterString = QString::fromStdString(node_path.toStdString().substr(0, found).substr(QString::fromLatin1(INCIPIT).length()));
    return adapterString.toInt();
}
