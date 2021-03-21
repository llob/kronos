#ifndef WEEKLYTOTALCALCULATOR_H
#define WEEKLYTOTALCALCULATOR_H

#include <QObject>
#include <QList>
#include <QSharedPointer>
#include "jira/jiraclient.h"
#include "jira/jiraworklog.h"
#include "jira/jiraissue.h"

class WeeklyTotalCalculator : public QObject
{
    Q_OBJECT
private:
    JiraClient mJiraClient;
    int mTotal;
public:
    explicit WeeklyTotalCalculator(QObject *parent = nullptr);
    void update();
signals:
    void updated(int seconds);
private slots:
    void jiraClientSearchFinished(QList<QSharedPointer<JiraIssue>> issues);
    void jiraClientIssueWorklogsFinished(QList<QSharedPointer<JiraWorklog>> worklogs);
};

#endif // WEEKLYTOTALCALCULATOR_H
