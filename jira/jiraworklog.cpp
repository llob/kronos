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
    if (!authorNode.canConvert<QVariantMap>()) {
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

JiraWorklog::JiraWorklog(const QByteArray json) : AbstractWorklog(json)
{
    auto jsonDocument = QJsonDocument::fromJson(json);
    fromJson(jsonDocument.toVariant().toMap());
}

JiraWorklog::JiraWorklog(const QVariantMap json) : AbstractWorklog(json)
{
    fromJson(json);
}

JiraWorklog::~JiraWorklog()
{

}

QList<QSharedPointer<JiraWorklog> > JiraWorklog::fromJsonList(const QVariantList list)
{
    QList<QSharedPointer<JiraWorklog>> worklogs;
    foreach (QVariant worklogNode, list) {
        // Loop over each issue which is itself a map
        if (!worklogNode.canConvert<QVariantMap>()) {
            qWarning() << "[JiraResponseWorklog] Failed to convert issue to map";
        }
        QVariantMap worklog = worklogNode.toMap();
        auto jw = QSharedPointer<JiraWorklog>(new JiraWorklog(worklog));
        worklogs.append(jw);
    }
    return worklogs;
}

QByteArray JiraWorklog::toJson() const
{
    QVariant obj = toVariant();
    return QJsonDocument::fromVariant(obj).toJson();
}

QVariant JiraWorklog::toVariant() const
{
    QVariantMap obj;
    obj.insert("timeSpentSeconds", mTimeSpentSeconds);
    obj.insert("started", JiraUtils::dateToString(mStarted));
    return obj;
}
