#ifndef JIRACLIENT_H
#define JIRACLIENT_H

#include <QDate>
#include <QSharedPointer>
#include <QList>
#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QNetworkReply>
#include "utils/condition.h"
#include "jira/jiraissue.h"
#include "jira/jiraworklog.h"
#include "jira/jirauser.h"
#include "settings/settings.h"

class JiraClient : public QObject
{
    Q_OBJECT
private:
    QString mUsername;
    QString mToken;
    QNetworkReply *get(QUrl url);
    QNetworkReply *post(QUrl url, QByteArray data);
    QUrl url(QString path, QString query);
    QSharedPointer<QNetworkAccessManager> mNam;
    Settings mSettings;

    /**
     * @brief httpCode Get the HTTP code from the network reply
     * @param reply The QNetworkReply
     * @return the HTTP code or 0 if no valid code could be found
     */
    static int httpCode(const QNetworkReply *reply);

    bool hasHttpCode(QNetworkReply *reply, QList<int> httpCodes);
public:
    JiraClient();
    static QString jqlDate(QDate date);
    void setUsername(const QString username);
    void setToken(const QString token);
    void session();
    void myself();
    void addWorklog(QSharedPointer<JiraWorklog> worklog);
    void deleteWorklog(QSharedPointer<JiraWorklog> worklog);
    void issueWorklogs(QSharedPointer<JiraIssue> issue);
    void search(const QString query, int startAt = 0, int maxResults = 20);
    QNetworkReply *delete_request(QUrl url);
signals:
    void searchFinished(QList<QSharedPointer<JiraIssue>> issues);
    void searchFailed(int httpCode, QNetworkReply::NetworkError error, QString errorMessage);

    void myselfFinished(QSharedPointer<JiraUser> myself);
    void myselfFailed(int httpCode, QNetworkReply::NetworkError error, QString errorMessage);

    void addWorklogFinished(QSharedPointer<JiraWorklog> worklogs);
    void addWorklogFailed(int httpCode, QNetworkReply::NetworkError error, QString errorMessage);

    void deleteWorklogFinished(bool result);
    void deleteWorklogFailed(int httpCode, QNetworkReply::NetworkError error, QString errorMessage);

    void issueWorklogsFinished(QList<QSharedPointer<JiraWorklog>> worklogs);
    void issueWorklogsFailed(int httpCode, QNetworkReply::NetworkError error, QString errorMessage);

private slots:
    void settingsUpdated();
};

#endif // JIRACLIENT_H
