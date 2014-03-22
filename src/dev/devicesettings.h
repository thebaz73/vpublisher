#ifndef DEVICESETTINGS_H
#define DEVICESETTINGS_H

#include <QObject>
#include <QVariantMap>

class DeviceSettings : public QObject
{
    Q_OBJECT
public:
    explicit DeviceSettings(QObject *parent = 0);
    ~DeviceSettings();
    QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const;
    void setValue(const QString &key, const QVariant &value);
signals:
    void settingChanged(QString key);
private:
    QVariantMap m_settings;
};

#endif // DEVICESETTINGS_H
