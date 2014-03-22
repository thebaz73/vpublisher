#include "hlsmanager.h"
#include <QThread>
#include <QDebug>

HLSManager::HLSManager(QObject *parent) :
    QObject(parent),
    m_segmenter(NULL)
{
}

HLSManager::~HLSManager()
{
}

void HLSManager::initialize()
{
    qDebug() << "Initializing HSL manager...";
    QThread* thread = new QThread(this);
    m_segmenter = new Segmenter();

    m_segmenter->moveToThread(thread);
    connect(thread, SIGNAL(started()), m_segmenter, SLOT(process()));
    connect(m_segmenter, SIGNAL(finished()), thread, SLOT(quit()));
    connect(m_segmenter, SIGNAL(finished()), m_segmenter, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}
Segmenter *HLSManager::segmenter() const
{
    return m_segmenter;
}

void HLSManager::setSegmenter(Segmenter *segmenter)
{
    m_segmenter = segmenter;
}

