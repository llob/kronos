#include "jirauser.h"
#include <QByteArray>
#include <QJsonDocument>

JiraUser::JiraUser()
{

}

JiraUser::JiraUser(const QByteArray json)
{
    auto jsonDocument = QJsonDocument::fromJson(json);
    fromJson(jsonDocument.toVariant().toMap());
}

JiraUser::JiraUser(const QVariantMap json)
{
    fromJson(json);
}

QString JiraUser::accountId() const
{
    return mAccountId;
}

void JiraUser::setAccountId(const QString accountId)
{
    mAccountId = accountId;
}

QString JiraUser::name() const
{
    return mName;
}

void JiraUser::setName(const QString name)
{
    mName = name;
}

QString JiraUser::emailAddress() const
{
    return mEmailAddress;
}

void JiraUser::setEmailAddress(const QString emailAddress)
{
    mEmailAddress = emailAddress;
}

QStringList JiraUser::avatarUrls() const
{
    return mAvatarUrls;
}

void JiraUser::setAvatarUrls(const QStringList avatarUrls)
{
    mAvatarUrls = avatarUrls;
}

QString JiraUser::displayName() const
{
    return mDisplayName;
}

void JiraUser::setDisplayName(const QString displayName)
{
    mDisplayName = displayName;
}

bool JiraUser::active() const
{
    return mActive;
}

void JiraUser::setActive(const bool active)
{
    mActive = active;
}

QByteArray JiraUser::toJson() const
{
    QVariantMap obj;
    return QJsonDocument::fromVariant(obj).toJson();
}

QVariant JiraUser::toVariant() const
{
    // FIXME Not yet implemented
    return QVariant();
}

bool JiraUser::fromJson(const QVariantMap json)
{
    setAccountId(json.value("accountId").toString());
    setName(json.value("name").toString());
    foreach (QVariant url, json.value("avatarUrls").toMap().values()) {
        mAvatarUrls.append(url.toString());
    }
    setEmailAddress(json.value("emailAddress").toString());
    setDisplayName(json.value("displayName").toString());
    setActive(json.value("active").toBool());

    return true;
}
