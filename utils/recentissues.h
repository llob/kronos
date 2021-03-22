#ifndef RECENTISSUES_H
#define RECENTISSUES_H

#include <QObject>
#include <QSharedPointer>
#include <QMap>
#include <QList>
#include "jira/jiraissue.h"

class RecentIssues : public QObject
{
    Q_OBJECT
private:
    static const int mMaxIssueCount;
    QList<QSharedPointer<JiraIssue>> mIssues;
public:
    explicit RecentIssues(QObject *parent = nullptr);
    void addIssue(QSharedPointer<JiraIssue> issue);
    QList<QSharedPointer<JiraIssue>> issues() const;
signals:

};

#endif // RECENTISSUES_H
