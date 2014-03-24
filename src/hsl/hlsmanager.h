#ifndef HLSMANAGER_H
#define HLSMANAGER_H

#include <QObject>

#include "../segmentationmanager.h"
#include "segmenter.h"

class HLSManager : public SegmentationManager
{
    Q_OBJECT
public:
    explicit HLSManager(QObject *parent = 0);
    ~HLSManager();

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

    virtual unsigned int currentSegmentIndex() const;
    virtual void doSegmentation();
private:
    Segmenter *m_segmenter;
    QString m_inputFilename;
    int m_segmentLength;
    QString m_tempDirectory;
    QString m_filenamePrefix;
    QString m_encodingProfile;
};

#endif // HLSMANAGER_H
