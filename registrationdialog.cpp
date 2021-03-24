#include "registrationdialog.h"
#include "ui_registrationdialog.h"
#include <QLineEdit>
#include <QPushButton>
#include <QRegularExpression>

RegistrationDialog::RegistrationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegistrationDialog)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    mModel.setParent(ui->searchResultsListView);
    ui->searchResultsListView->setModel(&mModel);
    mItemDelegate = new RegistrationDialogListVievItemDelegate();
    ui->searchResultsListView->setItemDelegate(mItemDelegate);
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
    setupWorklogInformationLabel();
}

void RegistrationDialog::setDate(const QDate date)
{
    mDate = date;
    setupWorklogInformationLabel();
}

QSharedPointer<JiraIssue> RegistrationDialog::jiraIssue()
{
    return mJiraIssue;
}

void RegistrationDialog::populateModel() {
    mModel.clear();
    mModel.setRecentIssues(mRecentIssues);
    mModel.setSearchIssues(mJiraIssues);
}

void RegistrationDialog::setRecentIssues(QList<QSharedPointer<JiraIssue> > issues)
{
    mRecentIssues = issues;
    populateModel();
}

void RegistrationDialog::setupConnections()
{
    QObject::connect(&mSearchTimer, &QTimer::timeout,
                     this, &RegistrationDialog::searchTimerTimeout);
    QObject::connect(ui->searchLineEdit, &QLineEdit::textEdited,
                     this, &RegistrationDialog::searchLineEditTextChanged);
    QObject::connect(&mJiraClient, &JiraClient::searchFinished,
                     this, &RegistrationDialog::jiraClientSearchFinished);
    QObject::connect(ui->searchResultsListView->selectionModel(), &QItemSelectionModel::selectionChanged,
                     this, &RegistrationDialog::listViewIndexesMoved);
}

void RegistrationDialog::setupWorklogInformationLabel() {
    ui->worklogInfoLabel->setText(QString("New worklog from %2 to %3").arg(mStartTime.toString(), mEndTime.toString()));
}

void RegistrationDialog::searchLineEditTextChanged(const QString &text)
{
    Q_UNUSED(text);
    mSearchTimer.start(1000);
}

void RegistrationDialog::searchTimerTimeout()
{
    ui->searchLineEdit->setEnabled(false);
    QString terms = ui->searchLineEdit->text();
    QString query;
    QRegularExpression issueKeyRegExp("^[A-Za-z]{2,4}\\-[0-9]{1,10}$");
    if (issueKeyRegExp.match(terms).hasMatch()) {
        query = QString("issuekey = \"%1\"").arg(terms);
    } else {
        query = QString("text ~ \"%1\" ORDER BY updated, created DESC").arg(terms);
    }
    mJiraClient.search(query);
    mModel.clear();
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

void RegistrationDialog::jiraClientSearchFinished(QList<QSharedPointer<JiraIssue> > issues)
{
    ui->searchLineEdit->setDisabled(false);    
    mJiraIssues = issues;
    populateModel();
}

QSharedPointer<JiraIssue> RegistrationDialog::issueByModelIndex(int index) {
    if (index < mRecentIssues.length()) {
        return mRecentIssues.at(index);
    }
    if (index < mRecentIssues.length() + mJiraIssues.length()) {
        return mJiraIssues.at(index - mRecentIssues.length());
    }
    return nullptr;
}

void RegistrationDialog::listViewIndexesMoved(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected);
    // Top index of first selection
    if (selected.length() == 0) {
        mJiraIssue = nullptr;
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    } else {
        int index = selected.first().top();
        mJiraIssue = issueByModelIndex(index);
        if (mJiraIssue.isNull()) {
            qWarning() << "[RegistrationDialog] Failed to look up issue at index" << index;
        }
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
}

QString RegistrationDialog::issueToString(QSharedPointer<JiraIssue> issue)
{
    return QString("%1 %2").arg(issue->key(), issue->summary());
}

