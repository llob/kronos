#include "abstractworklog.h"

AbstractWorklog::AbstractWorklog()
{

}

AbstractWorklog::AbstractWorklog(const QByteArray json) : ModelObject(json)
{

}

AbstractWorklog::AbstractWorklog(const QVariantMap json) : ModelObject(json)
{

}


QString AbstractWorklog::id() const
{
    return mId;
}

void AbstractWorklog::setId(const QString id)
{
    mId = id;
}

QString AbstractWorklog::issueId() const
{
    return mIssueId;
}

void AbstractWorklog::setIssueId(const QString issueId)
{
    mIssueId = issueId;
}

QDateTime AbstractWorklog::started() const
{
    return mStarted;
}

void AbstractWorklog::setStarted(const QDateTime started)
{
    mStarted = started;
}

int AbstractWorklog::timeSpentSeconds() const
{
    return mTimeSpentSeconds;
}

void AbstractWorklog::setTimeSpentSeconds(int timeSpentSeconds)
{
    mTimeSpentSeconds = timeSpentSeconds;
}

QString AbstractWorklog::accountId() const
{
    return mAccountId;
}

void AbstractWorklog::setAccountId(const QString accountId)
{
    mAccountId = accountId;
}

QString AbstractWorklog::emailAddress() const
{
    return mEmailAddress;
}

void AbstractWorklog::setEmailAddress(const QString emailAddress)
{
    mEmailAddress = emailAddress;
}

bool AbstractWorklog::contains(QDateTime dateTime)
{
    QDateTime endTime = mStarted.addSecs(mTimeSpentSeconds);
    return (dateTime <= endTime && dateTime >= mStarted);
}
