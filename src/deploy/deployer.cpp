#include "deployer.h"

Deployer::Deployer(QObject *parent) :
    QObject(parent)
{
}

Deployer::~Deployer()
{
}

void Deployer::setSourceFolder(const QString &source_folder)
{
    m_source_folder = source_folder;
}
