#include "recentissues.h"

const int RecentIssues::mMaxIssueCount = 10;

RecentIssues::RecentIssues(QObject *parent) : QObject(parent)
{

}

void RecentIssues::addIssue(QSharedPointer<JiraIssue> issue)
{
    mIssues.removeAll(issue);
    mIssues.push_front(issue);
    if (mIssues.length() > mMaxIssueCount) {
        mIssues.removeLast();
    }
    foreach (QSharedPointer<JiraIssue> issue, mIssues) {
        qDebug() << "[RecentIssues]" << issue->key();
    }
}

QList<QSharedPointer<JiraIssue> > RecentIssues::issues() const
{
    return mIssues;
}
