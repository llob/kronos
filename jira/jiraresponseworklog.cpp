#include "jiraresponseworklog.h"
#include <QVariant>
#include <QDate>
#include <QTime>

void JiraResponseWorklog::parse()
{


}

JiraResponseWorklog::JiraResponseWorklog(QByteArray response) : JiraResponse(response)
{
}

QList<QSharedPointer<JiraWorklog> > JiraResponseWorklog::worklogs()
{
    if (!mParsed) {
        parse();
    }
    return mWorklogs;
}
