#ifndef JIRARESPONSESEARCH_H
#define JIRARESPONSESEARCH_H

#include "jiraresponse.h"
#include "jiraissue.h"

class JiraResponseSearch : public JiraResponse
{
private:
    QList<QSharedPointer<JiraIssue>> mIssues;
    void parse();
public:
    JiraResponseSearch(QByteArray response);
    QList<QSharedPointer<JiraIssue>> issues();
};

#endif // JIRARESPONSESEARCH_H
