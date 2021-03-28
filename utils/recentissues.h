#ifndef RECENTISSUES_H
#define RECENTISSUES_H

#include <QObject>
#include <QSharedPointer>
#include <QMap>
#include <QList>
#include "settings/settings.h"
#include "jira/jiraissue.h"

/**
 * @brief The RecentIssues class provides a convenient
 *  way of tracking recently accessed issues.
 *  This class operates as a FIFO queue limited by
 *  the maximum number of recent issues permitted.
 */
class RecentIssues : public QObject
{
    Q_OBJECT
private:
    static const int mMaxIssueCount;
    QList<QSharedPointer<JiraIssue>> mIssues;
    Settings mSettings;
public:
    explicit RecentIssues();
    /**
     * @brief addIssue Register an issue as recently used
     * @param issue
     */
    void addIssue(QSharedPointer<JiraIssue> issue);
    /**
     * @brief issues Get the recent issues
     * @return
     */
    QList<QSharedPointer<JiraIssue>> issues() const;
signals:

};

#endif // RECENTISSUES_H
