#ifndef JIRAOBJECT_H
#define JIRAOBJECT_H

#include <QByteArray>
#include <QJsonObject>
#include <QVariantMap>

class JiraObject
{
    bool mValid = false;
public:
    JiraObject();
    JiraObject(const QByteArray json);
    JiraObject(const QVariantMap json);
    virtual QByteArray toJson() const = 0;
    bool valid() const;
protected:
    void setValid(bool valid);
    bool fromJson(const QVariantMap json);
};

#endif // JIRAOBJECT_H
