#ifndef SEGMENTATIONMANAGER_H
#define SEGMENTATIONMANAGER_H

#include <QObject>
#include <QMap>

#include "configurablemanager.h"
#include "utils/common.h"
#include "dev/dtvdevice.h"

class SegmentationManager : public ConfigurableManager
{
    Q_OBJECT
public:
    enum Type {
        APPLE_HLS = 0,
        MPEG_DASH
    };

    explicit SegmentationManager(Type type, QObject *parent = 0);

    Type type() const;

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

    void setDtvDevice(DTVDevice *dtvDevice);

    QString outdir() const;

    int exitCode() const;

    void addPid(PidType type, int pid);

    virtual void startSegmentation() = 0;
    virtual void stopSegmentation() = 0;
signals:
    void finished();
    void segmentChanged(QString fileName, double segmentLength);
public slots:
    virtual void onUpdateConfiguration() = 0;
    void onSegmenterExitCode(int exitCode);
    void onSegmentIndexChanged(int segmentIndex, double segmentLength);
protected:
    Type m_type;
    int m_adapter_no;
    QString m_inputFilename;
    int m_segmentLength;
    QString m_tempDirectory;
    QString m_filenamePrefix;
    QString m_encodingProfile;
    QString m_outdir;
    int m_exit_code;
    QMap<PidType, u_int16_t> pids;

    DTVDevice *m_dtv_device;
};

#endif // SEGMENTATIONMANAGER_H
