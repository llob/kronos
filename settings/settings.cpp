#include "settings.h"
#include "jira/jiraissue.h"

Settings::Settings()
{
    static SettingsPrivate* settings = new SettingsPrivate();
    mSettings = QSharedPointer<SettingsPrivate>(settings);
    QObject::connect(mSettings.data(), &SettingsPrivate::updated,
                     this, &Settings::updated);
}

QString Settings::hostname() const
{
    return mSettings->get("jiraHostname").toString();
}

void Settings::setHostname(QString jiraHostname) {
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

QString Settings::accountId() const
{
    return mSettings->get("accountId").toString();
}

void Settings::setAccountId(const QString accountId) const
{
    return mSettings->set("accountId", accountId);
}

QPixmap Settings::avatar() const
{
    QByteArray bArray = mSettings->get("jiraAvatar").toByteArray();
    QPixmap result;
    result.loadFromData(bArray, "PNG");
    return result;
}

void Settings::setAvatar(const QImage avatar)
{
        QByteArray bArray;
        QBuffer buffer(&bArray);
        buffer.open(QIODevice::WriteOnly);
        avatar.save(&buffer, "PNG");
        mSettings->set("jiraAvatar", QVariant(bArray));
}

bool Settings::hasAvatar() const
{
    return !avatar().isNull();
}

void Settings::setDisplayName(const QString displayName)
{
    return mSettings->set("jiraDisplayName", displayName);
}

QString Settings::displayName() const
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

QString Settings::secret() const
{
    return mSettings->get("jiraToken").toString();
}

void Settings::setSecret(const QString token)
{
    mSettings->set("jiraToken", token);
}
