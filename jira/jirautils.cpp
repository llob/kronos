#include "jirautils.h"
#include <QRegularExpression>
#include <QRegularExpressionMatch>

JiraUtils::JiraUtils()
{

}

QString JiraUtils::dateToString(const QDateTime dateTime)
{
    // Example output: 2021-03-17T07:18:21.720+0000
    QDate date = dateTime.date();
    QTime time = dateTime.time();
    auto res = QString("%1-%2-%3T%4:%5:%6.%7+%8");
    res = res.arg(date.year(), 4, 10, QLatin1Char('0'));
    res = res.arg(date.month(), 2, 10, QLatin1Char('0'));
    res = res.arg(date.day(), 2, 10, QLatin1Char('0'));
    res = res.arg(time.hour(), 2, 10, QLatin1Char('0'));
    res = res.arg(time.minute(), 2, 10, QLatin1Char('0'));
    res = res.arg(time.second(), 2, 10, QLatin1Char('0'));
    res = res.arg(time.msec(), 3, 10, QLatin1Char('0'));
    res = res.arg(dateTime.offsetFromUtc()*100/3600, 4, 10, QLatin1Char('0'));
    return res;
}

QString JiraUtils::dateToString(const QDate date)
{
    return dateToString(date.startOfDay());
}

QDateTime JiraUtils::stringToDateTime(const QString string)
{
    QRegularExpression dateTimeRegExp("^([0-9]{4})-([0-9]{2})-([0-9]{2})T([0-9]{2}):([0-9]{2}):([0-9]{2})\\.?([0-9]{0,3})(.*)$");
    QRegularExpressionMatch match = dateTimeRegExp.match(string);
    int year = match.captured(1).toInt();
    int month = match.captured(2).toInt();
    int day = match.captured(3).toInt();
    int hour = match.captured(4).toInt();
    int minute = match.captured(5).toInt();
    int second = match.captured(6).toInt();
    int ms = match.captured(7).toInt();
    QString tzOffset = match.captured(8);
    QDate startedDate(year, month, day);
    QTime startedTime(hour, minute, second, ms);
    QDateTime startedDateTime(startedDate, startedTime);

    return startedDateTime;
}
