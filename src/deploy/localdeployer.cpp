#include "localdeployer.h"

#include <QDir>
#include <QDebug>

#include "../configurationmanager.h"

LocalDeployer::LocalDeployer(QObject *parent) :
    Deployer(parent),
    m_destination_folder("")
{
}

LocalDeployer::~LocalDeployer()
{
}

void LocalDeployer::initialize()
{
    m_destination_folder = ConfigurationManager::instance()->value("local", "destination_folder", "/tmp/www/hls").toString();
    QDir dir(m_destination_folder);
    if(!dir.exists()) {
        if(!dir.mkpath(m_destination_folder)) {
            qFatal("Cannot create destination folder: %s", qPrintable(m_destination_folder));
        }
    }
}

void LocalDeployer::deploy(QString playListName, QList<QString> fileNames, QString removedFileName)
{
    QDir source_dir(m_source_folder);
    QDir dest_dir(m_destination_folder);

    QFile::rename(source_dir.absoluteFilePath(playListName), dest_dir.absoluteFilePath(playListName));
    foreach (QString fileName, fileNames) {
        QFile::rename(source_dir.absoluteFilePath(fileName), dest_dir.absoluteFilePath(fileName));
    }

    if(!removedFileName.isEmpty()) {
        QFile file(dest_dir.absoluteFilePath(removedFileName));
        if(file.exists()) {
            if(!file.remove()) {
                qCritical("Cannot remove file: %s", qPrintable(removedFileName));
            }
        }
    }

    emit deployed();
}
