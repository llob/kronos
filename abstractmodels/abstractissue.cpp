#include "abstractissue.h"

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
    return ""; // FIXME Not yet implemented
}
