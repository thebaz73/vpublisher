#ifndef HLSMANAGER_H
#define HLSMANAGER_H

#include <QObject>

#include "segmenter.h"

class HLSManager : public QObject
{
    Q_OBJECT
public:
    explicit HLSManager(QObject *parent = 0);
    ~HLSManager();

    void initialize();
    Segmenter *segmenter() const;
    void setSegmenter(Segmenter *segmenter);

private:
    Segmenter *m_segmenter;
};

#endif // HLSMANAGER_H
