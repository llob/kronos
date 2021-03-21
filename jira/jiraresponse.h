#ifndef JIRARESPONSE_H
#define JIRARESPONSE_H

#include <QByteArray>
#include <QJsonDocument>

class JiraResponse
{
private:

    QByteArray mResponse;
    QJsonDocument mJsonDocument;
    QJsonParseError *mParseError;
protected:
    bool mValid;
    bool mParsed;
public:
    JiraResponse(QByteArray response);
    QJsonParseError* parseError() const;
    QJsonDocument jsonDocument() const;
    bool valid() const;
    void virtual parse() = 0;
};

#endif // JIRARESPONSE_H
