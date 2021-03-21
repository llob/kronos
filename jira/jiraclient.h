#ifndef JIRACLIENT_H
#define JIRACLIENT_H

#include <QDate>
#include <QSharedPointer>
#include <QList>
#include <QNetworkAccessManager>
#include <QJsonObject>
#include "utils/condition.h"
#include "models/registration.h"
#include "jira/jiraissue.h"
#include "jira/jiraworklog.h"
#include "jira/jirauser.h"
#include "jira/jirasession.h"
#include "settings/settings.h"

class JiraClient : public QObject
{
    Q_OBJECT
private:
    QString mUsername;
    QString mToken;
    QList<QSharedPointer<Registration>> worklogsToRegistrations(QList<QJsonObject> jsonWorklog);
    QNetworkReply *get(QUrl url);
    QNetworkReply *post(QUrl url, QByteArray data);
    QUrl url(QString path, QString query);
    QNetworkAccessManager *mNam;
    Settings mSettings;

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
    QNetworkReply *delete_request(QUrl url, QByteArray data);
signals:
    void searchFinished(QList<QSharedPointer<JiraIssue>> issues);

    void sessionFinished(QSharedPointer<JiraSession> session);

    void myselfFinished(QSharedPointer<JiraUser> myself);

    void addWorklogFinished(QSharedPointer<JiraWorklog> worklogs);
    void deleteWorklogFinished(bool result);
    void issueWorklogsFinished(QList<QSharedPointer<JiraWorklog>> worklogs);
private slots:
    void settingsUpdated();
};

#endif // JIRACLIENT_H
