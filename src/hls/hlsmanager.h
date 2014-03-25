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

    virtual void doSegmentation();
protected:

private:
    Segmenter *m_segmenter;
};

#endif // HLSMANAGER_H
