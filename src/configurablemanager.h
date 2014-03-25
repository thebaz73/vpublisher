#ifndef CONFIGURABLEMANAGER_H
#define CONFIGURABLEMANAGER_H

#include <QObject>

class ConfigurableManager : public QObject
{
    Q_OBJECT
public:
    explicit ConfigurableManager(QObject *parent = 0);

public slots:
    virtual void onUpdateConfiguration() = 0;
};

#endif // CONFIGURABLEMANAGER_H
