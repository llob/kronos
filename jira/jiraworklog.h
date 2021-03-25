#ifndef JIRAWORKLOG_H
#define JIRAWORKLOG_H

#include <QString>
#include <QDateTime>
#include <QVariant>
#include <QDebug>
#include <QSharedPointer>
#include "abstractmodels/abstractworklog.h"

class JiraWorklog : public AbstractWorklog
{
private:


protected:
    bool fromJson(const QVariantMap json);
public:
    JiraWorklog();
    JiraWorklog(const QByteArray json);
    JiraWorklog(const QVariantMap json);
    static QList<QSharedPointer<JiraWorklog>> fromJsonList(const QVariantList list);
    QByteArray toJson() const override;
    QVariant toVariant() const override;

};

#endif // JIRAWORKLOG_H
