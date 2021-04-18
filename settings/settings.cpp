#include "settings.h"
#include "jira/jiraissue.h"
#include "utils/images.h"

Settings::Settings()
{
    static QSharedPointer<SettingsPrivate> settings = QSharedPointer<SettingsPrivate>(new SettingsPrivate());
    mSettings = settings;
    QObject::connect(mSettings.data(), &SettingsPrivate::updated,
                     this, &Settings::updated);
    QObject::connect(mSettings.data(), &SettingsPrivate::credentialsUpdated,
                     this, &Settings::credentialsUpdated);
}

QString Settings::hostname() const
{
    return mSettings->get("jiraHostname").toString();
}

void Settings::setHostname(QString jiraHostname, bool suppressUpdate) {
    mSettings->setCredential("jiraHostname", jiraHostname, suppressUpdate);
    if (!suppressUpdate) {
        emit credentialsUpdated();
    }
}

QList<QSharedPointer<AbstractIssue> > Settings::recentIssues() const
{
    QList<QSharedPointer<AbstractIssue>> issues;
    QVariantList variant = mSettings->get("recentIssues").toList();
    foreach (QVariant issueVariant, variant) {
        auto issue = QSharedPointer<JiraIssue>(new JiraIssue(issueVariant.toMap()));
        issues.append(issue);
    }
    return issues;
}

void Settings::setRecentIssues(QList<QSharedPointer<AbstractIssue> > issues)
{
    QVariantList variant;
    foreach (QSharedPointer<AbstractIssue> issue, issues) {
        variant.append(issue->toVariant());
    }
    mSettings->set("recentIssues", variant);
}

void Settings::setWindowGeometry(const QByteArray geometry)
{
    mSettings->set("windowGeometry", geometry);
}

QByteArray Settings::windowGeometry() const
{
    return mSettings->get("windowGeometry").toByteArray();
}

void Settings::setShowCredentials(bool show)
{
    mSettings->set("showCredentials", show);
}

bool Settings::showCredentials() const
{
    return mSettings->get("showCredentials", true).toBool();
}

QVariantMap Settings::nagState() const
{
    return mSettings->get("nagState").toMap();
}

void Settings::setNagState(QVariantMap state)
{
    mSettings->set("nagState", state);
}

void Settings::clear()
{
    mSettings->clear();
}

QString Settings::accountId() const
{
    return mSettings->get("accountId").toString();
}

void Settings::setAccountId(const QString accountId) const
{
    return mSettings->set("accountId", accountId);
}

void Settings::unsetAccountId()
{
    mSettings->clear("accountId");
}

QByteArray Settings::imageToByteArray(const QImage image) const
{
    QByteArray bArray;
    QBuffer buffer(&bArray);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");
    return bArray;
}

QPixmap Settings::avatar() const
{
    QImage defaultAvatar = Images::defaultAvatar();
    QByteArray bArray = mSettings->get("jiraAvatar", imageToByteArray(defaultAvatar.scaledToHeight(48))).toByteArray();
    QPixmap result;
    result.loadFromData(bArray, "PNG");
    return result;
}

void Settings::setAvatar(const QImage avatar)
{
    mSettings->set("jiraAvatar", imageToByteArray(avatar));
}

void Settings::unsetAvatar()
{
    mSettings->clear("jiraAvatar");
}

bool Settings::hasAvatar() const
{
    return !avatar().isNull();
}

void Settings::setDisplayName(const QString displayName)
{
    return mSettings->set("jiraDisplayName", displayName);
}

void Settings::unsetDisplayName()
{
    mSettings->clear("jiraDisplayName");
}

QString Settings::displayName() const
{
    return mSettings->get("jiraDisplayName", "No authenticated user").toString();
}

QString Settings::username() const
{
    return mSettings->get("jiraUsername").toString();
}

void Settings::setUsername(const QString username, bool suppressUpdate)
{
    mSettings->setCredential("jiraUsername", username, suppressUpdate);
    if (!suppressUpdate) {
        emit credentialsUpdated();
    }
}

QString Settings::secret() const
{
    return mSettings->get("jiraToken").toString();
}

void Settings::setSecret(const QString token, bool suppressUpdate)
{
    mSettings->setCredential("jiraToken", token, suppressUpdate);
    if (!suppressUpdate) {
        emit credentialsUpdated();
    }
}
