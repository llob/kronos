#include "totalcalculatorprivate.h"

TotalCalculatorPrivate::TotalCalculatorPrivate(QString query, QDate firstDate) : QObject(), mFirstDate(firstDate), mQuery(query)
{
    mTotal = 0;
    mIssueWorklogRequestCount = 0;
    QObject::connect(&mJiraClient, &JiraClient::searchFailed,
                     this, &TotalCalculatorPrivate::jiraClientSearchFailed);
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
    qDebug() << "[TotalCalculatorPrivate] Recalculating with query" << mQuery;
    mJiraClient.search(mQuery);
}

void TotalCalculatorPrivate::jiraClientSearchFinished(QList<QSharedPointer<AbstractIssue> > issues)
{
    mIssueWorklogRequestCount = issues.length();
    foreach(QSharedPointer<AbstractIssue> issue, issues) {
        mJiraClient.issueWorklogs(issue);
    }
}

void TotalCalculatorPrivate::jiraClientSearchFailed(int httpCode, QNetworkReply::NetworkError error, QString errorMessage)
{
    Q_UNUSED(httpCode);
    Q_UNUSED(error);
    Q_UNUSED(errorMessage);
    mTotal = 0;
    emit updated(mTotal);
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
