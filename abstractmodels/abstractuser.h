#ifndef ABSTRACTUSER_H
#define ABSTRACTUSER_H

#include "modelobject.h"

class AbstractUser : public ModelObject
{
protected:
    QString mAccountId;
    QString mName;
    QString mEmailAddress;
    QStringList mAvatarUrls;
    QString mDisplayName;
    bool mActive;
public:
    AbstractUser();
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
};

#endif // ABSTRACTUSER_H
