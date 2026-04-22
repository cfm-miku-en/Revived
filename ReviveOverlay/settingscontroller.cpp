#include "settingscontroller.h"
#include "revivemanifestcontroller.h"
#include "revivesettings.h"
#include <QDir>
#include <QFileDialog>
#include <QRegularExpression>
#include <QSettings>

static CSettingsController *s_pSharedSettings = nullptr;

static const QString kDefaultAccent     = QStringLiteral("#1CC4F7");
static const QString kDefaultBorder     = QStringLiteral("#B4DFF7");
static const QString kDefaultBackground = QStringLiteral("#0E1A2B");

static const QRegularExpression kHexColor(QStringLiteral("^#?([0-9a-fA-F]{6})$"));

static QString coerceColor(const QString &raw, const QString &fallback)
{
	auto match = kHexColor.match(raw.trimmed());
	if (!match.hasMatch())
		return fallback;
	return QStringLiteral("#") + match.captured(1).toUpper();
}

CSettingsController *CSettingsController::SharedInstance()
{
	if (!s_pSharedSettings)
		s_pSharedSettings = new CSettingsController();
	return s_pSharedSettings;
}

CSettingsController::CSettingsController()
	: BaseClass()
	, m_runtime(OpenVR)
	, m_accentColor(kDefaultAccent)
	, m_borderColor(kDefaultBorder)
	, m_backgroundColor(kDefaultBackground)
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
	m_accentColor = coerceColor(reg.value(QStringLiteral("AccentColor")).toString(), kDefaultAccent);
	m_borderColor = coerceColor(reg.value(QStringLiteral("BorderColor")).toString(), kDefaultBorder);
	m_backgroundColor = coerceColor(reg.value(QStringLiteral("BackgroundColor")).toString(), kDefaultBackground);
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

	if (!kHexColor.match(m_accentColor).hasMatch())
		return QStringLiteral("Accent color must be a valid hex color (e.g. #1CC4F7).");
	if (!kHexColor.match(m_borderColor).hasMatch())
		return QStringLiteral("Border color must be a valid hex color (e.g. #B4DFF7).");
	if (!kHexColor.match(m_backgroundColor).hasMatch())
		return QStringLiteral("Background color must be a valid hex color (e.g. #0E1A2B).");

	m_accentColor = coerceColor(m_accentColor, kDefaultAccent);
	m_borderColor = coerceColor(m_borderColor, kDefaultBorder);
	m_backgroundColor = coerceColor(m_backgroundColor, kDefaultBackground);

	QSettings reg(QStringLiteral("HKEY_CURRENT_USER\\Software\\Revived"), QSettings::NativeFormat);
	reg.setValue(QStringLiteral("RuntimePreference"), m_runtime == OpenXR ? QStringLiteral("OpenXR") : QStringLiteral("OpenVR"));
	reg.setValue(QStringLiteral("MetaHorizonPath"), m_metaHorizonPath);
	reg.setValue(QStringLiteral("GamesLibraryPath"), m_gamesLibraryPath);
	reg.setValue(QStringLiteral("AccentColor"), m_accentColor);
	reg.setValue(QStringLiteral("BorderColor"), m_borderColor);
	reg.setValue(QStringLiteral("BackgroundColor"), m_backgroundColor);

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
	reg.remove(QStringLiteral("BorderColor"));
	reg.remove(QStringLiteral("BackgroundColor"));

	m_runtime = OpenVR;
	m_metaHorizonPath.clear();
	m_gamesLibraryPath.clear();
	m_accentColor = kDefaultAccent;
	m_borderColor = kDefaultBorder;
	m_backgroundColor = kDefaultBackground;

	emit settingsChanged();
}

QString CSettingsController::browseForFolder(const QString &hint)
{
	return QFileDialog::getExistingDirectory(nullptr, QStringLiteral("Select Folder"), hint);
}

QString CSettingsController::detectedMetaHorizonPath() const
{
	return QDir::fromNativeSeparators(CReviveManifestController::DetectBasePath());
}

QString CSettingsController::detectedGamesLibraryPath() const
{
	return QDir::fromNativeSeparators(CReviveManifestController::DetectDefaultLibraryPath());
}

QString CSettingsController::normalizeHex(const QString &input) const
{
	auto match = kHexColor.match(input.trimmed());
	if (!match.hasMatch())
		return QString();
	return QStringLiteral("#") + match.captured(1).toUpper();
}
