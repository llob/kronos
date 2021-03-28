#include "abstractissue.h"
#include <QDebug>

AbstractIssue::AbstractIssue()
{

}


void AbstractIssue::setKey(const QString key)
{
    mKey = key;
}

QString AbstractIssue::key() const
{
    return mKey;
}

void AbstractIssue::setId(const QString id)
{
    mId = id;
}

QString AbstractIssue::id() const
{
    return mId;
}

void AbstractIssue::setSummary(const QString summary)
{
    mSummary = summary;
}

QString AbstractIssue::summary() const
{
    return mSummary;
}

QByteArray AbstractIssue::toJson() const
{
    qWarning() << "Invalid call to not implemented function" << __FUNCTION__;
    return QByteArray();
}
