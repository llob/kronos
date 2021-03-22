#include "weeklytotalcalculator.h"

WeeklyTotalCalculator::WeeklyTotalCalculator(QObject *parent) : QObject(parent)
{
    mTotal = 0;
    QObject::connect(&mJiraClient, &JiraClient::searchFinished,
                     this, &WeeklyTotalCalculator::jiraClientSearchFinished);
    QObject::connect(&mJiraClient, &JiraClient::issueWorklogsFinished,
                     this, &WeeklyTotalCalculator::jiraClientIssueWorklogsFinished);
}

QDate WeeklyTotalCalculator::firstWeekDay() {
    auto today = QDate::currentDate();
    return today.addDays(-1 * (today.dayOfWeek() - 1));
}

void WeeklyTotalCalculator::update()
{
    mTotal = 0;
    auto firstDayOfWeek = firstWeekDay();
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
    QDate d = firstWeekDay();
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
