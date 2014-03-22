#ifndef SLEEPER_H
#define SLEEPER_H

#include <QThread>

class Sleeper : public QThread
{
    Q_OBJECT
public:
    explicit Sleeper(QObject *parent = 0);
    static void sleep(unsigned long seconds);
    static void msleep(unsigned long millis);
    static void usleep(unsigned long useconds);
};

#endif // SLEEPER_H
