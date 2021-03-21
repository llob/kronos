#include "jiraissue.h"
#include <QJsonDocument>

JiraIssue::JiraIssue()
{

}

JiraIssue::JiraIssue(const QVariantMap json)
{
    fromJson(json);
}

JiraIssue::JiraIssue(const QByteArray json)
{
    auto jsonDocument = QJsonDocument::fromJson(json);
    fromJson(jsonDocument.toVariant().toMap());
}

QList<QSharedPointer<JiraIssue> > JiraIssue::fromJsonList(const QVariantList list)
{
    QList<QSharedPointer<JiraIssue>> issues;
    foreach (QVariant issueNode, list) {
        // Loop over each issue which is itself a map
        if (!issueNode.canConvert(QMetaType::QVariantMap)) {
            qWarning() << "[JiraResponseSearch] Failed to convert issue to map";
            continue;
        }
        QVariantMap issue = issueNode.toMap();
        auto ji = QSharedPointer<JiraIssue>(new JiraIssue(issue));
        issues.append(ji);
    }
    return issues;
}

void JiraIssue::setKey(const QString key)
{
    mKey = key;
}

QString JiraIssue::key() const
{
    return mKey;
}

void JiraIssue::setId(const QString id)
{
    mId = id;
}

QString JiraIssue::id() const
{
    return mId;
}

void JiraIssue::setSummary(const QString summary)
{
    mSummary = summary;
}

QString JiraIssue::summary() const
{
    return mSummary;
}

QByteArray JiraIssue::toJson() const
{
    return "";
}

QString JiraIssue::toString() const
{
    return QString("%1 %2").arg(mKey, mSummary);
}

bool JiraIssue::fromJson(const QVariantMap json)
{
    QVariantMap fields = json.value("fields").toMap();
    mSummary = fields.value("summary").toString();
    mKey = json.value("key").toString();
    mId = json.value("id").toString();
    return true;
}
