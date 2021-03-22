#include "dailyregistrationsmodel.h"

DailyRegistrationsModel::DailyRegistrationsModel(JiraClient *jiraClient)
{
    mJiraClient = jiraClient;
    QObject::connect(mJiraClient, &JiraClient::searchFinished,
                     this, &DailyRegistrationsModel::searchFinished);
    QObject::connect(mJiraClient, &JiraClient::issueWorklogsFinished,
                     this, &DailyRegistrationsModel::issueWorklogsFinished);
    QObject::connect(mJiraClient, &JiraClient::addWorklogFinished,
                     this, &DailyRegistrationsModel::addWorklogFinished);
    QObject::connect(mJiraClient, &JiraClient::deleteWorklogFinished,
                     this, &DailyRegistrationsModel::deleteWorklogFinished);
    setCurrentDate(QDate::currentDate());
}

QList<QSharedPointer<JiraWorklog> > DailyRegistrationsModel::worklogs() const
{
    return mWorklogs;
}

QSharedPointer<JiraIssue> DailyRegistrationsModel::issueById(const QString issueId) const
{
    foreach (QSharedPointer<JiraIssue> issue, mIssues) {
        if (issue->id() == issueId) {
            return issue;
        }
    }
    return nullptr;
}

QList<QSharedPointer<JiraIssue> > DailyRegistrationsModel::recentIssues() const
{
    return mRecentIssues.issues();
}

void DailyRegistrationsModel::createRegistration(QTime startTime, QTime endTime, QSharedPointer<JiraIssue> issue)
{
    mRecentIssues.addIssue(issue);

    QSharedPointer<JiraWorklog> jwl = QSharedPointer<JiraWorklog>(new JiraWorklog());
    jwl->setAccountId(mSettings.jiraAccountId());
    jwl->setEmailAddress(mSettings.jiraUsername());
    jwl->setIssueId(issue->id());
    jwl->setStarted(QDateTime(mCurrentDate, startTime));
    jwl->setTimeSpentSeconds(startTime.secsTo(endTime));

    mJiraClient->addWorklog(jwl);
}

void DailyRegistrationsModel::deleteRegistration(QSharedPointer<JiraWorklog> worklog)
{
    mJiraClient->deleteWorklog(worklog);
}

void DailyRegistrationsModel::setCurrentDate(const QDate date)
{
    mCurrentDate = date;

    // Clear the current list of worklogs
    mWorklogs.clear();
    emit updated();

    // Get the list of issues with worklogs for the current date
    QString query = QString("worklogDate = %1 and worklogAuthor = %2 order by created DESC")
        .arg(JiraClient::jqlDate(date))
        .arg(mSettings.jiraAccountId());
    mJiraClient->search(query);
}

void DailyRegistrationsModel::issueWorklogsFinished(QList<QSharedPointer<JiraWorklog>> worklogs)
{
    QString accountId = mSettings.jiraAccountId();
    foreach (QSharedPointer<JiraWorklog> worklog, worklogs) {
        if (worklog->accountId() != accountId) {
            continue;
        }
        if (worklog->started().date() != mCurrentDate) {
            continue;
        }
        mWorklogs.append(worklog);
    }
    emit updated();
}

void DailyRegistrationsModel::searchFinished(QList<QSharedPointer<JiraIssue> > issues)
{
    mIssues = issues;
    foreach (QSharedPointer<JiraIssue> issue, mIssues) {
        mJiraClient->issueWorklogs(issue);
    }
}

void DailyRegistrationsModel::addWorklogFinished(QSharedPointer<JiraWorklog> worklog)
{
    mWorklogs.append(worklog);
    emit updated();
}

void DailyRegistrationsModel::deleteWorklogFinished(bool success)
{
    // FIXME Handle errors
    Q_UNUSED(success);
    setCurrentDate(mCurrentDate); // Force refresh of loaded worklogs
}
