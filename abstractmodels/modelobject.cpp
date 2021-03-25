#include "modelobject.h"
#include <QJsonDocument>

ModelObject::ModelObject()
{

}

ModelObject::ModelObject(const QByteArray json)
{
    Q_UNUSED(json);
}

ModelObject::ModelObject(const QVariantMap json)
{
    Q_UNUSED(json);
}

bool ModelObject::valid() const
{
    return mValid;
}

void ModelObject::setValid(bool valid)
{
    mValid = valid;
}

bool ModelObject::fromJson(const QVariantMap json)
{
    Q_UNUSED(json);
    return false;
}
