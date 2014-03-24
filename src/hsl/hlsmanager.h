#ifndef HLSMANAGER_H
#define HLSMANAGER_H

#include <QObject>
#include <QMap>

#include "segmenter.h"
#include "../dev/dtvdevice.h"

class HLSManager : public QObject
{
    Q_OBJECT
public:
    explicit HLSManager(QObject *parent = 0);
    ~HLSManager();

    void initialize();
    Segmenter *segmenter() const;
    void setSegmenter(Segmenter *segmenter);

    int adapterNumber() const;
    void setAdapterNumber(int adapterNumber);

    QString inputFilename() const;
    void setInputFilename(const QString &inputFilename);

    int segmentLength() const;
    void setSegmentLength(int segmentLength);

    QString tempDirectory() const;
    void setTempDirectory(const QString &tempDirectory);

    QString filenamePrefix() const;
    void setFilenamePrefix(const QString &filenamePrefix);

    QString encodingProfile() const;
    void setEncodingProfile(const QString &encodingProfile);

    DTVDevice *dtvDevice() const;
    void setDtvDevice(DTVDevice *dtvDevice);

    void addPid(PidType type, int pid);

private:
    Segmenter *m_segmenter;
    int m_adapter_no;
    QString m_inputFilename;
    int m_segmentLength;
    QString m_tempDirectory;
    QString m_filenamePrefix;
    QString m_encodingProfile;

    DTVDevice *m_dtv_device;
    QMap<PidType, u_int16_t> pids;
};

#endif // HLSMANAGER_H
