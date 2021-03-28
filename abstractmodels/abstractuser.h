#ifndef ABSTRACTUSER_H
#define ABSTRACTUSER_H

#include <QString>
#include "modelobject.h"

/**
 * @brief The AbstractUser class encapsulates a user. Users
 *  are the sources of worklogs.
 */
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
    /**
     * @brief accountId Get the unique account id associated with this user
     * @return
     */
    QString accountId() const;
    /**
     * @brief setAccountId Set the unique account id associated with this user
     * @param accountId
     */
    void setAccountId(const QString accountId);
    /**
     * @brief name Get the real name of this user
     * @return
     */
    QString name() const;
    /**
     * @brief setName Set the real name of this user
     * @param name
     */
    void setName(const QString name);
    /**
     * @brief emailAddress Get the email address of this user
     * @return
     */
    QString emailAddress() const;
    /**
     * @brief setEmailAddress Set the email address of this user
     * @param emailAddress
     */
    void setEmailAddress(const QString emailAddress);
    /**
     * @brief avatarUrls Get the list of avatar URLs for this user
     * @return
     */
    QStringList avatarUrls() const;
    /**
     * @brief setAvatarUrls Set the list of avatar URLs for this user
     * @param avatarUrls
     */
    void setAvatarUrls(const QStringList avatarUrls);
    /**
     * @brief displayName Get the displayable name of this user
     * @return
     */
    QString displayName() const;
    /**
     * @brief setDisplayName Set the displayable name of this user
     * @param displayName
     */
    void setDisplayName(const QString displayName);
    /**
     * @brief active Get the active state of this user
     * @return
     */
    bool active() const;
    /**
     * @brief setActive Set the active state of this user
     * @param active
     */
    void setActive(const bool active);
};

typedef QSharedPointer<AbstractUser> PAbstractUser;

#endif // ABSTRACTUSER_H
