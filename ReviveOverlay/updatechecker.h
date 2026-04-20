#ifndef UPDATECHECKER_H
#define UPDATECHECKER_H

#include <QObject>
#include <QString>

class QNetworkAccessManager;

class CUpdateChecker : public QObject
{
	Q_OBJECT
	typedef QObject BaseClass;

public:
	static CUpdateChecker *SharedInstance();

	Q_INVOKABLE void checkForUpdate();

signals:
	void updateAvailable(const QString &version, const QString &url);
	void upToDate();
	void updateCheckFailed(const QString &reason);

private:
	CUpdateChecker();
	QNetworkAccessManager *m_nam;
};

#endif // UPDATECHECKER_H
