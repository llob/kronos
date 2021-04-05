#include "weeklytotalcalculator.h"

WeeklyTotalCalculator::WeeklyTotalCalculator() : TotalCalculator()
{

}

QDate WeeklyTotalCalculator::firstDate() const {
    auto today = QDate::currentDate();
    return today.addDays(-1 * (today.dayOfWeek() - 1));
}

void WeeklyTotalCalculator::update()
{
    auto firstDayOfWeek = firstDate();
    search(QString("worklogDate >= %1 and worklogAuthor = %2 ORDER BY created DESC")
               .arg(JiraClient::jqlDate(firstDayOfWeek))
               .arg(mSettings.accountId()));
}

