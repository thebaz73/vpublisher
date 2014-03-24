#ifndef SEGMENTATIONMANAGER_H
#define SEGMENTATIONMANAGER_H

#include <QObject>
#include <QMap>

#include "utils/common.h"
#include "dev/dtvdevice.h"

class SegmentationManager : public QObject
{
    Q_OBJECT
public:
    explicit SegmentationManager(QObject *parent = 0);

    int adapterNumber() const;
    void setAdapterNumber(int adapterNumber);

    void setDtvDevice(DTVDevice *dtvDevice);

    int exitCode() const;

    void addPid(PidType type, int pid);

    virtual unsigned int currentSegmentIndex() const = 0;
    virtual void doSegmentation() = 0;
signals:
    void finished();
public slots:
    void onSegmenterExitCode(int exitCode);
protected:
    int m_adapter_no;
    int m_exit_code;
    QMap<PidType, u_int16_t> pids;

    DTVDevice *m_dtv_device;
};

#endif // SEGMENTATIONMANAGER_H
