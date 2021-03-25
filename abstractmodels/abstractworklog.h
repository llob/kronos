#ifndef ABSTRACTWORKLOG_H
#define ABSTRACTWORKLOG_H

#include "modelobject.h"

class AbstractWorklog : public ModelObject
{
protected:
    QString mId;
    QString mIssueId;
    QString mAccountId;
    QString mEmailAddress;
    QDateTime mStarted;
    int mTimeSpentSeconds;

public:
    AbstractWorklog();
    AbstractWorklog(const QByteArray json);
    AbstractWorklog(const QVariantMap json);
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

    bool contains(QDateTime dateTime);
};

#endif // ABSTRACTWORKLOG_H
