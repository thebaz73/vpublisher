#ifndef TSADAEMON_H
#define TSADAEMON_H

#include <QCoreApplication>
#include "app/qtlocalpeer.h"

class QtLocalPeer;

class VPubDaemon : public QCoreApplication
{
    Q_OBJECT
public:
    explicit VPubDaemon(int &argc, char **argv);

    bool isRunning();
    QString id() const;
public slots:
    bool sendMessage(const QString &message, int timeout = 5000);

signals:
    void messageReceived(const QString &message);


private:
    void sysInit(const QString &appId = QString());
    QtLocalPeer *peer;
};

#endif // TSADAEMON_H
