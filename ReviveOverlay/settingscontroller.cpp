#include "settingscontroller.h"
#include <QDir>
#include <QFileDialog>
#include <QRegularExpression>
#include <QSettings>

static CSettingsController *s_pSharedSettings = nullptr;

CSettingsController *CSettingsController::SharedInstance()
{
	if (!s_pSharedSettings)
		s_pSharedSettings = new CSettingsController();
	return s_pSharedSettings;
}

CSettingsController::CSettingsController()
	: BaseClass()
	, m_runtime(OpenVR)
	, m_accentColor(QStringLiteral("#1cc4f7"))
{
	load();
}

void CSettingsController::load()
{
	QSettings reg(QStringLiteral("HKEY_CURRENT_USER\\Software\\Revived"), QSettings::NativeFormat);
	QString runtime = reg.value(QStringLiteral("RuntimePreference")).toString();
	m_runtime = (runtime == QStringLiteral("OpenXR")) ? OpenXR : OpenVR;
	m_metaHorizonPath = reg.value(QStringLiteral("MetaHorizonPath")).toString();
	m_gamesLibraryPath = reg.value(QStringLiteral("GamesLibraryPath")).toString();
	QString color = reg.value(QStringLiteral("AccentColor")).toString();
	if (!color.isEmpty())
		m_accentColor = color;
}

QString CSettingsController::apply()
{
	if (!m_metaHorizonPath.isEmpty() && !QDir(m_metaHorizonPath).exists())
		return QStringLiteral("Meta Horizon path does not exist.");

	if (!m_gamesLibraryPath.isEmpty()) {
		if (!QDir(m_gamesLibraryPath).exists())
			return QStringLiteral("Games library path does not exist.");
		if (!m_metaHorizonPath.isEmpty()) {
			if (!QDir::cleanPath(m_gamesLibraryPath).startsWith(QDir::cleanPath(m_metaHorizonPath)))
				return QStringLiteral("Games library path must be inside the Meta Horizon install folder.");
		}
	}

	static const QRegularExpression hexColor(QStringLiteral("^#[0-9a-fA-F]{6}$"));
	if (!hexColor.match(m_accentColor).hasMatch())
		return QStringLiteral("Accent color must be a valid hex color (e.g. #1cc4f7).");

	QSettings reg(QStringLiteral("HKEY_CURRENT_USER\\Software\\Revived"), QSettings::NativeFormat);
	reg.setValue(QStringLiteral("RuntimePreference"), m_runtime == OpenXR ? QStringLiteral("OpenXR") : QStringLiteral("OpenVR"));
	reg.setValue(QStringLiteral("MetaHorizonPath"), m_metaHorizonPath);
	reg.setValue(QStringLiteral("GamesLibraryPath"), m_gamesLibraryPath);
	reg.setValue(QStringLiteral("AccentColor"), m_accentColor);

	emit settingsChanged();
	return QString();
}

void CSettingsController::cancel()
{
	load();
	emit settingsChanged();
}

void CSettingsController::restoreDefaults()
{
	QSettings reg(QStringLiteral("HKEY_CURRENT_USER\\Software\\Revived"), QSettings::NativeFormat);
	reg.remove(QStringLiteral("RuntimePreference"));
	reg.remove(QStringLiteral("MetaHorizonPath"));
	reg.remove(QStringLiteral("GamesLibraryPath"));
	reg.remove(QStringLiteral("AccentColor"));

	m_runtime = OpenVR;
	m_metaHorizonPath.clear();
	m_gamesLibraryPath.clear();
	m_accentColor = QStringLiteral("#1cc4f7");

	emit settingsChanged();
}

QString CSettingsController::browseForFolder(const QString &hint)
{
	return QFileDialog::getExistingDirectory(nullptr, QStringLiteral("Select Folder"), hint);
}
