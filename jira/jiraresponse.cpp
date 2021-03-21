#include "jiraresponse.h"

JiraResponse::JiraResponse(QByteArray response)
{
    mParsed = false;
    mValid = false;
    mParseError = new QJsonParseError();
    mResponse = response;
    mJsonDocument = QJsonDocument::fromJson(mResponse, mParseError);
}

bool JiraResponse::valid() const
{
    return !mJsonDocument.isNull();
}

QJsonParseError *JiraResponse::parseError() const
{
    return mParseError;
}

QJsonDocument JiraResponse::jsonDocument() const
{
    return mJsonDocument;
}
