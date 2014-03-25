#ifndef DEPLOYERFACTORY_H
#define DEPLOYERFACTORY_H

#include <QObject>

#include "deployer.h"

class DeployerFactory : public QObject
{
    Q_OBJECT
public:
    static Deployer *createDeployer();
private:
    explicit DeployerFactory(QObject *parent = 0);
};

#endif // DEPLOYERFACTORY_H
