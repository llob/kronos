#include "worklogdialoglistmodel.h"

WorklogDialogListModel::WorklogDialogListModel()
{

}

QSharedPointer<AbstractIssue> WorklogDialogListModel::issueByRow(int row) const {
    if (row < mRecentIssues.length()) {
        return mRecentIssues.at(row);;
    } else {
        return mSearchIssues.at(row - mRecentIssues.length());
    }
}

QVariant WorklogDialogListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }
    QSharedPointer<AbstractIssue> issue = issueByRow(index.row());
    if (role == Qt::UserRole) {
        QVariantMap result;
        result.insert("issue", issue->toVariant());
        if (index.row() < mRecentIssues.length()) {
            result.insert("type", WorklogDialogListModel::RecentIssue);
        } else {
            result.insert("type", WorklogDialogListModel::SearchIssue);
        }
        return result;
    }
    if (role == Qt::DisplayRole) {
        return issue->key();
    }
    return QVariant();
}

int WorklogDialogListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mRecentIssues.length() + mSearchIssues.length();
}

int WorklogDialogListModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

void WorklogDialogListModel::clear()
{
    beginResetModel();
    mSearchIssues.clear();
    mRecentIssues.clear();
    endResetModel();
}

void WorklogDialogListModel::setSearchIssues(QList<QSharedPointer<AbstractIssue> > issues)
{
    beginResetModel();
    mSearchIssues = issues;
    endResetModel();
}

void WorklogDialogListModel::setRecentIssues(QList<QSharedPointer<AbstractIssue> > issues)
{
    beginResetModel();
    mRecentIssues = issues;
    endResetModel();
}
