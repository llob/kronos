#ifndef MONTHLYTOTALCALCULATOR_H
#define MONTHLYTOTALCALCULATOR_H

#include <QObject>
#include <QList>
#include <QSharedPointer>
#include "jira/jiraclient.h"
#include "jira/jiraworklog.h"
#include "jira/jiraissue.h"

class MonthlyTotalCalculator : public QObject
{
    Q_OBJECT
private:
    JiraClient mJiraClient;
    int mTotal;
public:
    explicit MonthlyTotalCalculator(QObject *parent = nullptr);
    void update();
signals:
    void updated(int seconds);
private slots:
    void jiraClientSearchFinished(QList<QSharedPointer<JiraIssue>> issues);
    void jiraClientIssueWorklogsFinished(QList<QSharedPointer<JiraWorklog>> worklogs);
};

#endif // MONTHLYTOTALCALCULATOR_H
