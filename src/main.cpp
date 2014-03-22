#include <QCoreApplication>
#include <cstdio>
#include <cstdlib>
#include <QtDebug>
#include <QtGlobal>
#include <QDateTime>

#include "dev/devicemanager.h"
#include "hsl/hlsmanager.h"
#include "lifecyclemanager.h"

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

int main(int argc, char *argv[])
{
    qInstallMessageHandler(logger); //install : set the callback
    QCoreApplication a(argc, argv);

    DeviceManager deviceManager;
    deviceManager.initialize();

    HLSManager hlsManager;
    hlsManager.initialize();

    LifecycleManager lifecycleManager;
    lifecycleManager.setDeviceManager(&deviceManager);
    lifecycleManager.setHlsManager(&hlsManager);
    lifecycleManager.initialize();



    return a.exec();
}
