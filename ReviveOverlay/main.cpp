#include "trayiconcontroller.h"
#include "openvroverlaycontroller.h"
#include "revivemanifestcontroller.h"
#include "settingscontroller.h"
#include "windowsservices.h"
#include "oculusoauthtokencontroller.h"
#include <qt_windows.h>

#include <QApplication>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQuickItem>
#include <QQmlContext>
#include <QUrl>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QSettings>
#include <QStandardPaths>
#include <QSslSocket>
#include <QSurfaceFormat>

extern "C" {
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

QFile* g_LogFile = nullptr;

static QString dataFolderName()
{
	QSettings reg(QStringLiteral("HKEY_CURRENT_USER\\Software\\Revived"), QSettings::NativeFormat);
	if (reg.value(QStringLiteral("DataFolder")).toString() == QStringLiteral("Revived"))
		return QStringLiteral("Revived");
	return QStringLiteral("Revive");
}

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	Q_UNUSED(context);
	QByteArray localMsg = msg.toLocal8Bit();
	QTextStream log(g_LogFile);
	log << localMsg.constData() << "\n";
	OutputDebugStringA(localMsg.constData());
	OutputDebugStringA("\n");

	if (type == QtFatalMsg)
		abort();
}

int main(int argc, char *argv[])
{
	QSurfaceFormat fmt;
	fmt.setSamples(0);
	QSurfaceFormat::setDefaultFormat(fmt);
	QApplication a(argc, argv);

	// Open the log file and install our handler.
	QString logPath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/" + dataFolderName() + "/";
	if (QDir().mkpath(logPath)) {
		g_LogFile = new QFile(logPath + "ReviveOverlay.txt");
		g_LogFile->open(QIODevice::WriteOnly | QIODevice::Truncate);
	}
	qInstallMessageHandler(myMessageOutput);

	// Handle command-line arguments
	if (a.arguments().contains("-manifest")) {
		// Only initialize the manifest
		vr::EVRInitError err = vr::VRInitError_None;
		vr::VR_Init( &err, vr::VRApplication_Utility );

		if ( err != vr::VRInitError_None )
			return -1;

		QString filePath = QDir::toNativeSeparators(QCoreApplication::applicationDirPath() + "/app.vrmanifest");
		vr::VRApplications()->AddApplicationManifest(qPrintable(filePath));
		vr::VRApplications()->SetApplicationAutoLaunch(CReviveManifestController::AppKey, true);
		vr::VR_Shutdown();
		return 0;
	}

	if (COpenVROverlayController::SharedInstance()->Init())
	{
		// If the dashboard was successfully created keep running in the background
		a.setQuitOnLastWindowClosed(false);
	}

	if (!QSslSocket::supportsSsl())
		qDebug("Missing SSL support, online features will not work");

	if (!CTrayIconController::SharedInstance()->Init())
		qDebug("Failed to initialize the tray icon");

	if (!CReviveManifestController::SharedInstance()->Init())
		qDebug("Failed to initialize the revive manifest");

	if (!COculusOauthTokenController::SharedInstance()->Init())
		qDebug("Failed to initialize the Oculus OAuth token");

	// Apply persisted runtime preference
	CReviveManifestController::SharedInstance()->UseOpenXR(
		CSettingsController::SharedInstance()->runtimePreference() == CSettingsController::OpenXR
	);
	QObject::connect(CSettingsController::SharedInstance(), &CSettingsController::settingsChanged, []() {
		CReviveManifestController::SharedInstance()->UseOpenXR(
			CSettingsController::SharedInstance()->runtimePreference() == CSettingsController::OpenXR
		);
	});

	// Create a QML engine.
	QQmlEngine qmlEngine;
	qmlEngine.rootContext()->setContextProperty("Revive", CReviveManifestController::SharedInstance());
	qmlEngine.rootContext()->setContextProperty("OpenVR", COpenVROverlayController::SharedInstance());
	qmlEngine.rootContext()->setContextProperty("Platform", COculusOauthTokenController::SharedInstance());
	qmlEngine.rootContext()->setContextProperty("Settings", CSettingsController::SharedInstance());

	QQmlComponent qmlComponent( &qmlEngine, QUrl("qrc:/Overlay.qml"));
	if (qmlComponent.isError())
	{
		qDebug(qUtf8Printable(qmlComponent.errorString()));
		return -1;
	}

	QObject *rootObject = qmlComponent.create();
	QQuickItem *rootItem = qobject_cast<QQuickItem*>( rootObject );

	COpenVROverlayController::SharedInstance()->SetQuickItem( rootItem );

	QObject::connect(&a, &QApplication::aboutToQuit, CTrayIconController::SharedInstance(), &CTrayIconController::quit);
	return a.exec();
}
