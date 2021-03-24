#ifndef DAILYREGISTRATIONSMODEL_H
#define DAILYREGISTRATIONSMODEL_H

#include <QObject>
#include "jira/jiraclient.h"
#include "jira/jiraworklog.h"
#include "utils/condition.h"
#include "settings/settings.h"
#include "utils/recentissues.h"

class DailyRegistrationsModel : public QObject
{
    Q_OBJECT
private:
    JiraClient *mJiraClient;
    QList<QSharedPointer<JiraWorklog>> mWorklogs;
    QList<QSharedPointer<JiraIssue>> mIssues;
    QDate mCurrentDate;
    Settings mSettings;
    RecentIssues mRecentIssues;
public:
    DailyRegistrationsModel(JiraClient *jiraClient);
    QList<QSharedPointer<JiraWorklog>> worklogs() const;
    QSharedPointer<JiraIssue> issueById(const QString issueId) const;
    QList<QSharedPointer<JiraIssue>> recentIssues() const;
public slots:
    void createRegistration(QTime startTime, QTime endTime, QSharedPointer<JiraIssue> issue);
    void deleteRegistration(QSharedPointer<JiraWorklog> worklog);
    void setCurrentDate(const QDate date);

    void issueWorklogsFinished(QList<QSharedPointer<JiraWorklog> > worklogs);
    void issueWorklogsFailed(int httpCode, QNetworkReply::NetworkError error, QString message);

    void searchFinished(QList<QSharedPointer<JiraIssue>> issues);
    void searchFailed(int httpCode, QNetworkReply::NetworkError error, QString message);

    void addWorklogFinished(QSharedPointer<JiraWorklog> worklog);
    void addWorklogFailed(int httpCode, QNetworkReply::NetworkError error, QString message);

    void deleteWorklogFinished(bool success);
    void deleteWorklogFailed(int httpCode, QNetworkReply::NetworkError error, QString message);
signals:
    void updated();
};

#endif // DAILYREGISTRATIONSMODEL_H
