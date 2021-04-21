#ifndef KRONOSCALENDARWIDGET_H
#define KRONOSCALENDARWIDGET_H

#include <QCalendarWidget>
#include <QMap>
#include <QDate>
#include <jira/jiraclient.h>
#include "settings/settings.h"
#include "jira/jiraworklog.h"
#include "authentication/authenticationstate.h"

/**
 * @brief The KronosCalendarWidget class inherits from QCalendarWidget
 *  and adds per-day information about number of logged hours.
 */
class KronosCalendarWidget : public QCalendarWidget
{
    Q_OBJECT
private:
    JiraClient mJiraClient;
    /**
     * @brief mWorklogs Retrieved worklogs, indexed by
     *  year -> month -> day -> id
     *  Implemented like this for ease of per-month updates.
     */
    QMap<int, QMap<int, QMap<int, QMap<QString, PJiraWorklog>>>> mWorklogs;
    AuthenticationState mAuthenticationState;
    Settings mSettings;
    void setupConnections();
    QMap<QString, PJiraWorklog> dailyWorklogs(QDate date) const;
    void setDailyWorklogs(QDate date, QMap<QString, PJiraWorklog> worklogs);
    void clearMonthlyWorklogData(const QDate date);
    void setupCurrentDateButton();
private slots:
    void jiraClientSearchFinished(QList<QSharedPointer<AbstractIssue>> issues);
    void jiraClientIssueWorklogsFinished(QList<QSharedPointer<JiraWorklog>> worklogs);
    void calendarWidgetCurrentPageChanged(int year, int month);
    void calendarWidgetSelectionChanged();
    void authenticationStateChanged(AuthenticationState::State oldState, AuthenticationState::State newState, const QString message);
public:
    KronosCalendarWidget();
public slots:
    /**
     * @brief updateWorklogData causes this instance to discard worklog
     *  data for the month surrounding specified date, and redownload from
     *  backend.
     * @param d
     */
    void updateWorklogData(QDate d);
protected:
    void paintCell(QPainter *painter, const QRect &rect, const QDate &date) const override;
};

#endif // KRONOSCALENDARWIDGET_H
