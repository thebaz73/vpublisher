#ifndef CONFIGURATIONMANAGER_H
#define CONFIGURATIONMANAGER_H

#include <QObject>
#include <QVariant>
#include <QSettings>
#include <QSharedPointer>

class ConfigurationManager : public QObject
{
    Q_OBJECT
public:
    static QSharedPointer<ConfigurationManager> instance();
    ~ConfigurationManager();
    void initialize();

    QVariant value(const QString &prefix, const QString &key, const QVariant &defaultValue) const;
    void setValue(const QString &prefix, const QString &key, const QVariant &value);
    void remove(const QString &prefix, const QString & key);
    QVariant value(const int adapterNo, const QString & key, const QVariant & defaultValue = QVariant()) const;
    void setValue(const int adapterNo, const QString & key, const QVariant & value);
    void remove(const int adapterNo, const QString & key);
    QVariant value(const QString & key, const QVariant & defaultValue = QVariant()) const;
    void setValue(const QString & key, const QVariant & value);
    void remove(const QString & key);

    bool sync();
    bool clear();
    bool resetDefault();
signals:
    
public slots:
private:
    ConfigurationManager(QObject *parent = 0);
    static QSharedPointer<ConfigurationManager> m_instance;
    void defaultValues();
    QString m_settings_file;
    QSettings *m_settings;
};

#endif // CONFIGURATIONMANAGER_H
