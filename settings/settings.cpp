#include "settings.h"
#include "jira/jiraissue.h"

Settings::Settings()
{
    static SettingsPrivate* settings = new SettingsPrivate();
    mSettings = QSharedPointer<SettingsPrivate>(settings);
    QObject::connect(mSettings.data(), &SettingsPrivate::updated,
                     this, &Settings::updated);
}

QString Settings::jiraHostname() const
{
    return mSettings->get("jiraHostname").toString();
}

void Settings::setJiraHostname(QString jiraHostname) {
    mSettings->set("jiraHostname", jiraHostname);
}

QList<QSharedPointer<JiraIssue> > Settings::recentIssues() const
{
    QList<QSharedPointer<JiraIssue>> issues;
    QVariantList variant = mSettings->get("recentIssues").toList();
    foreach (QVariant issueVariant, variant) {
        auto issue = QSharedPointer<JiraIssue>(new JiraIssue(issueVariant.toMap()));
        issues.append(issue);
    }
    return issues;
}

void Settings::setRecentIssues(QList<QSharedPointer<JiraIssue> > issues)
{
    QVariantList variant;
    foreach (QSharedPointer<JiraIssue> issue, issues) {
        variant.append(issue->toVariant());
    }
    mSettings->set("recentIssues", variant);
}

QString Settings::jiraAccountId() const
{
    return mSettings->get("accountId").toString();
}

void Settings::setJiraAccountId(const QString accountId) const
{
    return mSettings->set("accountId", accountId);
}

QPixmap Settings::jiraAvatar() const
{
    QByteArray bArray = mSettings->get("jiraAvatar").toByteArray();
    QPixmap result;
    result.loadFromData(bArray, "PNG");
    return result;
}

void Settings::setJiraAvatar(const QImage avatar)
{
        QByteArray bArray;
        QBuffer buffer(&bArray);
        buffer.open(QIODevice::WriteOnly);
        avatar.save(&buffer, "PNG");
        mSettings->set("jiraAvatar", QVariant(bArray));
}

bool Settings::hasJiraAvatar() const
{
    return !jiraAvatar().isNull();
}

void Settings::setJiraDisplayName(const QString displayName)
{
    return mSettings->set("jiraDisplayName", displayName);
}

QString Settings::jiraDisplayName() const
{
    return mSettings->get("jiraDisplayName").toString();
}

QString Settings::username() const
{
    return mSettings->get("jiraUsername").toString();
}

void Settings::setUsername(const QString username)
{
    mSettings->set("jiraUsername", username);
}

QString Settings::jiraToken() const
{
    return mSettings->get("jiraToken").toString();
}

void Settings::setJiraToken(const QString token)
{
    mSettings->set("jiraToken", token);
}
