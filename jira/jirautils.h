#ifndef JIRAUTILS_H
#define JIRAUTILS_H

#include <QDateTime>
#include <QString>

class JiraUtils
{
public:
    JiraUtils();
    static QString dateToString(const QDateTime dateTime);
    static QDateTime stringToDateTime(const QString string);
};

#endif // JIRAUTILS_H
