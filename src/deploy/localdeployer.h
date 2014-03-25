#ifndef LOCALDEPLOYER_H
#define LOCALDEPLOYER_H

#include "deployer.h"

class LocalDeployer : public Deployer
{
    Q_OBJECT
public:
    explicit LocalDeployer(QObject *parent = 0);
    ~LocalDeployer();

    void initialize();
    void deploy(QString playListName, QList<QString> fileNames, QString removedFileName);

signals:

public slots:
private:
    QString m_destination_folder;

};

#endif // LOCALDEPLOYER_H
