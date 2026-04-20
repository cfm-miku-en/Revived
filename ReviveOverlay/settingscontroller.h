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

public:
	enum RuntimePreference { OpenVR = 0, OpenXR = 1 };
	Q_ENUM(RuntimePreference)

	static CSettingsController *SharedInstance();

	int runtimePreference() const { return m_runtime; }
	QString metaHorizonPath() const { return m_metaHorizonPath; }
	QString gamesLibraryPath() const { return m_gamesLibraryPath; }
	QString accentColor() const { return m_accentColor; }

	void setRuntimePreference(int value) { m_runtime = value; }
	void setMetaHorizonPath(const QString &path) { m_metaHorizonPath = path; }
	void setGamesLibraryPath(const QString &path) { m_gamesLibraryPath = path; }
	void setAccentColor(const QString &color) { m_accentColor = color; }

	Q_INVOKABLE QString apply();
	Q_INVOKABLE void cancel();
	Q_INVOKABLE void restoreDefaults();
	Q_INVOKABLE QString browseForFolder(const QString &hint);

signals:
	void settingsChanged();

private:
	CSettingsController();
	void load();

	int m_runtime;
	QString m_metaHorizonPath;
	QString m_gamesLibraryPath;
	QString m_accentColor;
};

#endif // SETTINGSCONTROLLER_H
