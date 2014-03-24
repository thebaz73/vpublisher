#ifndef SEGMENTATIONMANAGER_H
#define SEGMENTATIONMANAGER_H

#include <QObject>

class SegmentationManager : public QObject
{
    Q_OBJECT
public:
    explicit SegmentationManager(QObject *parent = 0);

    int exitCode() const;

    virtual void doSegmentation() = 0;
signals:
    void finished();
public slots:
    virtual void onSegmenterExitCode(int exitCode) = 0;
protected:
    int m_exit_code;
};

#endif // SEGMENTATIONMANAGER_H
