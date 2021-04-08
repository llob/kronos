#include "issuecacheprivate.h"

IssueCachePrivate::IssueCachePrivate()
{

}

PAbstractIssue IssueCachePrivate::getByKey(QString key)
{
    if (mCacheByKey.contains(key)) {
        return mCacheByKey.value(key);
    }
    return nullptr;
}

PAbstractIssue IssueCachePrivate::getById(QString id) {
    if (mCacheById.contains(id)) {
        return mCacheById.value(id);
    }
    return nullptr;
}

void IssueCachePrivate::put(PAbstractIssue issue)
{
    mCacheByKey.insert(issue->key(), issue);
    mCacheById.insert(issue->id(), issue);
    qDebug() << "Cache size" << mCacheByKey.size();
}

void IssueCachePrivate::putList(QList<PAbstractIssue> issues)
{
    foreach (PAbstractIssue issue, issues) {
        put(issue);
    }
}
