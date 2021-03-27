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

JiraUser::~JiraUser()
{

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
