#ifndef JIRAISSUE_H
#define JIRAISSUE_H

#include <QString>
#include "jiraobject.h"

class JiraIssue : public JiraObject
{
private:
    QString mKey;
    QString mSummary;
    QString mId;
public:
    JiraIssue();
    JiraIssue(const QVariantMap json);
    JiraIssue(const QByteArray json);
    static QList<QSharedPointer<JiraIssue>> fromJsonList(const QVariantList list);
    void setKey(const QString key);
    QString key() const;
    void setId(const QString id);
    QString id() const;
    void setSummary(const QString summary);
    QString summary() const;
    QByteArray toJson() const;
    QString toString() const;
protected:
    bool fromJson(const QVariantMap json);
};

#endif // JIRAISSUE_H
