#include "dtvdevice.h"
#include <QDebug>

#include <fcntl.h>
#include <errno.h>

#include <linux/dvb/frontend.h>
#include <linux/dvb/dmx.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#include "../utils/sleeper.h"
#include "../utils/common.h"

DTVDevice::DTVDevice(QObject *parent) :
    QObject(parent)
{
}

DTVDevice::~DTVDevice()
{
}

QString DTVDevice::name() const
{
    return QString("%1 %2").arg(m_manufacturer).arg(m_product);
}

QString DTVDevice::vendorId() const
{
    return m_vendorId;
}

void DTVDevice::setVendorId(const QString &vendorId)
{
    m_vendorId = vendorId;
}
QString DTVDevice::productId() const
{
    return m_productId;
}

void DTVDevice::setProductId(const QString &productId)
{
    m_productId = productId;
}
QString DTVDevice::manufacturer() const
{
    return m_manufacturer;
}

void DTVDevice::setManufacturer(const QString &manufacturer)
{
    m_manufacturer = manufacturer;
}
QString DTVDevice::product() const
{
    return m_product;
}

void DTVDevice::setProduct(const QString &product)
{
    m_product = product;
}
QString DTVDevice::serial() const
{
    return m_serial;
}

void DTVDevice::setSerial(const QString &serial)
{
    m_serial = serial;
}
QString DTVDevice::version() const
{
    return m_version;
}

void DTVDevice::setVersion(const QString &version)
{
    m_version = version;
}
QString DTVDevice::busNumber() const
{
    return m_busNumber;
}

void DTVDevice::setBusNumber(const QString &busNumber)
{
    m_busNumber = busNumber;
}
QString DTVDevice::devPath() const
{
    return m_devPath;
}

void DTVDevice::setDevPath(const QString &devPath)
{
    m_devPath = devPath;
}

QString DTVDevice::nodePath(NodeType type) const
{
    return m_nodePaths.value(type);
}

void DTVDevice::addNodePath(const QString &value)
{
    if(value.contains("frontend")) {
        m_nodePaths.insert(NODE_TYPE_FRONTEND, value);
    }
    else if(value.contains("dvr")) {
        m_nodePaths.insert(NODE_TYPE_DVR, value);
    }
    else if(value.contains("demux")) {
        m_nodePaths.insert(NODE_TYPE_DEMUX, value);
    }
    else if(value.contains("net")) {
        m_nodePaths.insert(NODE_TYPE_NET, value);
    }
}

QString DTVDevice::frontendNode()
{
    return nodePath(NODE_TYPE_FRONTEND);
}

QString DTVDevice::demuxNode()
{
    return nodePath(NODE_TYPE_DEMUX);
}

QString DTVDevice::dvrNode()
{
    return nodePath(NODE_TYPE_DVR);
}

QString DTVDevice::netNode()
{
    return nodePath(NODE_TYPE_NET);
}
int DTVDevice::adapterNumber() const
{
    return m_adapterNumber;
}

void DTVDevice::setAdapterNumber(int adapterNumber)
{
    m_adapterNumber = adapterNumber;
}

int DTVDevice::configure(DeviceSettings *settings)
{
    qDebug("Applying configuration to device: %s", qPrintable(name()));
    int retCode = 0;
    int dev_fd = ::open(qPrintable(frontendNode()), O_WRONLY);
    if(dev_fd < 0) {
        qWarning("Error opening frontend %s: %d", qPrintable(frontendNode()), errno);
        retCode = -1;
    }
    else {
        fe_status_t status = FE_REINIT;

        struct dtv_properties dtvps;
        int delivery_system = settings->value("delivery_system").toInt();

        if(delivery_system != 16 && delivery_system != 3) {
            qWarning("Wrong delivery system for device %s: %d", qPrintable(frontendNode()), delivery_system);
            retCode = -2;
        }
        else {
            if(delivery_system == 16) {
                struct dtv_property dtvp[6];
                dtvp[0].cmd = DTV_CLEAR;
                dtvp[0].u.data=1;
                dtvp[1].cmd = DTV_FREQUENCY;
                dtvp[1].u.data=settings->value("frequency").toInt();
                dtvp[2].cmd = DTV_BANDWIDTH_HZ;
                dtvp[2].u.data=settings->value("bandwidth").toInt();
                dtvp[3].cmd = DTV_DELIVERY_SYSTEM;
                dtvp[3].u.data=delivery_system;
                dtvp[4].cmd = DTV_DVBT2_PLP_ID;
                dtvp[4].u.data=settings->value("plp_id").toInt();
                dtvp[5].cmd = DTV_TUNE;
                dtvp[5].u.data=1;
                dtvps.num = 6;

                dtvps.props = dtvp;
            }
            else if(delivery_system == 3) {
                struct dtv_property dtvp[5];
                dtvp[0].cmd = DTV_CLEAR;
                dtvp[0].u.data=1;
                dtvp[1].cmd = DTV_FREQUENCY;
                dtvp[1].u.data=settings->value("frequency").toInt();
                dtvp[2].cmd = DTV_BANDWIDTH_HZ;
                dtvp[2].u.data=settings->value("bandwidth").toInt();
                dtvp[3].cmd = DTV_DELIVERY_SYSTEM;
                dtvp[3].u.data=delivery_system;
                dtvp[4].cmd = DTV_TUNE;
                dtvp[4].u.data=1;
                dtvps.num = 5;

                dtvps.props = dtvp;
            }
            if (ioctl(dev_fd, FE_SET_PROPERTY, &dtvps) < 0) {
                qWarning("Error executing ioctl on %s: %d", qPrintable(frontendNode()), errno);
                retCode = -3;
            }
            for (int i = 0; i < 10; i++) {
                Sleeper::msleep(100);
                if(ioctl(dev_fd, FE_READ_STATUS, &status) >= 0) {
                    if(status == 31) {
                        qDebug("Device: %s signal locked (code: %d).", qPrintable(name()), status);
                        break;
                    }
                }
            }
            if(status != 31) {
                qWarning("Device: %s signal not locked (code: %d).", qPrintable(name()), status);
                retCode = -FE_HAS_LOCK;
            }
            showInfo(dev_fd);
        }
        close(dev_fd);
    }

    return retCode;
}

void DTVDevice::showInfo(int dev_fd)
{
    dvb_frontend_info* info = (dvb_frontend_info*)malloc(sizeof(dvb_frontend_info));
    if(ioctl(dev_fd, FE_GET_INFO, info) >= 0) {
        qDebug() << "Device informations";
        qDebug() << "============================================================";
        qDebug() << "Name: " << info->name;
        qDebug() << "Type: " << info->type;
        qDebug() << "Frequency: [ min:" << info->frequency_min/1000000 << "Mhz, max:"<< info->frequency_max/1000000 << "Mhz ], stepsize:" << info->frequency_stepsize << "- tolerance:" << info->frequency_tolerance;
        qDebug() << "Symbol rate [ min:" << info->symbol_rate_min << ", max: " << info->symbol_rate_max<<", tolerance:" << info->symbol_rate_tolerance << "]";
        QList<QString> caps = capabilities(info->caps);
        qDebug() << "Capabilities: ";
        foreach (QString capability, caps) {
            qDebug() << "- " << capability;
        }
    }
    free(info);
}










