#ifndef DELETECONFIRMATIONDIALOG_H
#define DELETECONFIRMATIONDIALOG_H

#include <QDialog>
#include <QSharedPointer>
#include "jira/jiraworklog.h"

namespace Ui {
class DeleteConfirmationDialog;
}

class DeleteConfirmationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DeleteConfirmationDialog(QWidget *parent = nullptr);
    ~DeleteConfirmationDialog();
    void setWorklog(QSharedPointer<JiraWorklog> worklog);
    QSharedPointer<JiraWorklog> worklog() const;
private:
    Ui::DeleteConfirmationDialog *ui;
    QSharedPointer<JiraWorklog> mWorklog;
};

#endif // DELETECONFIRMATIONDIALOG_H
