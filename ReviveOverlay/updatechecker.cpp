#include "updatechecker.h"
#include "version.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

static CUpdateChecker *s_pSharedUpdateChecker = nullptr;

CUpdateChecker *CUpdateChecker::SharedInstance()
{
	if (!s_pSharedUpdateChecker)
		s_pSharedUpdateChecker = new CUpdateChecker();
	return s_pSharedUpdateChecker;
}

static bool isNewer(const QString &remote, const QString &local)
{
	QStringList r = remote.split('.');
	QStringList l = local.split('.');
	int n = qMax(r.size(), l.size());
	for (int i = 0; i < n; i++) {
		int rv = (i < r.size()) ? r[i].trimmed().toInt() : 0;
		int lv = (i < l.size()) ? l[i].trimmed().toInt() : 0;
		if (rv > lv) return true;
		if (rv < lv) return false;
	}
	return false;
}

CUpdateChecker::CUpdateChecker()
	: BaseClass()
	, m_nam(new QNetworkAccessManager(this))
{
}

void CUpdateChecker::checkForUpdate()
{
	static const QUrl updateUrl(QStringLiteral("https://raw.githubusercontent.com/cfm-miku-en/Revived/master/update.txt"));
	QNetworkReply *reply = m_nam->get(QNetworkRequest(updateUrl));
	QObject::connect(reply, &QNetworkReply::finished, [this, reply]() {
		reply->deleteLater();
		if (reply->error() != QNetworkReply::NoError) {
			qInfo("update check: %s", qUtf8Printable(reply->errorString()));
			emit updateCheckFailed(reply->errorString());
			return;
		}
		int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
		if (status != 200) {
			QString reason = QStringLiteral("HTTP %1").arg(status);
			qInfo("update check: %s", qUtf8Printable(reason));
			emit updateCheckFailed(reason);
			return;
		}
		QString body = QString::fromUtf8(reply->readAll());
		QStringList lines = body.split('\n');
		QString remoteVersion = lines.isEmpty() ? QString() : lines[0].trimmed();
		if (remoteVersion.isEmpty()) {
			qInfo("update check: empty response");
			emit updateCheckFailed(QStringLiteral("empty response"));
			return;
		}
		QString releaseUrl = (lines.size() > 1) ? lines[1].trimmed() : QString();
		if (isNewer(remoteVersion, QStringLiteral(REV_VERSION_STRING)))
			emit updateAvailable(remoteVersion, releaseUrl);
		else
			emit upToDate();
	});
}
