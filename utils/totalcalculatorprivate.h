#ifndef TOTALCALCULATORPRIVATE_H
#define TOTALCALCULATORPRIVATE_H

#include <QObject>
#include "jira/jiraclient.h"
#include "settings/settings.h"

/**
 * @brief The TotalCalculatorPrivate class Internal class used by
 *  the TotalCalculator public class.
 *  Instances of this class perform the heavy lifting in terms of
 *  talking to the backend and calculating totals.
 *  Instances of this class are used to ensure consistent calculation of
 *  totals, which may require multiple roundtrips to the backend and
 *  intermediate calculations.
 *
 *  Instances of this class delete itself (using deleteLater()) once
 *  it has emitted its updated() signal once.
 */
class TotalCalculatorPrivate : public QObject
{
    Q_OBJECT
private:
    int mTotal;
    QDate mFirstDate;
    QString mQuery;
    Settings mSettings;
    JiraClient mJiraClient;
    int mIssueWorklogRequestCount;
public:
    explicit TotalCalculatorPrivate(QString query, QDate firstDate);
    virtual ~TotalCalculatorPrivate();
    void calculate();
signals:
    void updated(int seconds);
private slots:
    void jiraClientIssueWorklogsFinished(QList<QSharedPointer<JiraWorklog> > worklogs);
    void jiraClientSearchFinished(QList<QSharedPointer<AbstractIssue> > issues);
};

#endif // TOTALCALCULATORPRIVATE_H
