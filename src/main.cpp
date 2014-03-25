#include <cstdio>
#include <cstdlib>
#include <csignal>

#include <QtDebug>
#include <QtGlobal>
#include <QDateTime>
#include <QNetworkProxy>
#include <QNetworkConfigurationManager>
#include <QNetworkSession>

#include "vpubdaemon.h"
#include "configurationmanager.h"
#include "dev/devicemanager.h"
#include "hls/hlsmanager.h"
#include "lifecyclemanager.h"

#include "vpublishingservice.h"
#include "vpublishingservice_interface.h"

#ifdef DBUS_USE_SESSION_BUS
    #define  DBUS_MESSAGE_BUS QDBusConnection::sessionBus
#else
    #define  DBUS_MESSAGE_BUS QDBusConnection::systemBus
#endif

#define LOG_LEVEL 0
//#define DETAILED_LOG

void logger(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
#ifndef DETAILED_LOG
    Q_UNUSED(context);
#endif
    QByteArray localMsg = msg.toLocal8Bit();
    QByteArray date = QDateTime::currentDateTime().toString(Qt::ISODate).toLocal8Bit();
    //in this function, you can write the message to any stream!
    switch (type) {
    case QtDebugMsg:
#if LOG_LEVEL < 1
#ifdef DETAILED_LOG
        fprintf(stderr, "%s - DEBUG: (%s:%u %s): %s\n", date.constData(), context.file, context.line, context.function, localMsg.constData());
#else
        fprintf(stderr, "%s - DEBUG: %s\n", date.constData(), localMsg.constData());
#endif
#endif
        break;
    case QtWarningMsg:
#if LOG_LEVEL < 2
#ifdef DETAILED_LOG
        fprintf(stderr, "%s - WARNING: (%s:%u %s): %s\n", date.constData(), context.file, context.line, context.function, localMsg.constData());
#else
        fprintf(stderr, "%s - WARNING: %s\n", date.constData(), localMsg.constData());
#endif
#endif
        break;
    case QtCriticalMsg:
#if LOG_LEVEL < 3
#ifdef DETAILED_LOG
        fprintf(stderr, "%s - CRITICAL: (%s:%u %s): %s\n", date.constData(), context.file, context.line, context.function, localMsg.constData());
#else
        fprintf(stderr, "%s - CRITICAL: %s\n", date.constData(), localMsg.constData());
#endif
#endif
        break;
    case QtFatalMsg:
#ifdef DETAILED_LOG
        fprintf(stderr, "%s - FATAL: (%s:%u %s): %s\n", date.constData(), context.file, context.line, context.function, localMsg.constData());
#else
        fprintf(stderr, "%s - FATAL: %s\n", date.constData(), localMsg.constData());
#endif
        abort();
    }
}

static QNetworkSession *g_networkSession = 0;

struct CleanExit {
    CleanExit() {
        signal(SIGINT, &CleanExit::exitQt);
        signal(SIGTERM, &CleanExit::exitQt);
        signal(SIGSEGV, &CleanExit::exitQt);
    }

    static void exitQt(int sig) {
        QCoreApplication::exit(sig);
    }
};

static void setupNetwork() {
    // Set Internet Access Point
    QNetworkConfigurationManager mgr;
    QList<QNetworkConfiguration> activeConfigs = mgr.allConfigurations();
    if (activeConfigs.count() <= 0)
        return;

    QNetworkConfiguration cfg = activeConfigs.at(0);
    foreach(QNetworkConfiguration config, activeConfigs) {
        if (config.type() == QNetworkConfiguration::UserChoice) {
            cfg = config;
            break;
        }
    }

    g_networkSession = new QNetworkSession(cfg);
    g_networkSession->open();
    g_networkSession->waitForOpened(-1);
}

int main(int argc, char *argv[])
{
    CleanExit exit;
    Q_UNUSED(exit);

    qInstallMessageHandler(logger); //install : set the callback
#ifdef USING_DEVICE
#ifdef USING_PIPE
    qFatal("Cannot run with both DEVICE and PIPE profile");
#endif
#endif

    VPubDaemon app(argc, argv);
    app.setApplicationName("Video publisher");
    app.setOrganizationName("Marco Bazzoni");
    app.setOrganizationDomain("thebaz.it");

    if (app.arguments().contains("--help") || app.arguments().contains("-h")) {
        printf("%s Usage: vpubd\n", qPrintable(app.applicationName()));

        return 0;
    }
    else {
        setupNetwork();
        bool primarySession = !app.isRunning();
        if (!primarySession) {
            qWarning() << app.applicationName() << "is already running, aborting";
            return false;
        }

        ConfigurationManager::instance()->initialize();

#ifdef USING_DEVICE
        DeviceManager deviceManager;
        deviceManager.initialize();
#endif

        LifecycleManager lifecycleManager;
#ifdef USING_DEVICE
        lifecycleManager.setDeviceManager(&deviceManager);
#endif
        lifecycleManager.initialize();

        VPublishingService *service = new VPublishingService();
        service->setLifecycleManager(&lifecycleManager);
        service->initialize();

        new ServiceAdaptor(service);
        QDBusConnection connection = DBUS_MESSAGE_BUS();
        connection.registerObject("/vpub/daemon", service);
        connection.registerService("vpub.daemon.dbus.service");

        int ret = app.exec();

        g_networkSession->close();
        return ret;
    }
}
