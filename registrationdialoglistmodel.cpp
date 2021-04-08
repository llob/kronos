#include "registrationdialoglistmodel.h"

RegistrationDialogListModel::RegistrationDialogListModel()
{

}

QSharedPointer<AbstractIssue> RegistrationDialogListModel::issueByRow(int row) const {
    if (row < mRecentIssues.length()) {
        return mRecentIssues.at(row);;
    } else {
        return mSearchIssues.at(row - mRecentIssues.length());
    }
}

QVariant RegistrationDialogListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }
    QSharedPointer<AbstractIssue> issue = issueByRow(index.row());
    if (role == Qt::UserRole) {
        QVariantMap result;
        result.insert("issue", issue->toVariant());
        if (index.row() < mRecentIssues.length()) {
            result.insert("type", RegistrationDialogListModel::RecentIssue);
        } else {
            result.insert("type", RegistrationDialogListModel::SearchIssue);
        }
        return result;
    }
    if (role == Qt::DisplayRole) {
        return issue->key();
    }
    return QVariant();
}

int RegistrationDialogListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mRecentIssues.length() + mSearchIssues.length();
}

int RegistrationDialogListModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

void RegistrationDialogListModel::clear()
{
    beginResetModel();
    mSearchIssues.clear();
    mRecentIssues.clear();
    endResetModel();
}

void RegistrationDialogListModel::setSearchIssues(QList<QSharedPointer<AbstractIssue> > issues)
{
    beginResetModel();
    mSearchIssues = issues;
    endResetModel();
}

void RegistrationDialogListModel::setRecentIssues(QList<QSharedPointer<AbstractIssue> > issues)
{
    beginResetModel();
    mRecentIssues = issues;
    endResetModel();
}
