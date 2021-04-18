#include "recentissues.h"

const int RecentIssues::mMaxIssueCount = 5;

RecentIssues::RecentIssues() : QObject()
{
    mIssues = mSettings.recentIssues();
}

void RecentIssues::addIssue(QSharedPointer<AbstractIssue> issue)
{
    // Ensure that we don't have duplicates
    QList<int> indices;
    for (int i = 0; i < mIssues.length(); i++) {
        if (issue->id() == mIssues.at(i)->id()) {
            indices.append(i);
        }
    }
    foreach (int index, indices) {
        mIssues.removeAt(index);
    }
    // Add the issue
    mIssues.push_front(issue);
    // Trim list of issues to configured max number of recent issues
    if (mIssues.length() > mMaxIssueCount) {
        mIssues.removeLast();
    }
    mSettings.setRecentIssues(mIssues);
}

QList<QSharedPointer<AbstractIssue> > RecentIssues::issues() const
{
    return mIssues;
}
