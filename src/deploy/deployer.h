#ifndef DEPLOYER_H
#define DEPLOYER_H

#include <QObject>

class Deployer : public QObject
{
    Q_OBJECT
public:
    explicit Deployer(QObject *parent = 0);
    ~Deployer();

    void setSourceFolder(const QString &source_folder);

    virtual void initialize() = 0;
    virtual void deploy(QString playListName, QList<QString> fileNames, QString removedFileName) = 0;
signals:
    void deployed();
protected:
    QString m_source_folder;
};

#endif // DEPLOYER_H
