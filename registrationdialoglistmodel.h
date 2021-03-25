#ifndef REGISTRATIONDIALOGLISTMODEL_H
#define REGISTRATIONDIALOGLISTMODEL_H

#include <QAbstractListModel>
#include <QSharedPointer>
#include <QModelIndex>
#include <QList>
#include "jira/jiraissue.h"

class RegistrationDialogListModel : public QAbstractListModel
{
private:
    QList<QSharedPointer<JiraIssue>> mRecentIssues;
    QList<QSharedPointer<JiraIssue>> mSearchIssues;
    QSharedPointer<JiraIssue> issueByRow(int row) const;
public:
    enum RegistrationType {
        RecentIssue,
        SearchIssue
    };
    RegistrationDialogListModel();
    QVariant data(const QModelIndex &index, int role) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    void clear();
    void setRecentIssues(QList<QSharedPointer<JiraIssue>> issues);
    void setSearchIssues(QList<QSharedPointer<JiraIssue>> issues);
};

#endif // REGISTRATIONDIALOGLISTMODEL_H
