#ifndef DTVDEVICE_H
#define DTVDEVICE_H

#include <QObject>
#include <QMap>

#include "devicesettings.h"

class DTVDevice : public QObject
{
    Q_OBJECT
public:
    enum NodeType {
        NODE_TYPE_NONE = 0,
        NODE_TYPE_FRONTEND,
        NODE_TYPE_DVR,
        NODE_TYPE_DEMUX,
        NODE_TYPE_NET
    };
    explicit DTVDevice(QObject *parent = 0);
    ~DTVDevice();

    QString name() const;

    QString vendorId() const;
    void setVendorId(const QString &vendorId);

    QString productId() const;
    void setProductId(const QString &productId);

    QString manufacturer() const;
    void setManufacturer(const QString &manufacturer);

    QString product() const;
    void setProduct(const QString &product);

    QString serial() const;
    void setSerial(const QString &serial);

    QString version() const;
    void setVersion(const QString &version);

    QString busNumber() const;
    void setBusNumber(const QString &busNumber);

    QString devPath() const;
    void setDevPath(const QString &devPath);

    QString nodePath(NodeType type) const;
    void addNodePath(const QString &value);

    QString frontendNode();
    QString demuxNode();
    QString dvrNode();
    QString netNode();

    int adapterNumber() const;
    void setAdapterNumber(int adapterNumber);

signals:

public slots:
    int configure(DeviceSettings *settings);
private:
    QString m_vendorId;
    QString m_productId;
    QString m_manufacturer;
    QString m_product;
    QString m_serial;
    QString m_version;
    QString m_busNumber;
    QString m_devPath;
    QMap<NodeType, QString> m_nodePaths;
    int m_adapterNumber;

    void showInfo(int dev_fd);
};

#endif // DTVDEVICE_H
