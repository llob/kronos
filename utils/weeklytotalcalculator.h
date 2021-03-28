#ifndef WEEKLYTOTALCALCULATOR_H
#define WEEKLYTOTALCALCULATOR_H

#include <QObject>
#include <QList>
#include <QSharedPointer>
#include "jira/jiraclient.h"
#include "jira/jiraworklog.h"
#include "jira/jiraissue.h"
#include "settings/settings.h"

/**
 * @brief The WeeklyTotalCalculator class continuous calculates the time
 *  logged during the current week.
 */
class WeeklyTotalCalculator : public QObject
{
    Q_OBJECT
private:
    JiraClient mJiraClient;
    int mTotal;
    QDate firstWeekDay();
    Settings mSettings;
public:
    explicit WeeklyTotalCalculator();
    /**
     * @brief update Recalculate the weekly total
     */
    void update();
signals:
    /**
     * @brief updated Emitted whenever the weekly total has been recalculated
     * @param seconds The number of seconds logged for the current week
     */
    void updated(int seconds);
private slots:
    void jiraClientSearchFinished(QList<QSharedPointer<JiraIssue>> issues);
    void jiraClientIssueWorklogsFinished(QList<QSharedPointer<JiraWorklog>> worklogs);
};

#endif // WEEKLYTOTALCALCULATOR_H
