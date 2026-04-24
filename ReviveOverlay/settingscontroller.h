#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include <QObject>
#include <QString>

class CSettingsController : public QObject
{
	Q_OBJECT
	typedef QObject BaseClass;

	Q_PROPERTY(int runtimePreference READ runtimePreference WRITE setRuntimePreference NOTIFY settingsChanged)
	Q_PROPERTY(QString metaHorizonPath READ metaHorizonPath WRITE setMetaHorizonPath NOTIFY settingsChanged)
	Q_PROPERTY(QString gamesLibraryPath READ gamesLibraryPath WRITE setGamesLibraryPath NOTIFY settingsChanged)
	Q_PROPERTY(QString accentColor READ accentColor WRITE setAccentColor NOTIFY settingsChanged)
	Q_PROPERTY(QString borderColor READ borderColor WRITE setBorderColor NOTIFY settingsChanged)
	Q_PROPERTY(QString settingsBackgroundColor READ settingsBackgroundColor WRITE setSettingsBackgroundColor NOTIFY settingsChanged)
	Q_PROPERTY(QString dashboardBackgroundColor READ dashboardBackgroundColor WRITE setDashboardBackgroundColor NOTIFY settingsChanged)

public:
	enum RuntimePreference { OpenVR = 0, OpenXR = 1 };
	Q_ENUM(RuntimePreference)

	static CSettingsController *SharedInstance();

	int runtimePreference() const { return m_runtime; }
	QString metaHorizonPath() const { return m_metaHorizonPath; }
	QString gamesLibraryPath() const { return m_gamesLibraryPath; }
	QString accentColor() const { return m_accentColor; }
	QString borderColor() const { return m_borderColor; }
	QString settingsBackgroundColor() const { return m_settingsBackgroundColor; }
	QString dashboardBackgroundColor() const { return m_dashboardBackgroundColor; }

	Q_INVOKABLE void setRuntimePreference(int value) { m_runtime = value; }
	Q_INVOKABLE void setMetaHorizonPath(const QString &path) { m_metaHorizonPath = path; }
	Q_INVOKABLE void setGamesLibraryPath(const QString &path) { m_gamesLibraryPath = path; }
	Q_INVOKABLE void setAccentColor(const QString &color) { m_accentColor = color; }
	Q_INVOKABLE void setBorderColor(const QString &color) { m_borderColor = color; }
	Q_INVOKABLE void setSettingsBackgroundColor(const QString &color) { m_settingsBackgroundColor = color; }
	Q_INVOKABLE void setDashboardBackgroundColor(const QString &color) { m_dashboardBackgroundColor = color; }

	Q_INVOKABLE QString apply();
	Q_INVOKABLE void cancel();
	Q_INVOKABLE void restoreDefaults();
	Q_INVOKABLE QString browseForFolder(const QString &hint);
	Q_INVOKABLE QString normalizeHex(const QString &input) const;
	Q_INVOKABLE QString detectedMetaHorizonPath() const;
	Q_INVOKABLE QString detectedGamesLibraryPath() const;

signals:
	void settingsChanged();

private:
	CSettingsController();
	void load();

	int m_runtime;
	QString m_metaHorizonPath;
	QString m_gamesLibraryPath;
	QString m_accentColor;
	QString m_borderColor;
	QString m_settingsBackgroundColor;
	QString m_dashboardBackgroundColor;
};

#endif // SETTINGSCONTROLLER_H
