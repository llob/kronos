#include "monthlytotalcalculator.h"

QDate MonthlyTotalCalculator::firstDate() const
{
    auto today = QDate::currentDate();
    return today.addDays(-1 * (today.day() - 1));
}

MonthlyTotalCalculator::MonthlyTotalCalculator() : TotalCalculator()
{
}

void MonthlyTotalCalculator::update()
{
    auto firstDayOfMonth = firstDate();
    search(QString("worklogDate >= %1 and worklogAuthor = %2 ORDER BY created DESC")
               .arg(JiraClient::jqlDate(firstDayOfMonth))
               .arg(mSettings.accountId()));
}

