#include "jiraobject.h"
#include <QJsonDocument>

JiraObject::JiraObject()
{

}

JiraObject::JiraObject(const QByteArray json)
{
    Q_UNUSED(json);
}

JiraObject::JiraObject(const QVariantMap json)
{
    Q_UNUSED(json);
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
    Q_UNUSED(json);
    return false;
}
