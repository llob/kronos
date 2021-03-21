#ifndef JIRARESPONSEWORKLOG_H
#define JIRARESPONSEWORKLOG_H

#include "jiraresponse.h"
#include "jiraworklog.h"

class JiraResponseWorklog : public JiraResponse
{
private:
    QList<QSharedPointer<JiraWorklog>> mWorklogs;
    void parse();
public:
    JiraResponseWorklog(QByteArray response);
    QList<QSharedPointer<JiraWorklog>> worklogs();
};

#endif // JIRARESPONSEWORKLOG_H
