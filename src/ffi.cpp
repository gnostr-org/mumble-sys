#include "MainWindow.h"
#include "ServerHandler.h"
#include "Global.h"
#include "Net.h"
#include "Version.h"
#include "Log.h"
#include "Database.h"
#include "ConnectDialog.h"
#include "Connection.h"
#include "Channel.h"
#include "User.h"
#include "TextMessage.h"
#include "MumbleConstants.h"
#include "MumbleApplication.h"

#include <iostream>
#include <QObject>
#include <QAbstractSocket>

// FFI Callbacks
static on_connected_callback g_on_connected = nullptr;
static on_disconnected_callback g_on_disconnected = nullptr;
static on_error_callback g_on_error = nullptr;

class FFIConnectionCallbacks : public QObject {
    Q_OBJECT
public:
    FFIConnectionCallbacks(QObject *parent = nullptr) : QObject(parent) {}

public slots:
    void handleConnected() {
        if (g_on_connected) {
            g_on_connected();
        }
    }

    void handleDisconnected(QAbstractSocket::SocketError, const QString &reason) {
        if (g_on_disconnected) {
            g_on_disconnected(reason.toUtf8().constData());
        }
    }

    void handleError(QAbstractSocket::SocketError, const QString &reason) {
        if (g_on_error) {
            g_on_error(reason.toUtf8().constData());
        }
    }
};

// We need to include the moc-generated file to provide the implementation for the signals and slots.
#include "ffi.moc"

static FFIConnectionCallbacks *g_ffi_callbacks = nullptr;

static void recreateServerHandler() {
	ServerHandlerPtr sh = Global::get().sh;
	if (sh && sh->isRunning()) {
		// Disconnect any existing connections without triggering the FFI callbacks again.
        if (g_ffi_callbacks) {
            QObject::disconnect(sh.get(), 0, g_ffi_callbacks, 0);
        }
		Global::get().mw->on_qaServerDisconnect_triggered();
		sh->disconnect();
		sh->wait();
		QCoreApplication::instance()->processEvents();
	}

	Global::get().sh.reset();
	while (sh && sh.use_count() > 1)
		QThread::yieldCurrentThread();
	sh.reset();

	sh = ServerHandlerPtr(new ServerHandler());
	sh->moveToThread(sh.get());
	Global::get().sh = sh;

    // Standard MainWindow connections
	Global::get().mw->connect(sh.get(), SIGNAL(connected()), Global::get().mw, SLOT(serverConnected()));
	Global::get().mw->connect(sh.get(), SIGNAL(disconnected(QAbstractSocket::SocketError, QString)), Global::get().mw,
							  SLOT(serverDisconnected(QAbstractSocket::SocketError, QString)));
	Global::get().mw->connect(sh.get(), SIGNAL(error(QAbstractSocket::SocketError, QString)), Global::get().mw,
							  SLOT(resolverError(QAbstractSocket::SocketError, QString)));

    // FFI Connections
    if (!g_ffi_callbacks) {
        g_ffi_callbacks = new FFIConnectionCallbacks();
    }
    QObject::connect(sh.get(), &ServerHandler::connected, g_ffi_callbacks, &FFIConnectionCallbacks::handleConnected);
    QObject::connect(sh.get(), SIGNAL(disconnected(QAbstractSocket::SocketError, const QString &)), g_ffi_callbacks, SLOT(handleDisconnected(QAbstractSocket::SocketError, const QString &)));
    QObject::connect(sh.get(), SIGNAL(error(QAbstractSocket::SocketError, const QString &)), g_ffi_callbacks, SLOT(handleError(QAbstractSocket::SocketError, const QString &)));


	QObject::connect(sh.get(), &ServerHandler::disconnected, Global::get().talkingUI,
					 &TalkingUI::on_serverDisconnected);

	Global::get().pluginManager->connect(sh.get(), &ServerHandler::connected, Global::get().pluginManager,
										 &PluginManager::on_serverConnected, Qt::DirectConnection);
	Global::get().pluginManager->connect(sh.get(), &ServerHandler::aboutToDisconnect, Global::get().pluginManager,
										 &PluginManager::on_serverDisconnected, Qt::DirectConnection);
}

extern "C" {
    void mumble_init() {
        if (!qApp) {
            // This is a bit of a hack. MumbleApplication expects argc and argv,
            // but for a library, we don't have them. We'll create dummy ones.
            static int argc = 1;
            static char *argv[] = { (char*)"mumble-sys", nullptr };
            new MumbleApplication(argc, argv);
        }
    }

    void mumble_process_events() {
        if (qApp) {
            qApp->processEvents();
        }
    }

    void mumble_connect(const char *hostname, int port, const char *username, const char *password,
                        on_connected_callback on_connected,
                        on_disconnected_callback on_disconnected,
                        on_error_callback on_error) {
        g_on_connected = on_connected;
        g_on_disconnected = on_disconnected;
        g_on_error = on_error;

        recreateServerHandler();

        Global::get().sh->setConnectionInfo(hostname, port, username, password);
        Global::get().sh->start(QThread::TimeCriticalPriority);
    }
}