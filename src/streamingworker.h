#ifndef STREAMINGWORKER_H
#define STREAMINGWORKER_H

#include <QThread>

#include "dev/devicemanager.h"
#include "dev/dtvdevice.h"
#include "segmentationmanager.h"
#include "playlistmanager.h"
#include "deploymentmanager.h"

class StreamingWorker : public QThread
{
    Q_OBJECT
public:    
    explicit StreamingWorker(QObject *parent = 0);
    ~StreamingWorker();

    void initialize();
    int adapterNumber() const;
    void setAdapterNumber(int adapterNumber);

    DTVDevice *device() const;
    void setDevice(DTVDevice *device);

signals:
    void exitStatus(WorkerExitStatus status);

public slots:
    void run();
    void processSegmentationFinished();
private:
    int m_adapterNumber;
    DTVDevice *m_device;
    SegmentationManager *m_segmentation_manager;
    PlaylistManager *m_playlist_manager;
    DeploymentManager *m_deployment_manager;
};

#endif // STREAMINGWORKER_H
