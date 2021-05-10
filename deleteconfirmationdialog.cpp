#include "deleteconfirmationdialog.h"
#include "ui_deleteconfirmationdialog.h"

DeleteConfirmationDialog::DeleteConfirmationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DeleteConfirmationDialog)
{
    ui->setupUi(this);
}

DeleteConfirmationDialog::~DeleteConfirmationDialog()
{
    delete ui;
}

void DeleteConfirmationDialog::setWorklog(QSharedPointer<JiraWorklog> worklog)
{
    // If for some reason we don't have the issue cached
    // fall back to using the issue id when showing the dialog
    QString issueIdentifier = worklog->id();
    PAbstractIssue issue = mIssueCache.getById(worklog->issueId());
    if (!issue.isNull())
    {
        issueIdentifier = QString("%1: %2").arg(issue->key(), issue->summary());
    }
    mWorklog = worklog;
    ui->messageLabel->setText(QString("Really delete worklog from %1 to %2 for issue %3")
                                  .arg(worklog->started().toString())
                                  .arg(worklog->started().addSecs(worklog->timeSpentSeconds()).toString())
                                  .arg(issueIdentifier));
}

QSharedPointer<JiraWorklog> DeleteConfirmationDialog::worklog() const
{
    return mWorklog;
}
