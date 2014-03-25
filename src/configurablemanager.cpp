#include "configurablemanager.h"
#include "configurationmanager.h"

ConfigurableManager::ConfigurableManager(QObject *parent) :
    QObject(parent)
{
    connect(ConfigurationManager::instance().data(), SIGNAL(updateConfiguration()), this, SLOT(onUpdateConfiguration()));
}
