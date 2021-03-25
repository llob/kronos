#ifndef MODELOBJECT_H
#define MODELOBJECT_H

#include <QByteArray>
#include <QJsonObject>
#include <QVariantMap>
#include <QSharedPointer>

class ModelObject
{
    bool mValid = false;
public:
    ModelObject();
    ModelObject(const QByteArray json);
    ModelObject(const QVariantMap json);
    virtual QByteArray toJson() const = 0;
    virtual QVariant toVariant() const = 0;
    bool valid() const;
protected:
    void setValid(bool valid);
    bool fromJson(const QVariantMap json);
};

#endif // MODELOBJECT_H
