#include "registrationdialog.h"
#include "ui_registrationdialog.h"
#include <QLineEdit>
#include <QPushButton>

RegistrationDialog::RegistrationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegistrationDialog)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    ui->searchResultsListView->setModel(&mModel);
    mSearchTimer.setSingleShot(true);
    setupConnections();
}

RegistrationDialog::~RegistrationDialog()
{
    delete ui;
}

QTime RegistrationDialog::startTime() const
{
    return mStartTime;
}

void RegistrationDialog::setStartTime(const QTime startTime)
{
    mStartTime = startTime;
}

QTime RegistrationDialog::endTime() const
{
    return mEndTime;
}

void RegistrationDialog::setEndTime(const QTime endTime)
{
    mEndTime = endTime;
}

QSharedPointer<JiraIssue> RegistrationDialog::jiraIssue()
{
    return mJiraIssue;
}

void RegistrationDialog::setupConnections()
{
    QObject::connect(&mSearchTimer, &QTimer::timeout,
                     this, &RegistrationDialog::searchTimerTimeout);
    QObject::connect(ui->searchLineEdit, &QLineEdit::textEdited,
                     this, &RegistrationDialog::issueComboBoxEditTextChanged);
    QObject::connect(&mJiraClient, &JiraClient::searchFinished,
                     this, &RegistrationDialog::jiraClientSearchFinished);
    QObject::connect(ui->searchResultsListView->selectionModel(), &QItemSelectionModel::selectionChanged,
                     this, &RegistrationDialog::listViewIndexesMoved);
}

void RegistrationDialog::issueComboBoxEditTextChanged(const QString &text)
{
    mSearchTimer.start(1000);
}

void RegistrationDialog::searchTimerTimeout()
{
    ui->searchLineEdit->setEnabled(false);
    mJiraClient.search(QString("text ~ \"%1\"").arg(ui->searchLineEdit->text()));
    mModel.setStringList({});
}

void RegistrationDialog::jiraClientSearchFinished(QList<QSharedPointer<JiraIssue> > issues)
{
    ui->searchLineEdit->setDisabled(false);
    QStringList issueKeys;
    foreach (QSharedPointer<JiraIssue> issue, issues) {
        issueKeys.append(QString("%1 %2").arg(issue->key(), issue->summary()));
    }
    mJiraIssues = issues;
    mModel.setStringList(issueKeys);
}

void RegistrationDialog::listViewIndexesMoved(const QItemSelection &selected, const QItemSelection &deselected)
{
    // Top index of first selection
    if (selected.length() == 0) {
        mJiraIssue = 0;
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    } else {
        mJiraIssue = mJiraIssues.at(selected.first().top());
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
}

