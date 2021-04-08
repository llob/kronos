#include "recentissues.h"

const int RecentIssues::mMaxIssueCount = 5;

RecentIssues::RecentIssues() : QObject()
{
    mIssues = mSettings.recentIssues();
}

void RecentIssues::addIssue(QSharedPointer<AbstractIssue> issue)
{
    mIssues.removeAll(issue);
    mIssues.push_front(issue);
    if (mIssues.length() > mMaxIssueCount) {
        mIssues.removeLast();
    }
    mSettings.setRecentIssues(mIssues);
}

QList<QSharedPointer<AbstractIssue> > RecentIssues::issues() const
{
    return mIssues;
}
