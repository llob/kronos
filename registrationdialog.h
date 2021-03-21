#ifndef REGISTRATIONDIALOG_H
#define REGISTRATIONDIALOG_H

#include <QDialog>
#include <QTime>
#include <QTimer>
#include <QSharedPointer>
#include <QItemSelection>
#include <QItemSelectionModel>
#include <QStringListModel>
#include "jira/jiraissue.h"
#include "jira/jiraclient.h"

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
    QSharedPointer<JiraIssue> jiraIssue();

private:
    Ui::RegistrationDialog *ui;
    QTimer mSearchTimer;
    QTime mStartTime;
    QTime mEndTime;
    QDate mDate;
    QList<QSharedPointer<JiraIssue>> mJiraIssues;
    QSharedPointer<JiraIssue> mJiraIssue;
    QStringListModel mModel;
    JiraClient mJiraClient;
    void setupConnections();
    void setupWorklogInformationLabel();
private slots:
    void searchLineEditTextChanged(const QString &text);
    void searchTimerTimeout();
    void jiraClientSearchFinished(QList<QSharedPointer<JiraIssue>> issues);
    void listViewIndexesMoved(const QItemSelection &selected, const QItemSelection &deselected);
};

#endif // REGISTRATIONDIALOG_H
