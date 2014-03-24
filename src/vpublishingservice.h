#ifndef VPUBLISHINGSERVICE_H
#define VPUBLISHINGSERVICE_H

#include <QObject>
#include "lifecyclemanager.h"

class VPublishingService : public QObject
{
    Q_OBJECT
public:
    explicit VPublishingService(QObject *parent = 0);
    ~VPublishingService();
    void setLifecycleManager(LifecycleManager *lifecycleManager);
    void initialize();
signals:
    void status(const QString &timestamp);
public slots:
    void updateStatus(qint64 timestamp);
private:
    LifecycleManager *m_lifecycle_manager;
};

#endif // VPUBLISHINGSERVICE_H
