#ifndef SEGMENTER_H
#define SEGMENTER_H

#include <QObject>
#include <QMutex>
#include <QWaitCondition>

class Segmenter : public QObject
{
    Q_OBJECT
public:
    explicit Segmenter(QObject *parent = 0);
    ~Segmenter();

signals:
    void finished();

public slots:
    void process();
    void suspend();
    void resume();
    void stop();
private:
    bool m_running;
    bool m_suspend;
    QWaitCondition m_suspend_condition;
    QMutex m_suspend_mutex;
};

#endif // SEGMENTER_H
