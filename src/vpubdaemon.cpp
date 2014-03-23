#include "vpubdaemon.h"

VPubDaemon::VPubDaemon(int &argc, char **argv) :
    QCoreApplication(argc, argv)
{
    sysInit();
}

/*!
    Returns true if another instance of this application is running;
    otherwise false.

    This function does not find instances of this application that are
    being run by a different user (on Windows: that are running in
    another session).

    \sa sendMessage()
*/

bool VPubDaemon::isRunning()
{
    return peer->isClient();
}


/*!
    Tries to send the text \a message to the currently running
    instance. The VPubDaemon object in the running instance
    will emit the messageReceived() signal when it receives the
    message.

    This function returns true if the message has been sent to, and
    processed by, the current instance. If there is no instance
    currently running, or if the running instance fails to process the
    message within \a timeout milliseconds, this function return false.

    \sa isRunning(), messageReceived()
*/
bool VPubDaemon::sendMessage(const QString &message, int timeout)
{
    return peer->sendMessage(message, timeout);
}


/*!
    Returns the application identifier. Two processes with the same
    identifier will be regarded as instances of the same application.
*/
QString VPubDaemon::id() const
{
    return peer->applicationId();
}


/*!
    \class VPubDaemon qtsingleapplication.h
    \brief The VPubDaemon class provides an API to detect and
    communicate with running instances of an application.

    This class allows you to create applications where only one
    instance should be running at a time. I.e., if the user tries to
    launch another instance, the already running instance will be
    activated instead. Another usecase is a client-server system,
    where the first started instance will assume the role of server,
    and the later instances will act as clients of that server.

    By default, the full path of the executable file is used to
    determine whether two processes are instances of the same
    application. You can also provide an explicit identifier string
    that will be compared instead.

    The application should create the VPubDaemon object early
    in the startup phase, and call isRunning() to find out if another
    instance of this application is already running. If isRunning()
    returns false, it means that no other instance is running, and
    this instance has assumed the role as the running instance. In
    this case, the application should continue with the initialization
    of the application user interface before entering the event loop
    with exec(), as normal.

    The messageReceived() signal will be emitted when the running
    application receives messages from another instance of the same
    application. When a message is received it might be helpful to the
    user to raise the application so that it becomes visible. To
    facilitate this, VPubDaemon provides the
    setActivationWindow() function and the activateWindow() slot.

    If isRunning() returns true, another instance is already
    running. It may be alerted to the fact that another instance has
    started by using the sendMessage() function. Also data such as
    startup parameters (e.g. the name of the file the user wanted this
    new instance to open) can be passed to the running instance with
    this function. Then, the application should terminate (or enter
    client mode).

    If isRunning() returns true, but sendMessage() fails, that is an
    indication that the running instance is frozen.

    Here's an example that shows how to convert an existing
    application to use VPubDaemon. It is very simple and does
    not make use of all VPubDaemon's functionality (see the
    examples for that).

    \code
    // Original
    int main(int argc, char **argv)
    {
        QApplication app(argc, argv);

        MyMainWidget mmw;
        mmw.show();
        return app.exec();
    }

    // Single instance
    int main(int argc, char **argv)
    {
        VPubDaemon app(argc, argv);

        if (app.isRunning())
            return !app.sendMessage(someDataString);

        MyMainWidget mmw;
        app.setActivationWindow(&mmw);
        mmw.show();
        return app.exec();
    }
    \endcode

    Once this VPubDaemon instance is destroyed (normally when
    the process exits or crashes), when the user next attempts to run the
    application this instance will not, of course, be encountered. The
    next instance to call isRunning() or sendMessage() will assume the
    role as the new running instance.

    For console (non-GUI) applications, QtSingleCoreApplication may be
    used instead of this class, to avoid the dependency on the QtGui
    library.

    \sa QtSingleCoreApplication
*/


void VPubDaemon::sysInit(const QString &appId)
{
    peer = new QtLocalPeer(this, appId);
    connect(peer, SIGNAL(messageReceived(const QString&)), SIGNAL(messageReceived(const QString&)));
}
