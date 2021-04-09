#include "issuecache.h"

QSharedPointer<IssueCachePrivate> IssueCache::mIssueCachePrivate;

IssueCache::IssueCache()
{
    static auto issueCachePrivate = QSharedPointer<IssueCachePrivate>(new IssueCachePrivate());
    mIssueCachePrivate = issueCachePrivate;
}

PAbstractIssue IssueCache::getByKey(QString key) const
{
    return mIssueCachePrivate->getByKey(key);
}

PAbstractIssue IssueCache::getById(QString id) const
{
    return mIssueCachePrivate->getById(id);
}

void IssueCache::put(PAbstractIssue issue)
{
    mIssueCachePrivate->put(issue);
}

void IssueCache::putList(QList<PAbstractIssue> issues)
{
    mIssueCachePrivate->putList(issues);
}
