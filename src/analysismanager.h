#ifndef ANALYSISMANAGER_H
#define ANALYSISMANAGER_H

#include "configurablemanager.h"

#include "dev/dtvdevice.h"

class AnalysisManager : public ConfigurableManager
{
    Q_OBJECT
public:
    explicit AnalysisManager(QObject *parent = 0);
    ~AnalysisManager();

    void initialize();

    int adapterNumber() const;
    void setAdapterNumber(int adapterNumber);

    DTVDevice *device() const;
    void setDevice(DTVDevice *device);

signals:

public slots:
    void onUpdateConfiguration();
private:
    int m_adapterNumber;
    DTVDevice *m_device;
};

#endif // ANALYSISMANAGER_H
