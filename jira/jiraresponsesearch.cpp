#include "jiraresponsesearch.h"

JiraResponseSearch::JiraResponseSearch(QByteArray response) : JiraResponse(response)
{
    mParsed = false;
    mValid = false;
}

void JiraResponseSearch::parse() {
    QVariant rootNode = jsonDocument().toVariant();
    mValid = false;
    mParsed = true;
    if (!rootNode.canConvert(QMetaType::QVariantMap)) {
        qDebug() << "[JiraResponseSearch] Failed to convert root node to map";
        return;
    }

    QVariantMap root = rootNode.toMap();
    QVariant expandNode = root.value("expand");
    QVariant startAtNode = root.value("startAt");
    QVariant maxResultsNode = root.value("maxResults");
    QVariant issuesNode = root.value("issues");

    if (!issuesNode.convert(QMetaType::QVariantList)) {
        qDebug() << "[JiraResponseSearch] Failed to convert issues node to list";
        return;
    }

    QVariantList issues = issuesNode.toList();
    foreach (QVariant issueNode, issues) {
        // Loop over each issue which is itself a map
        if (!issueNode.canConvert(QMetaType::QVariantMap)) {
            qDebug() << "[JiraResponseSearch] Failed to convert issue to map";
            return;
        }
        QVariantMap issue = issueNode.toMap();
        auto ji = QSharedPointer<JiraIssue>(new JiraIssue());
        ji->setKey(issue.value("key").toString());
        mIssues.append(ji);
    }
    mValid = true;
}

QList<QSharedPointer<JiraIssue> > JiraResponseSearch::issues()
{
    if (!mParsed) {
        parse();
    }
    return mIssues;
}
