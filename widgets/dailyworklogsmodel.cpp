#include "dailyworklogsmodel.h"

DailyWorklogsModel::DailyWorklogsModel(QSharedPointer<JiraClient> jiraClient)
{
    mJiraClient = jiraClient;
    QObject::connect(mJiraClient.data(), &JiraClient::searchFinished,
                     this, &DailyWorklogsModel::searchFinished);
    QObject::connect(mJiraClient.data(), &JiraClient::searchFailed,
                     this, &DailyWorklogsModel::searchFailed);

    QObject::connect(mJiraClient.data(), &JiraClient::issueWorklogsFinished,
                     this, &DailyWorklogsModel::issueWorklogsFinished);
    QObject::connect(mJiraClient.data(), &JiraClient::issueWorklogsFailed,
                     this, &DailyWorklogsModel::issueWorklogsFailed);

    QObject::connect(mJiraClient.data(), &JiraClient::addWorklogFinished,
                     this, &DailyWorklogsModel::addWorklogFinished);
    QObject::connect(mJiraClient.data(), &JiraClient::addWorklogFailed,
                     this, &DailyWorklogsModel::addWorklogFailed);


    QObject::connect(mJiraClient.data(), &JiraClient::deleteWorklogFinished,
                     this, &DailyWorklogsModel::deleteWorklogFinished);
    QObject::connect(mJiraClient.data(), &JiraClient::deleteWorklogFailed,
                     this, &DailyWorklogsModel::deleteWorklogFailed);

    QObject::connect(&mAuthenticationState, &AuthenticationState::stateChanged,
                     this, &DailyWorklogsModel::authenticationStateChanged);



    setCurrentDate(QDate::currentDate());
}

QList<QSharedPointer<JiraWorklog> > DailyWorklogsModel::worklogs() const
{
    return mWorklogs;
}

QSharedPointer<AbstractIssue> DailyWorklogsModel::issueById(const QString issueId) const
{
    return mIssueCache.getById(issueId);
}

QList<QSharedPointer<AbstractIssue> > DailyWorklogsModel::recentIssues() const
{
    return mRecentIssues.issues();
}

void DailyWorklogsModel::addWorklog(QTime startTime, QTime endTime, QSharedPointer<AbstractIssue> issue)
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

void DailyWorklogsModel::deleteWorklog(QSharedPointer<JiraWorklog> worklog)
{
    mJiraClient->deleteWorklog(worklog);
}

void DailyWorklogsModel::setCurrentDate(const QDate date)
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

void DailyWorklogsModel::issueWorklogsFinished(QList<QSharedPointer<JiraWorklog>> worklogs)
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

void DailyWorklogsModel::issueWorklogsFailed(int httpCode, QNetworkReply::NetworkError error, QString message)
{
    qDebug() << __FUNCTION__ << httpCode << error << message;
}

void DailyWorklogsModel::searchFinished(QList<QSharedPointer<AbstractIssue> > issues)
{
    foreach (QSharedPointer<AbstractIssue> issue, issues) {
        mJiraClient->issueWorklogs(issue);
    }
}

void DailyWorklogsModel::searchFailed(int httpCode, QNetworkReply::NetworkError error, QString message)
{
    qDebug() << __FUNCTION__ << httpCode << error << message;
}

void DailyWorklogsModel::addWorklogFinished(QSharedPointer<JiraWorklog> worklog)
{
    mWorklogs.append(worklog);
    emit updated();
}

void DailyWorklogsModel::addWorklogFailed(int httpCode, QNetworkReply::NetworkError error, QString message)
{
    qDebug() << __FUNCTION__ << httpCode << error << message;
}

void DailyWorklogsModel::deleteWorklogFinished(bool success)
{
    Q_UNUSED(success);
    setCurrentDate(mCurrentDate); // Force refresh of loaded worklogs
}

void DailyWorklogsModel::deleteWorklogFailed(int httpCode, QNetworkReply::NetworkError error, QString message)
{
    qDebug() << __FUNCTION__ << httpCode << error << message;
}

void DailyWorklogsModel::authenticationStateChanged(AuthenticationState::State oldState, AuthenticationState::State newState, const QString message)
{
    Q_UNUSED(oldState);
    Q_UNUSED(message);
    if (newState == AuthenticationState::DEAUTHENTICATED) {
        clear();
    } else if (newState == AuthenticationState::AUTHENTICATED) {
        setCurrentDate(mCurrentDate);
    }
}

void DailyWorklogsModel::clear()
{
    mWorklogs.clear();
    emit updated();
}
