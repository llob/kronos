#include "totalcalculatorprivate.h"

TotalCalculatorPrivate::TotalCalculatorPrivate(QString query, QDate firstDate) : QObject(), mFirstDate(firstDate), mQuery(query)
{
    mTotal = 0;
    mIssueWorklogRequestCount = 0;
    QObject::connect(&mJiraClient, &JiraClient::searchFinished,
                     this, &TotalCalculatorPrivate::jiraClientSearchFinished);
    QObject::connect(&mJiraClient, &JiraClient::issueWorklogsFinished,
                     this, &TotalCalculatorPrivate::jiraClientIssueWorklogsFinished);
}

TotalCalculatorPrivate::~TotalCalculatorPrivate()
{
}

void TotalCalculatorPrivate::calculate()
{
    mJiraClient.search(mQuery);
}

void TotalCalculatorPrivate::jiraClientSearchFinished(QList<QSharedPointer<JiraIssue> > issues)
{
    mIssueWorklogRequestCount = issues.length();
    foreach(QSharedPointer<JiraIssue> issue, issues) {
        mJiraClient.issueWorklogs(issue);
    }
}

void TotalCalculatorPrivate::jiraClientIssueWorklogsFinished(QList<QSharedPointer<JiraWorklog> > worklogs)
{
    mIssueWorklogRequestCount--;
    QString jiraAccountId = mSettings.accountId();
    foreach (QSharedPointer<JiraWorklog> worklog, worklogs) {
        if (worklog->accountId() != jiraAccountId) {
            continue;
        }
        if (worklog->started().date() < mFirstDate) {
            continue;
        }
        mTotal += worklog->timeSpentSeconds();
    }
    if (mIssueWorklogRequestCount == 0) {
        // We are done handling all the requests generated, so we can
        // publish the result and delete ourselves
        emit updated(mTotal);
        deleteLater();
    }

}
