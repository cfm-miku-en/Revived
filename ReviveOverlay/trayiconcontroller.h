#ifndef CTRAYICONCONTROLLER_H
#define CTRAYICONCONTROLLER_H

#include <QObject>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QString>
#include <memory>

enum ETrayInfo
{
	TrayInfo_OculusLibraryNotFound,
	TrayInfo_AutoLaunchEnabled,
	TrayInfo_AutoLaunchFailed,
	TrayInfo_OculusAccessTokenNotFound,
	TrayInfo_UpdateAvailable,
	TrayInfo_UpToDate,
	TrayInfo_UpdateCheckFailed,
};

class CTrayIconController : public QObject
{
	Q_OBJECT
	typedef QObject BaseClass;

public:
	static CTrayIconController *SharedInstance();

public:
	CTrayIconController();
	~CTrayIconController();

	bool Init();
	void ShowInformation(ETrayInfo info);

public slots:
	void quit();

protected slots:
	void openxr(bool checked);
	void inject();
	void shortcut();
	void show();
	void showHelp();
	void checkForUpdates();
	void onUpdateAvailable(const QString &version, const QString &url);
	void onUpToDate();
	void onUpdateCheckFailed(const QString &reason);
	void openUpdatePage();
	void messageClicked();
	void activated(QSystemTrayIcon::ActivationReason reason);

private:
	std::unique_ptr<QSystemTrayIcon> m_trayIcon;
	QMenu m_trayIconMenu;
	ETrayInfo m_LastInfo;
	QAction *m_updateAvailableAction;
	QString m_updateUrl;
	bool m_manualCheck;

	QString openDialog();
};

#endif // CTRAYICONCONTROLLER_H
