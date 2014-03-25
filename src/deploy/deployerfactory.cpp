#include "deployerfactory.h"
#include "../configurationmanager.h"
#include "localdeployer.h"

Deployer *DeployerFactory::createDeployer()
{
    Deployer *deployer = NULL;
    if(ConfigurationManager::instance()->value("local_deploy", true).toBool()) {
        deployer = new LocalDeployer();
        deployer->initialize();
    }

    return deployer;
}

DeployerFactory::DeployerFactory(QObject *parent) :
    QObject(parent)
{
}
