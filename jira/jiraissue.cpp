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
        if (!issueNode.canConvert<QVariantMap>()) {
            qWarning() << "[JiraResponseSearch] Failed to convert issue to map";
            continue;
        }
        QVariantMap issue = issueNode.toMap();
        auto ji = QSharedPointer<JiraIssue>(new JiraIssue(issue));
        issues.append(ji);
    }
    return issues;
}

QVariant JiraIssue::toVariant() const
{
    QVariantMap result;
    QVariantMap fields;
    fields.insert("summary", mSummary);
    result.insert("fields", fields);
    result.insert("key", mKey);
    result.insert("id", mId);
    return result;
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
