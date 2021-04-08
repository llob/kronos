#ifndef ISSUECACHEPRIVATE_H
#define ISSUECACHEPRIVATE_H

#include "abstractmodels/abstractissue.h"

class IssueCachePrivate
{
private:
    QMap<QString, PAbstractIssue> mCacheByKey;
    QMap<QString, PAbstractIssue> mCacheById;

public:
    IssueCachePrivate();
    PAbstractIssue getByKey(QString key);
    PAbstractIssue getById(QString id);
    void put(PAbstractIssue issue);
    void putList(QList<PAbstractIssue> issues);
};

#endif // ISSUECACHEPRIVATE_H
