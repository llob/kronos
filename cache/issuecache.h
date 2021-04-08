#ifndef ISSUECACHE_H
#define ISSUECACHE_H

#include <QString>
#include "issuecacheprivate.h"
#include "abstractmodels/abstractissue.h"

class IssueCache
{
private:
    static QSharedPointer<IssueCachePrivate> mIssueCachePrivate;
public:
    IssueCache();
    PAbstractIssue getByKey(QString key) const;
    PAbstractIssue getById(QString id) const;
    void put(PAbstractIssue issue);
    void putList(QList<PAbstractIssue> issues);
};

#endif // ISSUECACHE_H
