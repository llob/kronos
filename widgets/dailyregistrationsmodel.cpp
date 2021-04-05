#include "dailyregistrationsmodel.h"

DailyRegistrationsModel::DailyRegistrationsModel(QSharedPointer<JiraClient> jiraClient)
{
    mJiraClient = jiraClient;
    QObject::connect(mJiraClient.data(), &JiraClient::searchFinished,
                     this, &DailyRegistrationsModel::searchFinished);
    QObject::connect(mJiraClient.data(), &JiraClient::searchFailed,
                     this, &DailyRegistrationsModel::searchFailed);

    QObject::connect(mJiraClient.data(), &JiraClient::issueWorklogsFinished,
                     this, &DailyRegistrationsModel::issueWorklogsFinished);
    QObject::connect(mJiraClient.data(), &JiraClient::issueWorklogsFailed,
                     this, &DailyRegistrationsModel::issueWorklogsFailed);

    QObject::connect(mJiraClient.data(), &JiraClient::addWorklogFinished,
                     this, &DailyRegistrationsModel::addWorklogFinished);
    QObject::connect(mJiraClient.data(), &JiraClient::addWorklogFailed,
                     this, &DailyRegistrationsModel::addWorklogFailed);


    QObject::connect(mJiraClient.data(), &JiraClient::deleteWorklogFinished,
                     this, &DailyRegistrationsModel::deleteWorklogFinished);
    QObject::connect(mJiraClient.data(), &JiraClient::deleteWorklogFailed,
                     this, &DailyRegistrationsModel::deleteWorklogFailed);



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

void DailyRegistrationsModel::addWorklog(QTime startTime, QTime endTime, QSharedPointer<JiraIssue> issue)
{
    mRecentIssues.addIssue(issue);

    QSharedPointer<JiraWorklog> jwl = QSharedPointer<JiraWorklog>(new JiraWorklog());
    jwl->setAccountId(mSettings.accountId());
    jwl->setEmailAddress(mSettings.username());
    jwl->setIssueId(issue->id());
    jwl->setStarted(QDateTime(mCurrentDate, startTime));
    jwl->setTimeSpentSeconds(startTime.secsTo(endTime));

    mJiraClient->addWorklog(jwl);
}

void DailyRegistrationsModel::deleteWorklog(QSharedPointer<JiraWorklog> worklog)
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
        .arg(mSettings.accountId());
    mJiraClient->search(query);
}

void DailyRegistrationsModel::issueWorklogsFinished(QList<QSharedPointer<JiraWorklog>> worklogs)
{
    QString accountId = mSettings.accountId();
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

void DailyRegistrationsModel::issueWorklogsFailed(int httpCode, QNetworkReply::NetworkError error, QString message)
{
    qDebug() << __FUNCTION__ << httpCode << error << message;
}

void DailyRegistrationsModel::searchFinished(QList<QSharedPointer<JiraIssue> > issues)
{
    mIssues = issues;
    foreach (QSharedPointer<JiraIssue> issue, mIssues) {
        mJiraClient->issueWorklogs(issue);
    }
}

void DailyRegistrationsModel::searchFailed(int httpCode, QNetworkReply::NetworkError error, QString message)
{
    qDebug() << __FUNCTION__ << httpCode << error << message;
}

void DailyRegistrationsModel::addWorklogFinished(QSharedPointer<JiraWorklog> worklog)
{
    qDebug() << "Add worklog finished";
    mWorklogs.append(worklog);
    emit updated();
}

void DailyRegistrationsModel::addWorklogFailed(int httpCode, QNetworkReply::NetworkError error, QString message)
{
    qDebug() << __FUNCTION__ << httpCode << error << message;
}

void DailyRegistrationsModel::deleteWorklogFinished(bool success)
{
    Q_UNUSED(success);
    setCurrentDate(mCurrentDate); // Force refresh of loaded worklogs
}

void DailyRegistrationsModel::deleteWorklogFailed(int httpCode, QNetworkReply::NetworkError error, QString message)
{
    qDebug() << __FUNCTION__ << httpCode << error << message;
}
