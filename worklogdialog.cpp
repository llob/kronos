#include "worklogdialog.h"
#include "ui_worklogdialog.h"
#include <QLineEdit>
#include <QPushButton>
#include <QRegularExpression>

WorklogDialog::WorklogDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WorklogDialog)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    mModel.setParent(ui->searchResultsListView);
    ui->searchResultsListView->setModel(&mModel);
    mItemDelegate = new WorklogDialogListVievItemDelegate();
    ui->searchResultsListView->setItemDelegate(mItemDelegate);
    mSearchTimer.setSingleShot(true);
    setupConnections();
}

WorklogDialog::~WorklogDialog()
{
    delete ui;
}

QTime WorklogDialog::startTime() const
{
    return mStartTime;
}

void WorklogDialog::setStartTime(const QTime startTime)
{
    mStartTime = startTime;
}

QTime WorklogDialog::endTime() const
{
    return mEndTime;
}

void WorklogDialog::setEndTime(const QTime endTime)
{
    mEndTime = endTime;
    setupWorklogInformationLabel();
}

void WorklogDialog::setDate(const QDate date)
{
    mDate = date;
    setupWorklogInformationLabel();
}

QSharedPointer<AbstractIssue> WorklogDialog::jiraIssue()
{
    return mJiraIssue;
}

void WorklogDialog::populateModel() {
    mModel.clear();
    mModel.setRecentIssues(mRecentIssues);
    mModel.setSearchIssues(mJiraIssues);
}

void WorklogDialog::setRecentIssues(QList<QSharedPointer<AbstractIssue> > issues)
{
    mRecentIssues = issues;
    populateModel();
}

void WorklogDialog::setupConnections()
{
    QObject::connect(&mSearchTimer, &QTimer::timeout,
                     this, &WorklogDialog::search);
    QObject::connect(ui->searchLineEdit, &QLineEdit::textEdited,
                     this, &WorklogDialog::searchLineEditTextChanged);
    QObject::connect(&mJiraClient, &JiraClient::searchFinished,
                     this, &WorklogDialog::jiraClientSearchFinished);
    QObject::connect(ui->searchResultsListView->selectionModel(), &QItemSelectionModel::selectionChanged,
                     this, &WorklogDialog::listViewIndexesMoved);
    QObject::connect(ui->searchResultsListView, &QListView::doubleClicked,
                     this, &WorklogDialog::listViewDoubleClicked);
    QObject::connect(ui->searchLineEdit, &QLineEdit::returnPressed,
                     this, &WorklogDialog::search);
}

void WorklogDialog::setupWorklogInformationLabel() {
    ui->worklogInfoLabel->setText(QString("New worklog from %2 to %3").arg(mStartTime.toString(), mEndTime.toString()));
}

void WorklogDialog::searchLineEditTextChanged(const QString &text)
{
    Q_UNUSED(text);
    mSearchTimer.start(1000);
}

void WorklogDialog::search()
{
    // In case the search was started by a timer timeout, we must
    // ensure that we don't fire off another search, by
    // unconditionally killing the search timer
    mSearchTimer.stop();

    ui->searchLineEdit->setEnabled(false);
    QString terms = ui->searchLineEdit->text();
    QString query;
    QRegularExpression issueKeyRegExp("^[A-Za-z]{2,4}\\-[0-9]{1,10}$");
    if (terms.isEmpty()) {
        // Grab issues assigned to the current user in the current sprint
        query = QString("assignee = currentUser() AND Sprint in openSprints () AND statusCategory in (\"In Progress\", \"To Do\") ORDER BY updated, created DESC");
    } else if (issueKeyRegExp.match(terms).hasMatch()) {
        // Looks like a search for a specific issue key
        query = QString("issuekey = \"%1\"").arg(terms);
    } else {
        // Generic search
        query = QString("text ~ \"%1\" AND statusCategory in (\"In Progress\", \"To Do\") ORDER BY updated, created DESC").arg(terms);
    }
    mJiraClient.search(query, 0, 30);
    mModel.clear();
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

void WorklogDialog::jiraClientSearchFinished(QList<QSharedPointer<AbstractIssue> > issues)
{
    ui->searchLineEdit->setDisabled(false);
    mJiraIssues = issues;
    populateModel();
}

QSharedPointer<AbstractIssue> WorklogDialog::issueByModelIndex(int index) {
    if (index < mRecentIssues.length()) {
        return mRecentIssues.at(index);
    }
    if (index < mRecentIssues.length() + mJiraIssues.length()) {
        return mJiraIssues.at(index - mRecentIssues.length());
    }
    return nullptr;
}

void WorklogDialog::listViewIndexesMoved(const QItemSelection &selected, const QItemSelection &deselected)
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

void WorklogDialog::listViewDoubleClicked(const QModelIndex &selected)
{
    mJiraIssue = issueByModelIndex(selected.row());
    accept();

}

QString WorklogDialog::issueToString(QSharedPointer<AbstractIssue> issue)
{
    return QString("%1 %2").arg(issue->key(), issue->summary());
}

