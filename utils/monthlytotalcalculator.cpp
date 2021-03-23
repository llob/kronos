#include "monthlytotalcalculator.h"

QDate MonthlyTotalCalculator::firstMonthDay() const
{
    auto today = QDate::currentDate();
    return today.addDays(-1 * (today.day() - 1));
}

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
    auto firstDayOfMonth = firstMonthDay();
    mJiraClient.search(QString("worklogDate >= %1 and worklogAuthor = %2 ORDER BY created DESC")
                           .arg(JiraClient::jqlDate(firstDayOfMonth))
                           .arg(mSettings.jiraAccountId()));
}

void MonthlyTotalCalculator::jiraClientSearchFinished(QList<QSharedPointer<JiraIssue> > issues)
{
    foreach(QSharedPointer<JiraIssue> issue, issues) {
        mJiraClient.issueWorklogs(issue);
    }
}

void MonthlyTotalCalculator::jiraClientIssueWorklogsFinished(QList<QSharedPointer<JiraWorklog> > worklogs)
{
    QDate d = firstMonthDay();
    QString jiraAccountId = mSettings.jiraAccountId();
    foreach (QSharedPointer<JiraWorklog> worklog, worklogs) {
        if (worklog->accountId() != jiraAccountId) {
            continue;
        }
        if (worklog->started().date() < d) {
            continue;
        }
        mTotal += worklog->timeSpentSeconds();
    }
    emit updated(mTotal);
}
