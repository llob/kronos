#ifndef JIRAUSER_H
#define JIRAUSER_H

#include "abstractmodels/abstractuser.h"

class JiraUser : public AbstractUser
{
private:

public:
    JiraUser();
    JiraUser(const QByteArray json);
    JiraUser(const QVariantMap json);
    virtual ~JiraUser();
    QByteArray toJson() const;
    QVariant toVariant() const override;

protected:
    bool fromJson(const QVariantMap json);
};

#endif // JIRAUSER_H
