#include "weeklytotalcalculator.h"

WeeklyTotalCalculator::WeeklyTotalCalculator(QObject *parent) : QObject(parent)
{
    mTotal = 0;
    QObject::connect(&mJiraClient, &JiraClient::searchFinished,
                     this, &WeeklyTotalCalculator::jiraClientSearchFinished);
    QObject::connect(&mJiraClient, &JiraClient::issueWorklogsFinished,
                     this, &WeeklyTotalCalculator::jiraClientIssueWorklogsFinished);
}

void WeeklyTotalCalculator::update()
{
    mTotal = 0;
    auto today = QDate::currentDate();
    auto firstDayOfWeek = today.addDays(-1 * (today.dayOfWeek() - 1));
    mJiraClient.search(QString("worklogDate = %1 ORDER BY created DESC")
                           .arg(JiraClient::jqlDate(firstDayOfWeek)));
}

void WeeklyTotalCalculator::jiraClientSearchFinished(QList<QSharedPointer<JiraIssue> > issues)
{
    foreach(QSharedPointer<JiraIssue> issue, issues) {
        mJiraClient.issueWorklogs(issue);
    }
}

void WeeklyTotalCalculator::jiraClientIssueWorklogsFinished(QList<QSharedPointer<JiraWorklog> > worklogs)
{
    foreach (QSharedPointer<JiraWorklog> worklog, worklogs) {
        mTotal += worklog->timeSpentSeconds();
    }
    emit updated(mTotal);
}
