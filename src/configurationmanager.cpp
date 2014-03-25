#include "configurationmanager.h"
#include <QDebug>
#include <QFile>
#include <QDir>

QSharedPointer<ConfigurationManager> ConfigurationManager::m_instance(new ConfigurationManager());

QSharedPointer<ConfigurationManager> ConfigurationManager::instance()
{
    return m_instance;
}

ConfigurationManager::ConfigurationManager(QObject *parent) :
    QObject(parent),
    m_settings_file("/var/lib/vpubd/vpubd.ini")
{
    QSettings::setDefaultFormat(QSettings::IniFormat);
    m_settings = new QSettings(m_settings_file, QSettings::IniFormat);
}

ConfigurationManager::~ConfigurationManager()
{
    delete m_settings;
}

void ConfigurationManager::initialize()
{
    qDebug() << "Initializing configuration manager...";
    if(!m_settings->value("initialized", false).toBool()) {
        defaultValues();
    }
    bool status = true;
    status = status && sync();
    if(!status) {
        qFatal("Cannot sync settings, cannot initialize application.");
    }
}

QVariant ConfigurationManager::value(const QString &prefix, const QString &key, const QVariant &defaultValue) const
{
    m_settings->beginGroup(prefix);
    QVariant value = m_settings->value(key, defaultValue);
    m_settings->endGroup();

    return value;
}

void ConfigurationManager::setValue(const QString &prefix, const QString &key, const QVariant &value)
{
    m_settings->beginGroup(prefix);
    m_settings->setValue(key, value);
    m_settings->endGroup();
}

void ConfigurationManager::remove(const QString &prefix, const QString &key)
{
    m_settings->beginGroup(prefix);
    m_settings->remove(key);
    m_settings->endGroup();
}

QVariant ConfigurationManager::value(const int adapterNo, const QString &key, const QVariant &defaultValue) const
{
    const QString prefix = "adapter_" + QString::number(adapterNo);
    m_settings->beginGroup(prefix);
    QVariant value = m_settings->value(key, defaultValue);
    m_settings->endGroup();

    return value;
}

void ConfigurationManager::setValue(const int adapterNo, const QString &key, const QVariant &value)
{
    const QString prefix = "adapter_" + QString::number(adapterNo);
    m_settings->beginGroup(prefix);
    m_settings->setValue(key, value);
    m_settings->endGroup();
}

void ConfigurationManager::remove(const int adapterNo, const QString &key)
{
    const QString prefix = "adapter_" + QString::number(adapterNo);
    m_settings->beginGroup(prefix);
    m_settings->remove(key);
    m_settings->endGroup();
}

QVariant ConfigurationManager::value(const QString &key, const QVariant &defaultValue) const
{
    QVariant value = m_settings->value(key, defaultValue);

    return value;
}

void ConfigurationManager::setValue(const QString &key, const QVariant &value)
{
    m_settings->setValue(key, value);
}

void ConfigurationManager::remove(const QString &key)
{
    m_settings->remove(key);
}

bool ConfigurationManager::sync()
{
    m_settings->sync();

    bool retValue = m_settings->status() == QSettings::NoError;
    if(retValue) {
        emit updateConfiguration();
    }

    return retValue;
}

bool ConfigurationManager::clear()
{
    m_settings->clear();

    return m_settings->status() == QSettings::NoError;
}

bool ConfigurationManager::resetDefault()
{
    defaultValues();
    return sync();
}

void ConfigurationManager::defaultValues()
{
    //General
    setValue("initialized", true);

    setValue("local_deploy", true);
    setValue("scp_deploy", false);
    setValue("webdav_deploy", false);
    setValue("rpc_deploy", false);

    setValue("hls_streaming", true);
    setValue("dash_streaming", false);
    setValue("max_filesize", 5);

    //Device
    setValue("device", "subsystem", "dvb");
    setValue("device", "system_bus", "usb");
    setValue("device", "device_type", "usb_device");

    //LOCAL DEPLOY
    setValue("local", "destination_folder", "/tmp/www/hls");

    //HSL
    setValue("hls","tmp_outdir", QDir::tempPath() + "/hls");
    setValue("hls","segment_length",15);
    setValue("hls","filename_prefix","hls_vpub");
    setValue("hls","encoding_profile","128K");
    setValue("hls","input_filename", "../vpubd/etc/test%1.ts");
    setValue("hls", "playlist_name", "hls_vpub.m3u8");
    setValue("hls", "segment_url", "http://localhost:8080/vpub");

    //DASH
    setValue("dash","tmp_outdir", QDir::tempPath() + "/dash");
    setValue("dash","segment_length",15);
    setValue("dash","filename_prefix","dash_vpub");
    setValue("dash","encoding_profile","128K");
}
