#ifndef WORKLOGDIALOG_H
#define WORKLOGDIALOG_H

#include <QDialog>
#include <QTime>
#include <QTimer>
#include <QSharedPointer>
#include <QItemSelection>
#include <QItemSelectionModel>
#include <QModelIndex>
#include "worklogdialoglistmodel.h"
#include "jira/jiraissue.h"
#include "jira/jiraclient.h"
#include "worklogdialoglistvievitemdelegate.h"

namespace Ui {
class WorklogDialog;
}

class WorklogDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WorklogDialog(QWidget *parent = nullptr);
    ~WorklogDialog();

    QTime startTime() const;
    void setStartTime(const QTime startTime);
    QTime endTime() const;
    void setEndTime(const QTime endTime);
    void setDate(const QDate date);
    QSharedPointer<AbstractIssue> jiraIssue();
    void setRecentIssues(QList<QSharedPointer<AbstractIssue> > issues);

private:
    Ui::WorklogDialog *ui;
    QTimer mSearchTimer;
    QTime mStartTime;
    QTime mEndTime;
    QDate mDate;
    QList<QSharedPointer<AbstractIssue>> mJiraIssues;
    QList<QSharedPointer<AbstractIssue>> mRecentIssues;
    QSharedPointer<AbstractIssue> mJiraIssue;
    WorklogDialogListModel mModel;
    JiraClient mJiraClient;
    WorklogDialogListVievItemDelegate *mItemDelegate;
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

#endif // WORKLOGDIALOG_H
