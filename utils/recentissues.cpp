#include "recentissues.h"

const int RecentIssues::mMaxIssueCount = 10;

RecentIssues::RecentIssues(QObject *parent) : QObject(parent)
{
    mIssues = mSettings.recentIssues();
}

void RecentIssues::addIssue(QSharedPointer<JiraIssue> issue)
{
    mIssues.removeAll(issue);
    mIssues.push_front(issue);
    if (mIssues.length() > mMaxIssueCount) {
        mIssues.removeLast();
    }
    mSettings.setRecentIssues(mIssues);
}

QList<QSharedPointer<JiraIssue> > RecentIssues::issues() const
{
    return mIssues;
}
