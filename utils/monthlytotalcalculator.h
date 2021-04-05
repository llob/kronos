#ifndef MONTHLYTOTALCALCULATOR_H
#define MONTHLYTOTALCALCULATOR_H

#include <QObject>
#include <QList>
#include <QSharedPointer>
#include "jira/jiraworklog.h"
#include "jira/jiraissue.h"
#include "settings/settings.h"
#include "totalcalculator.h"

/**
 * @brief The MonthlyTotalCalculator class continuously calculates
 *  the total number of hours logged for the current month
 */
class MonthlyTotalCalculator : public TotalCalculator
{
    Q_OBJECT
private:
    QDate firstDate() const override;
public:
    explicit MonthlyTotalCalculator();
    /**
     * @brief update Recalculate month total
     */
    void update() override;

};

#endif // MONTHLYTOTALCALCULATOR_H
