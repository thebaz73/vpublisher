#ifndef DEPLOYMENTMANAGER_H
#define DEPLOYMENTMANAGER_H

#include "configurablemanager.h"
#include "deploy/deployer.h"

class DeploymentManager : public ConfigurableManager
{
    Q_OBJECT
public:
    explicit DeploymentManager(QObject *parent = 0);
    ~DeploymentManager();

    void setSourceFolder(const QString &source_folder);

signals:

public slots:
    void onUpdateConfiguration();
    void deploy(QString playListName, QList<QString> fileNames, QString removedFileName);
private:
    QString m_source_folder;
    Deployer *m_deployer;
};

#endif // DEPLOYMENTMANAGER_H
