#include "jiraworklog.h"
#include <QJsonDocument>
#include "jirautils.h"

bool JiraWorklog::fromJson(const QVariantMap json)
{
    setId(json.value("id").toString());
    setIssueId(json.value("issueId").toString());
    // Parse the "started" timestamp
    QString startedStr = json.value("started").toString();
    setStarted(JiraUtils::stringToDateTime(startedStr));
    setTimeSpentSeconds(json.value("timeSpentSeconds").toUInt());

    // Parse the "author" object
    QVariant authorNode = json.value("author");
    if (!authorNode.canConvert(QMetaType::QVariantMap)) {
        qDebug() << "[JiraResponseWorklog] Failed to convert author to map";
        return false;
    }
    QVariantMap author = authorNode.toMap();
    setAccountId(author.value("accountId").toString());
    setEmailAddress(author.value("emailAddress").toString());

    return true;
}

JiraWorklog::JiraWorklog()
{

}

JiraWorklog::JiraWorklog(const QByteArray json) : JiraObject(json)
{
    auto jsonDocument = QJsonDocument::fromJson(json);
    fromJson(jsonDocument.toVariant().toMap());
}

JiraWorklog::JiraWorklog(const QVariantMap json) : JiraObject(json)
{
    fromJson(json);
}

QList<QSharedPointer<JiraWorklog> > JiraWorklog::fromJsonList(const QVariantList list)
{
    QList<QSharedPointer<JiraWorklog>> worklogs;
    foreach (QVariant worklogNode, list) {
        // Loop over each issue which is itself a map
        if (!worklogNode.canConvert(QMetaType::QVariantMap)) {
            qWarning() << "[JiraResponseWorklog] Failed to convert issue to map";
        }
        QVariantMap worklog = worklogNode.toMap();
        auto jw = QSharedPointer<JiraWorklog>(new JiraWorklog(worklog));
        worklogs.append(jw);
    }
    return worklogs;
}

QString JiraWorklog::id() const
{
    return mId;
}

void JiraWorklog::setId(const QString id)
{
    mId = id;
}

QString JiraWorklog::issueId() const
{
    return mIssueId;
}

void JiraWorklog::setIssueId(const QString issueId)
{
    mIssueId = issueId;
}

QDateTime JiraWorklog::started() const
{
    return mStarted;
}

void JiraWorklog::setStarted(const QDateTime started)
{
    mStarted = started;
}

int JiraWorklog::timeSpentSeconds() const
{
    return mTimeSpentSeconds;
}

void JiraWorklog::setTimeSpentSeconds(int timeSpentSeconds)
{
    mTimeSpentSeconds = timeSpentSeconds;
}

QString JiraWorklog::accountId() const
{
    return mAccountId;
}

void JiraWorklog::setAccountId(const QString accountId)
{
    mAccountId = accountId;
}

QString JiraWorklog::emailAddress() const
{
    return mEmailAddress;
}

void JiraWorklog::setEmailAddress(const QString emailAddress)
{
    mEmailAddress = emailAddress;
}

QByteArray JiraWorklog::toJson() const
{
    QVariantMap obj;
    obj.insert("timeSpentSeconds", mTimeSpentSeconds);
    obj.insert("started", JiraUtils::dateToString(mStarted));
    return QJsonDocument::fromVariant(obj).toJson();
}

QVariant JiraWorklog::toVariant() const
{
    // FIXME Not yet implemented
    return QVariant();
}

bool JiraWorklog::contains(QDateTime dateTime)
{
    QDateTime endTime = mStarted.addSecs(mTimeSpentSeconds);
    return (dateTime <= endTime && dateTime >= mStarted);
}
