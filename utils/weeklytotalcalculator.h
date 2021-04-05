#ifndef WEEKLYTOTALCALCULATOR_H
#define WEEKLYTOTALCALCULATOR_H

#include <QObject>
#include <QList>
#include <QSharedPointer>
#include "jira/jiraworklog.h"
#include "jira/jiraissue.h"
#include "settings/settings.h"
#include "totalcalculator.h"

/**
 * @brief The WeeklyTotalCalculator class continuous calculates the time
 *  logged during the current week.
 */
class WeeklyTotalCalculator : public TotalCalculator
{
    Q_OBJECT
private:
    QDate firstDate() const override;
public:
    explicit WeeklyTotalCalculator();
    /**
     * @brief update Recalculate the weekly total
     */
    void update() override;
signals:

};

#endif // WEEKLYTOTALCALCULATOR_H
