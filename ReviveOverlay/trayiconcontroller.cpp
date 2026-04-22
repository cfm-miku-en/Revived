#include "trayiconcontroller.h"
#include "openvroverlaycontroller.h"
#include "revivemanifestcontroller.h"
#include "updatechecker.h"
#include "windowsservices.h"
#include "oculusoauthtokencontroller.h"
#include "version.h"

#include <qt_windows.h>

#include <QCoreApplication>
#include <QDesktopServices>
#include <QFileDialog>
#include <QFileInfo>
#include <QIcon>
#include <QProcess>
#include <QUrl>
#include <QMessageBox>

CTrayIconController *s_pSharedTrayController = NULL;

CTrayIconController *CTrayIconController::SharedInstance()
{
	if ( !s_pSharedTrayController )
	{
		s_pSharedTrayController = new CTrayIconController();
	}
	return s_pSharedTrayController;
}

CTrayIconController::CTrayIconController()
	: BaseClass()
	, m_trayIcon()
	, m_trayIconMenu()
	, m_LastInfo()
	, m_updateAvailableAction(nullptr)
	, m_manualCheck(false)
{
}

CTrayIconController::~CTrayIconController()
{
}

bool CTrayIconController::Init()
{
	m_trayIcon = std::make_unique<QSystemTrayIcon>(QIcon(":/revive_white.ico"));

	m_updateAvailableAction = m_trayIconMenu.addAction(QString());
	m_updateAvailableAction->setVisible(false);
	QObject::connect(m_updateAvailableAction, &QAction::triggered, this, &CTrayIconController::openUpdatePage);

	m_trayIconMenu.addAction("Check for updates", this, SLOT(checkForUpdates()));

	QAction* action = m_trayIconMenu.addAction("Use OpenXR runtime");
	action->setCheckable(true);
	QObject::connect(action, SIGNAL(triggered(bool)), this, SLOT(openxr(bool)));
	m_trayIconMenu.addSeparator();
	m_trayIconMenu.addAction("&Open library", this, SLOT(show()));
	m_trayIconMenu.addAction("Se&ttings", this, SLOT(showSettings()));
	m_trayIconMenu.addAction("&Inject...", this, SLOT(inject()));
	m_trayIconMenu.addAction("&Shortcut...", this, SLOT(shortcut()));
	m_trayIconMenu.addSeparator();
	m_trayIconMenu.addAction("&Help", this, SLOT(showHelp()));
	m_trayIconMenu.addAction("&Quit", this, SLOT(quit()));
	m_trayIcon->setContextMenu(&m_trayIconMenu);
	m_trayIcon->setToolTip("Revived Dashboard");

	connect(m_trayIcon.get(), &QSystemTrayIcon::messageClicked, this, &CTrayIconController::messageClicked);
	connect(m_trayIcon.get(), &QSystemTrayIcon::activated, this, &CTrayIconController::activated);

	QObject::connect(CUpdateChecker::SharedInstance(), &CUpdateChecker::updateAvailable,
	                 this, &CTrayIconController::onUpdateAvailable);
	QObject::connect(CUpdateChecker::SharedInstance(), &CUpdateChecker::upToDate,
	                 this, &CTrayIconController::onUpToDate);
	QObject::connect(CUpdateChecker::SharedInstance(), &CUpdateChecker::updateCheckFailed,
	                 this, &CTrayIconController::onUpdateCheckFailed);

	m_trayIcon->show();
	return true;
}

void CTrayIconController::ShowInformation(ETrayInfo info)
{
	m_LastInfo = info;
	if (!m_trayIcon)
		return;

	switch (info)
	{
		case TrayInfo_AutoLaunchEnabled:
			m_trayIcon->showMessage("Revived succesfully installed",
								   "Revived will automatically add Oculus Store games to your library while SteamVR is running.",
								   QSystemTrayIcon::Information);
		break;
		case TrayInfo_AutoLaunchFailed:
			m_trayIcon->showMessage("Revived did not start correctly",
								   "Unable to set the auto-launch flag, please report this to the Revived issue tracker.",
								   QSystemTrayIcon::Critical);
		break;
		case TrayInfo_OculusLibraryNotFound:
			m_trayIcon->showMessage("Revived did not start correctly",
								   "No Oculus Library was found, click here to install the Oculus Software from oculus.com/setup.",
								   QSystemTrayIcon::Warning);
		break;
		case TrayInfo_OculusAccessTokenNotFound:
			m_trayIcon->showMessage("Unable to load Oculus OAuth token",
								   "Multiplayer may have issues! Sign out & back in to the Oculus app, then reboot your PC or restart Revived & OVRService.",
								   QSystemTrayIcon::Warning);
		break;
	}
}

void CTrayIconController::quit()
{
	m_trayIcon.reset();
	QCoreApplication::quit();
}

void CTrayIconController::openxr(bool checked)
{
	CReviveManifestController::SharedInstance()->UseOpenXR(checked);
}

void CTrayIconController::inject()
{
	QString file = openDialog();
	if (file.isNull())
		return;

	CReviveManifestController::SharedInstance()->LaunchInjector(QDir::toNativeSeparators(file));
}

void CTrayIconController::shortcut()
{
	QString file = openDialog();
	if (file.isNull())
		return;

	QString args = file;
	if (CReviveManifestController::SharedInstance()->UsingOpenXR())
		args.prepend("/openxr ");

	if (!WindowsServices::CreateShortcut(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/" + QFileInfo(file).baseName() + ".lnk",
									QCoreApplication::applicationDirPath() + "/ReviveInjector.exe", args, file))
		return;

	m_trayIcon->showMessage("Shortcut created on desktop",
						   "You can use this shortcut to launch the executable with Revived directly.",
						   QSystemTrayIcon::Information);
}

void CTrayIconController::showHelp()
{
	QDesktopServices::openUrl(QUrl("https://github.com/cfm-miku-en/Revived/wiki"));
}

void CTrayIconController::messageClicked()
{
	switch (m_LastInfo)
	{
		case TrayInfo_AutoLaunchEnabled:
			show();
		break;
		case TrayInfo_AutoLaunchFailed:
			QDesktopServices::openUrl(QUrl("https://github.com/cfm-miku-en/Revived/issues"));
		break;
		case TrayInfo_OculusLibraryNotFound:
			QDesktopServices::openUrl(QUrl("https://oculus.com/setup"));
		break;
		case TrayInfo_UpdateAvailable:
			openUpdatePage();
		break;
		default:
		break;
	}
}

void CTrayIconController::checkForUpdates()
{
	m_manualCheck = true;
	CUpdateChecker::SharedInstance()->checkForUpdate();
}

void CTrayIconController::onUpdateAvailable(const QString &version, const QString &url)
{
	m_updateUrl = url;
	m_updateAvailableAction->setText("Update available (" + version + ")");
	m_updateAvailableAction->setVisible(true);
	m_LastInfo = TrayInfo_UpdateAvailable;
	m_manualCheck = false;
	m_trayIcon->showMessage("Update available",
	                        "Version " + version + " is available. Click here to open the release page.",
	                        QSystemTrayIcon::Information);
}

void CTrayIconController::onUpToDate()
{
	if (!m_manualCheck)
		return;
	m_LastInfo = TrayInfo_UpToDate;
	m_manualCheck = false;
	m_trayIcon->showMessage("Revived",
	                        QStringLiteral("Revived is up to date (current: ") + QStringLiteral(REV_VERSION_STRING) + QStringLiteral(")."),
	                        QSystemTrayIcon::Information);
}

void CTrayIconController::onUpdateCheckFailed(const QString &reason)
{
	if (!m_manualCheck)
		return;
	m_LastInfo = TrayInfo_UpdateCheckFailed;
	m_manualCheck = false;
	m_trayIcon->showMessage("Revived", "Update check failed: " + reason, QSystemTrayIcon::Warning);
}

void CTrayIconController::openUpdatePage()
{
	if (!m_updateUrl.isEmpty())
		QDesktopServices::openUrl(QUrl(m_updateUrl));
}

QString CTrayIconController::openDialog()
{
	return QFileDialog::getOpenFileName(
				nullptr, "Revived",
				QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),
				"Application (*.exe)");
}

void CTrayIconController::show()
{
	COpenVROverlayController::SharedInstance()->ShowWindow();
}

void CTrayIconController::showSettings()
{
	COpenVROverlayController::SharedInstance()->ShowWindow();
	emit showSettingsRequested();
}

void CTrayIconController::activated(QSystemTrayIcon::ActivationReason reason)
{
	if (reason == QSystemTrayIcon::ActivationReason::DoubleClick)
		show();
}
