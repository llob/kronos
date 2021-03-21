#include "monthlytotalcalculator.h"

MonthlyTotalCalculator::MonthlyTotalCalculator(QObject *parent) : QObject(parent)
{
    mTotal = 0;
    QObject::connect(&mJiraClient, &JiraClient::searchFinished,
                     this, &MonthlyTotalCalculator::jiraClientSearchFinished);
    QObject::connect(&mJiraClient, &JiraClient::issueWorklogsFinished,
                     this, &MonthlyTotalCalculator::jiraClientIssueWorklogsFinished);
}

void MonthlyTotalCalculator::update()
{
    mTotal = 0;
    auto today = QDate::currentDate();
    auto firstDayOfMonth = today.addDays(-1 * (today.day() - 1));
    mJiraClient.search(QString("worklogDate = %1 ORDER BY created DESC")
                           .arg(JiraClient::jqlDate(firstDayOfMonth)));
}

void MonthlyTotalCalculator::jiraClientSearchFinished(QList<QSharedPointer<JiraIssue> > issues)
{
    foreach(QSharedPointer<JiraIssue> issue, issues) {
        mJiraClient.issueWorklogs(issue);
    }
}

void MonthlyTotalCalculator::jiraClientIssueWorklogsFinished(QList<QSharedPointer<JiraWorklog> > worklogs)
{
    foreach (QSharedPointer<JiraWorklog> worklog, worklogs) {
        mTotal += worklog->timeSpentSeconds();
    }
    emit updated(mTotal);
}
