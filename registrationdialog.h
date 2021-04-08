#ifndef REGISTRATIONDIALOG_H
#define REGISTRATIONDIALOG_H

#include <QDialog>
#include <QTime>
#include <QTimer>
#include <QSharedPointer>
#include <QItemSelection>
#include <QItemSelectionModel>
#include <QModelIndex>
#include "registrationdialoglistmodel.h"
#include "jira/jiraissue.h"
#include "jira/jiraclient.h"
#include "registrationdialoglistvievitemdelegate.h"

namespace Ui {
class RegistrationDialog;
}

class RegistrationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegistrationDialog(QWidget *parent = nullptr);
    ~RegistrationDialog();

    QTime startTime() const;
    void setStartTime(const QTime startTime);
    QTime endTime() const;
    void setEndTime(const QTime endTime);
    void setDate(const QDate date);
    QSharedPointer<AbstractIssue> jiraIssue();
    void setRecentIssues(QList<QSharedPointer<AbstractIssue> > issues);

private:
    Ui::RegistrationDialog *ui;
    QTimer mSearchTimer;
    QTime mStartTime;
    QTime mEndTime;
    QDate mDate;
    QList<QSharedPointer<AbstractIssue>> mJiraIssues;
    QList<QSharedPointer<AbstractIssue>> mRecentIssues;
    QSharedPointer<AbstractIssue> mJiraIssue;
    RegistrationDialogListModel mModel;
    JiraClient mJiraClient;
    RegistrationDialogListVievItemDelegate *mItemDelegate;
    void setupConnections();
    void setupWorklogInformationLabel();
    void populateModel();
    QSharedPointer<AbstractIssue> issueByModelIndex(int index);
private slots:
    void searchLineEditTextChanged(const QString &text);
    void search();
    void jiraClientSearchFinished(QList<QSharedPointer<AbstractIssue> > issues);
    void listViewIndexesMoved(const QItemSelection &selected, const QItemSelection &deselected);
    void listViewDoubleClicked(const QModelIndex &selected);
    QString issueToString(QSharedPointer<AbstractIssue> issue);
};

#endif // REGISTRATIONDIALOG_H
