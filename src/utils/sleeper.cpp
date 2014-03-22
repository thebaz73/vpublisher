#include "sleeper.h"

Sleeper::Sleeper(QObject *parent) :
    QThread(parent)
{
}

void Sleeper::sleep(unsigned long seconds)
{
    QThread::sleep(seconds);
}

void Sleeper::msleep(unsigned long millis)
{
    QThread::msleep(millis);
}

void Sleeper::usleep(unsigned long useconds)
{
    QThread::usleep(useconds);
}
