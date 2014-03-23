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
    /**
     * Open a demux device. Can be called multiple times. These let you setup a
     * single filter per FD. It can can also be read() from if you use a section
     * filter, or create a pes_filter or raw_filter with output DVBDEMUX_OUTPUT_DEMUX.
     *
     * @param nonblocking If true, frontend will be opened in nonblocking mode.
     * @return A unix file descriptor on success, or -1 on failure.
     */
    int openDvbdemux(bool nonblocking);
    /**
     * Close a demux device
     *
     * @param demux_fd FD as opened with openDvbdemux() above.
     * @return 0 on success, nonzero on failure.
     */
    int closeDvbdemux(int demux_fd);
    /**
     * Create a pid filter - this will extract transport stream packets for a
     * specified PID.
     *
     * @param demux_fd FD as opened with dvbdemux_open_demux() above.
     * @param pid PID to retrieve, or use -1 as a wildcard for ALL PIDs.
     * @param start If true, the filter will be started immediately. Otherwise you must
     * call startDvbdemux() manually.
     * @return 0 on success, nonzero on failure.
     */
    int addPidFilter(int demux_fd, u_int16_t pid, bool start);
    /**
     * Start a demux going.
     *
     * @param demux_fd FD as opened with openDvbdemux() above.
     * @return 0 on success, nonzero on failure.
     */
    int startDvbdemux(int demux_fd);
    /**
     * Stop a demux.
     *
     * @param demux_fd FD as opened with openDvbdemux() above.
     * @return 0 on success, nonzero on failure.
     */
    int stopDvbdemux(int demux_fd);

    /**
     * Open a DVR device. May be opened for writing or reading once.
     * It is used to either write() transport stream data to be demuxed,
     * or to read() a stream of demuxed data
     *
     * Note, all demux filters will be multiplexed together and output
     * their data on this device.
     *
     * @param readonly If true, frontend will be opened in readonly mode only.
     * @param nonblocking If true, frontend will be opened in nonblocking mode.
     * @return A unix file descriptor on success, or -1 on failure.
     */
    int openDvbdvr(bool readonly, bool nonblocking);
    /**
     * Close a dvr device
     *
     * @param dvr_fd FD as opened with openDvbdvr() above.
     * @return 0 on success, nonzero on failure.
     */
    int closeDvbdvr(int dvr_fd);
    /**
     * Reads data into buffer given a unix file descriptor
     *
     * @param fd FD as opened with openDvbdemux() or openDvbdvr() above.
     * @param data pointer to read data buffer
     * @param length length data buffer
     * @return number of bytes read.
     */
    int readData(int fd, u_int8_t *data, size_t length);
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
