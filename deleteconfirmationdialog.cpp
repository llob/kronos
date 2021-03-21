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
    mWorklog = worklog;
    ui->messageLabel->setText(QString("Really delete worklog from %1 to %2 for issue %3")
                                  .arg(worklog->started().toString())
                                  .arg(worklog->started().addSecs(worklog->timeSpentSeconds()).toString())
                                  .arg(worklog->issueId()));
}

QSharedPointer<JiraWorklog> DeleteConfirmationDialog::worklog() const
{
    return mWorklog;
}
