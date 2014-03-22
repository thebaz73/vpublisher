#include "segmenter.h"

Segmenter::Segmenter(QObject *parent) :
    QObject(parent),
    m_running(false),
    m_suspend(false)

{
}

Segmenter::~Segmenter()
{
}

void Segmenter::process()
{
    m_running = true;

    while (m_running) {

        if(m_suspend) {
            m_suspend_condition.wait(&m_suspend_mutex);
        }
    }
}

void Segmenter::suspend()
{
    this->m_suspend = true;
}

void Segmenter::resume()
{
    this->m_suspend = false;
    m_suspend_condition.wakeAll();
}

void Segmenter::stop()
{
    if(m_suspend) {
        resume();
    }
    m_running = false;
    emit finished();
}
