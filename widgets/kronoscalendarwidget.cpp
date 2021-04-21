#include "kronoscalendarwidget.h"
#include <QPainter>
#include "utils/colors.h"
#include "jira/jirautils.h"

void KronosCalendarWidget::updateWorklogData(QDate d)
{
    QDate startDate(d.year(), d.month(), 1);
    QDate endDate(d.year(), d.month(), d.daysInMonth());
    QString query = QString("worklogDate >= %1 AND worklogDate <= %2 AND worklogAuthor = %3 order by created DESC")
                        .arg(JiraClient::jqlDate(startDate))
                        .arg(JiraClient::jqlDate(endDate))
                        .arg(mSettings.accountId());
    mJiraClient.search(query);
    clearMonthlyWorklogData(d);
}

void KronosCalendarWidget::jiraClientSearchFinished(QList<QSharedPointer<AbstractIssue> > issues)
{
    foreach (QSharedPointer<AbstractIssue> issue, issues) {
        mJiraClient.issueWorklogs(issue);
    }
}

void KronosCalendarWidget::jiraClientIssueWorklogsFinished(QList<QSharedPointer<JiraWorklog> > worklogs)
{
    bool updated = false;
    QString accountId = mSettings.accountId();
    foreach (QSharedPointer<JiraWorklog> worklog, worklogs) {
        if (worklog->accountId() != accountId) {
            continue;
        }
        QMap<QString, PJiraWorklog> daysWorklogs = dailyWorklogs(worklog->started().date());
        daysWorklogs.insert(worklog->id(), worklog);
        setDailyWorklogs(worklog->started().date(), daysWorklogs);
        updated = true;
    }
    if (updated) {
        updateCells();
    }
}

void KronosCalendarWidget::calendarWidgetCurrentPageChanged(int year, int month)
{
    updateWorklogData(QDate(year, month, 1));
}

void KronosCalendarWidget::calendarWidgetSelectionChanged()
{
    updateCells();
}

void KronosCalendarWidget::authenticationStateChanged(AuthenticationState::State oldState, AuthenticationState::State newState, const QString message)
{
    Q_UNUSED(oldState);
    Q_UNUSED(message);
    if (newState == AuthenticationState::DEAUTHENTICATED) {
        mWorklogs.clear();
        updateCells();
    } else if (newState == AuthenticationState::AUTHENTICATED) {
        updateWorklogData(selectedDate());
    }
}

KronosCalendarWidget::KronosCalendarWidget() : QCalendarWidget()
{

    QString ss = "#qt_calendar_prevmonth {"
                 "  background-color: rgb(255, 67, 101);"
                 "  icon-size: 30px;"
                 "  qproperty-icon: url(:/resources/chevron_left.svg);"
                 "}"
                 "#qt_calendar_nextmonth {"
                 "  background-color: rgb(255, 67, 101);"
                 "  icon-size: 30px;"
                 "  qproperty-icon: url(:/resources/chevron_right.svg);"
                 "}"
                 "QCalendarWidget QWidget#qt_calendar_navigationbar {"
                 "  background-color: rgb(255, 67, 101);"
                 "}"
                 "#qt_calendar_calendarview"
                 "{"
                 "  selection-background-color: rgb(183, 173, 153);"
                 "  selection-color: rgb(3, 3, 1);"
                 "}"
                 "#kronos_calendar_today_button"
                 "{"
                 "  background-color: rgb(255, 255, 255);"
                 "  background: white;"
                 "  color: rgb(0, 217, 192);"
                 "}";
    setStyleSheet(ss);
    setupConnections();
    setupCurrentDateButton();
    setSelectedDate(QDate::currentDate());
    updateWorklogData(selectedDate());
}

void KronosCalendarWidget::setupCurrentDateButton()
{
    // Get layout from navigation bar in QCalendarWidget
    QHBoxLayout *l = qobject_cast<QHBoxLayout*>(this->findChild<QWidget*>("qt_calendar_navigationbar")->layout());
    // Find an existing button to copy its height
    auto buttonHeight = this->findChild<QWidget*>("qt_calendar_prevmonth")->height();

    // Create and style button
    auto button = new QPushButton("Today");
    button->setFlat(true);
    button->setObjectName("kronos_calendar_today_button");
    button->setMinimumHeight(buttonHeight);
    QObject::connect(button, &QPushButton::clicked,
        [this] {
            this->showToday();
            this->setSelectedDate(QDate::currentDate());
        });

    // Add it to the layout, right after
    // previous month button
    l->insertWidget(1, button);
}

void KronosCalendarWidget::setupConnections()
{
    QObject::connect(this, &QCalendarWidget::selectionChanged,
                     this, &KronosCalendarWidget::calendarWidgetSelectionChanged);
    QObject::connect(this, &KronosCalendarWidget::currentPageChanged,
                     this, &KronosCalendarWidget::calendarWidgetCurrentPageChanged);
    QObject::connect(&mJiraClient, &JiraClient::searchFinished,
                     this, &KronosCalendarWidget::jiraClientSearchFinished);
    QObject::connect(&mJiraClient, &JiraClient::issueWorklogsFinished,
                     this, &KronosCalendarWidget::jiraClientIssueWorklogsFinished);
    QObject::connect(&mAuthenticationState, &AuthenticationState::stateChanged,
                     this, &KronosCalendarWidget::authenticationStateChanged);
}

QMap<QString, PJiraWorklog> KronosCalendarWidget::dailyWorklogs(QDate date) const
{
    auto months = mWorklogs.value(date.year());
    auto days = months.value(date.month());
    auto daysWorklogs = days.value(date.day());

    return daysWorklogs;
}

void KronosCalendarWidget::setDailyWorklogs(QDate date, QMap<QString, PJiraWorklog> worklogs)
{
    auto months = mWorklogs.value(date.year());
    auto days = months.value(date.month());
    days.insert(date.day(), worklogs);
    months.insert(date.month(), days);
    mWorklogs.insert(date.year(), months);
}

void KronosCalendarWidget::clearMonthlyWorklogData(const QDate date)
{
    auto months = mWorklogs.value(date.year());
    months.remove(date.month());
    mWorklogs.insert(date.year(), months);
}

void KronosCalendarWidget::paintCell(QPainter *painter, const QRect &rect, const QDate &date) const
{
    QMap<QString, PJiraWorklog> dw = dailyWorklogs(date);
    int dailySeconds = 0;
    foreach (PJiraWorklog worklog, dw.values()) {
        dailySeconds += worklog->timeSpentSeconds();
    }
    float dailyHours = dailySeconds / 3600.0;

    QRect r = rect.adjusted(rect.width()/1.5, 0, 0, 0);
    QFont f = painter->font();
    f.setPixelSize(8);

    QCalendarWidget::paintCell(painter, rect, date);

    if (date.dayOfWeek() > 5) {
        painter->setPen(Colors::khakiWeb());
    } else {
        if (dailyHours < 7) {
            painter->setPen(Colors::radicalRed());
        } else {
            painter->setPen(Colors::turquise());
        }
    }
    painter->setFont(f);
    painter->drawText(r, Qt::AlignCenter, QString("(%1)").arg(dailyHours, 2));
}

