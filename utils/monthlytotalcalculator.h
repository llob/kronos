#ifndef MONTHLYTOTALCALCULATOR_H
#define MONTHLYTOTALCALCULATOR_H

#include <QObject>
#include <QList>
#include <QSharedPointer>
#include "jira/jiraclient.h"
#include "jira/jiraworklog.h"
#include "jira/jiraissue.h"
#include "settings/settings.h"

/**
 * @brief The MonthlyTotalCalculator class continuously calculates
 *  the total number of hours logged for the current month
 */
class MonthlyTotalCalculator : public QObject
{
    Q_OBJECT
private:
    JiraClient mJiraClient;
    int mTotal;
    Settings mSettings;
    QDate firstMonthDay() const;
public:
    explicit MonthlyTotalCalculator();
    /**
     * @brief update Recalculate month total
     */
    void update();
signals:
    /**
     * @brief updated Emitted when the monthly total has been recalculated
     * @param seconds Number of seconds logged for current month
     */
    void updated(int seconds);
private slots:
    void jiraClientSearchFinished(QList<QSharedPointer<JiraIssue>> issues);
    void jiraClientIssueWorklogsFinished(QList<QSharedPointer<JiraWorklog>> worklogs);
};

#endif // MONTHLYTOTALCALCULATOR_H
