#ifndef JIRAWORKLOG_H
#define JIRAWORKLOG_H

#include <QString>
#include <QDateTime>
#include <QVariant>
#include <QDebug>
#include <QSharedPointer>
#include "jiraobject.h"

class JiraWorklog : public JiraObject
{
private:
    QString mId;
    QString mIssueId;
    QString mAccountId;
    QString mEmailAddress;
    QDateTime mStarted;
    int mTimeSpentSeconds;

protected:
    bool fromJson(const QVariantMap json);
public:
    JiraWorklog();
    JiraWorklog(const QByteArray json);
    JiraWorklog(const QVariantMap json);
    static QList<QSharedPointer<JiraWorklog>> fromJsonList(const QVariantList list);
    QString id() const;
    void setId(const QString id);
    QString issueId() const;
    void setIssueId(const QString issueId);
    QDateTime started() const;
    void setStarted(const QDateTime started);
    int timeSpentSeconds() const;
    void setTimeSpentSeconds(int timeSpentSeconds);
    QString accountId() const;
    void setAccountId(const QString accountId);
    QString emailAddress() const;
    void setEmailAddress(const QString emailAddress);
    QByteArray toJson() const override;
    QVariant toVariant() const override;
    bool contains(QDateTime dateTime);

};

#endif // JIRAWORKLOG_H
