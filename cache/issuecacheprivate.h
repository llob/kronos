#ifndef ISSUECACHEPRIVATE_H
#define ISSUECACHEPRIVATE_H

#include "abstractmodels/abstractissue.h"

/**
 * @brief The IssueCachePrivate class caches issues retrieved from
 *  the backend
 */
class IssueCachePrivate
{
private:
    // Mapping from key/id to issue
    QMap<QString, PAbstractIssue> mCacheByKey;
    QMap<QString, PAbstractIssue> mCacheById;

public:
    IssueCachePrivate();
    /**
     * @brief getByKey Get an issue by key.
     * @param key
     * @return nullptr if no such key exists in cache
     */
    PAbstractIssue getByKey(QString key);
    /**
     * @brief getById Get an issue by id
     * @param id
     * @return nullptr if no such key exists in cache
     */
    PAbstractIssue getById(QString id);
    /**
     * @brief put Insert an issue in the cache
     * @param issue
     */
    void put(PAbstractIssue issue);
    /**
     * @brief putList Convenience method for adding
     *  a list of issues to the cache.
     * @param issues
     */
    void putList(QList<PAbstractIssue> issues);
};

#endif // ISSUECACHEPRIVATE_H
