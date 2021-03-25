#ifndef ABSTRACTISSUE_H
#define ABSTRACTISSUE_H

#include <QByteArray>
#include <QString>
#include "modelobject.h"

class AbstractIssue : public ModelObject
{
protected:
    QString mKey;
    QString mSummary;
    QString mId;
public:
    AbstractIssue();
    void setKey(const QString key);
    QString key() const;
    void setId(const QString id);
    QString id() const;
    void setSummary(const QString summary);
    QString summary() const;
    QByteArray toJson() const;
};

typedef QSharedPointer<AbstractIssue> PAbstractIssue;

#endif // ABSTRACTISSUE_H
