#include "settings.h"

SettingsPrivate* Settings::mSettings = new SettingsPrivate();

Settings::Settings()
{
    QObject::connect(Settings::mSettings, &SettingsPrivate::updated,
                     this, &Settings::updated);
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

QString Settings::jiraUsername() const
{
    return mSettings->get("jiraUsername").toString();
}

void Settings::setJiraUsername(const QString username)
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
