#ifndef DAILYREGISTRATIONSMODEL_H
#define DAILYREGISTRATIONSMODEL_H

#include <QObject>
#include "models/registration.h"
#include "jira/jiraclient.h"
#include "jira/jiraworklog.h"
#include "utils/condition.h"
#include "settings/settings.h"

class DailyRegistrationsModel : public QObject
{
    Q_OBJECT
private:
    JiraClient *mJiraClient;
    QList<QSharedPointer<JiraWorklog>> mWorklogs;
    QList<QSharedPointer<JiraIssue>> mIssues;
    QDate mCurrentDate;
    Settings mSettings;
public:
    DailyRegistrationsModel(JiraClient *jiraClient);
    QList<QSharedPointer<JiraWorklog>> worklogs() const;
    QSharedPointer<JiraIssue> issueById(const QString issueId) const;
public slots:
    void createRegistration(QTime startTime, QTime endTime, QSharedPointer<JiraIssue> issue);
    void deleteRegistration(QSharedPointer<JiraWorklog> worklog);
    void setCurrentDate(const QDate date);
    void issueWorklogsFinished(QList<QSharedPointer<JiraWorklog> > worklogs);
    void searchFinished(QList<QSharedPointer<JiraIssue>> issues);
    void addWorklogFinished(QSharedPointer<JiraWorklog> worklog);
    void deleteWorklogFinished(bool success);
signals:
    void updated();
};

#endif // DAILYREGISTRATIONSMODEL_H
