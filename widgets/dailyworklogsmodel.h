#ifndef DAILYWORKLOGSMODEL_H
#define DAILYWORKLOGSMODEL_H

#include <QObject>
#include "jira/jiraclient.h"
#include "jira/jiraworklog.h"
#include "utils/condition.h"
#include "settings/settings.h"
#include "utils/recentissues.h"
#include "cache/issuecache.h"

class DailyWorklogsModel : public QObject
{
    Q_OBJECT
private:
    QSharedPointer<JiraClient> mJiraClient;
    QList<QSharedPointer<JiraWorklog>> mWorklogs;
    IssueCache mIssueCache;
    QDate mCurrentDate;
    Settings mSettings;
    RecentIssues mRecentIssues;
public:
    DailyWorklogsModel(QSharedPointer<JiraClient> jiraClient);
    QList<QSharedPointer<JiraWorklog>> worklogs() const;
    QSharedPointer<AbstractIssue> issueById(const QString issueId) const;
    QList<QSharedPointer<AbstractIssue>> recentIssues() const;
public slots:
    void addWorklog(QTime startTime, QTime endTime, QSharedPointer<AbstractIssue> issue);
    void deleteWorklog(QSharedPointer<JiraWorklog> worklog);
    void setCurrentDate(const QDate date);

    void issueWorklogsFinished(QList<QSharedPointer<JiraWorklog> > worklogs);
    void issueWorklogsFailed(int httpCode, QNetworkReply::NetworkError error, QString message);

    void searchFinished(QList<QSharedPointer<AbstractIssue> > issues);
    void searchFailed(int httpCode, QNetworkReply::NetworkError error, QString message);

    void addWorklogFinished(QSharedPointer<JiraWorklog> worklog);
    void addWorklogFailed(int httpCode, QNetworkReply::NetworkError error, QString message);

    void deleteWorklogFinished(bool success);
    void deleteWorklogFailed(int httpCode, QNetworkReply::NetworkError error, QString message);
signals:
    void updated();
};

#endif // DAILYWORKLOGSMODEL_H
