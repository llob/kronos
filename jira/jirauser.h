#ifndef JIRAUSER_H
#define JIRAUSER_H

#include "jiraobject.h"

class JiraUser : public JiraObject
{
private:
    QString mAccountId;
    QString mName;
    QString mEmailAddress;
    QStringList mAvatarUrls;
    QString mDisplayName;
    bool mActive;

public:
    JiraUser();
    JiraUser(const QByteArray json);
    JiraUser(const QVariantMap json);
    QString accountId() const;
    void setAccountId(const QString accountId);
    QString name() const;
    void setName(const QString name);
    QString emailAddress() const;
    void setEmailAddress(const QString emailAddress);
    QStringList avatarUrls() const;
    void setAvatarUrls(const QStringList avatarUrls);
    QString displayName() const;
    void setDisplayName(const QString displayName);
    bool active() const;
    void setActive(const bool active);
    QByteArray toJson() const;

protected:
    bool fromJson(const QVariantMap json);
};

#endif // JIRAUSER_H
