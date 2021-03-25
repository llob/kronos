#ifndef JIRAISSUE_H
#define JIRAISSUE_H

#include <QString>
#include <QSharedPointer>
#include <QVariantList>
#include <QVariantMap>
#include <QVariant>
#include <QDebug>
#include "abstractmodels/abstractissue.h"

class JiraIssue : public AbstractIssue
{
private:

public:
    JiraIssue();
    JiraIssue(const QVariantMap json);
    JiraIssue(const QByteArray json);
    static QList<QSharedPointer<JiraIssue>> fromJsonList(const QVariantList list);
    QString toString() const;
    QVariant toVariant() const override;
protected:
    bool fromJson(const QVariantMap json);
};

#endif // JIRAISSUE_H
