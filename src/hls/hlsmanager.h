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

    virtual void startSegmentation();
    virtual void stopSegmentation();
public slots:
    void onUpdateConfiguration();
protected:

private:
    Segmenter *m_segmenter;
};

#endif // HLSMANAGER_H
