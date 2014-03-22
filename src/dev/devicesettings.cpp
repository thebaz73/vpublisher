#include "devicesettings.h"

DeviceSettings::DeviceSettings(QObject *parent) :
    QObject(parent)
{
}

DeviceSettings::~DeviceSettings()
{
}

QVariant DeviceSettings::value(const QString &key, const QVariant &defaultValue) const
{
    if(!m_settings.contains(key)) {
        return defaultValue;
    }

    return m_settings.value(key, defaultValue);
}

void DeviceSettings::setValue(const QString &key, const QVariant &value)
{
    m_settings.insert(key ,value);
    emit settingChanged(key);
}
