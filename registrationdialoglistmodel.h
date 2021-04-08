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
    QList<QSharedPointer<AbstractIssue>> mRecentIssues;
    QList<QSharedPointer<AbstractIssue>> mSearchIssues;
    QSharedPointer<AbstractIssue> issueByRow(int row) const;
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
    void setRecentIssues(QList<QSharedPointer<AbstractIssue> > issues);
    void setSearchIssues(QList<QSharedPointer<AbstractIssue> > issues);
};

#endif // REGISTRATIONDIALOGLISTMODEL_H
