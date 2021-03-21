#include "jiraobject.h"
#include <QJsonDocument>

JiraObject::JiraObject()
{

}

JiraObject::JiraObject(const QByteArray json)
{
}

JiraObject::JiraObject(const QVariantMap json)
{
}

bool JiraObject::valid() const
{
    return mValid;
}

void JiraObject::setValid(bool valid)
{
    mValid = valid;
}

bool JiraObject::fromJson(const QVariantMap json)
{
    return false;
}
