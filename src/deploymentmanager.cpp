#include "deploymentmanager.h"

#include "deploy/deployerfactory.h"

DeploymentManager::DeploymentManager(QObject *parent) :
    ConfigurableManager(parent)
{
    onUpdateConfiguration();
}

DeploymentManager::~DeploymentManager()
{
}

void DeploymentManager::setSourceFolder(const QString &source_folder)
{
    m_source_folder = source_folder;
}

void DeploymentManager::onUpdateConfiguration()
{

}

void DeploymentManager::deploy(QString playListName, QList<QString> fileNames, QString removedFileName)
{
    m_deployer = DeployerFactory::createDeployer();
    m_deployer->setSourceFolder(m_source_folder);
    m_deployer->deploy(playListName, fileNames, removedFileName);
    delete m_deployer;
}
