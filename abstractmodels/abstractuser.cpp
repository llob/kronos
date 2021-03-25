#include "abstractuser.h"

AbstractUser::AbstractUser()
{

}

QString AbstractUser::accountId() const
{
    return mAccountId;
}

void AbstractUser::setAccountId(const QString accountId)
{
    mAccountId = accountId;
}

QString AbstractUser::name() const
{
    return mName;
}

void AbstractUser::setName(const QString name)
{
    mName = name;
}

QString AbstractUser::emailAddress() const
{
    return mEmailAddress;
}

void AbstractUser::setEmailAddress(const QString emailAddress)
{
    mEmailAddress = emailAddress;
}

QStringList AbstractUser::avatarUrls() const
{
    return mAvatarUrls;
}

void AbstractUser::setAvatarUrls(const QStringList avatarUrls)
{
    mAvatarUrls = avatarUrls;
}

QString AbstractUser::displayName() const
{
    return mDisplayName;
}

void AbstractUser::setDisplayName(const QString displayName)
{
    mDisplayName = displayName;
}

bool AbstractUser::active() const
{
    return mActive;
}

void AbstractUser::setActive(const bool active)
{
    mActive = active;
}
