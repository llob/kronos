#include "jiraclient.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QNetworkRequest>

JiraClient::JiraClient()
{
    mNam = QSharedPointer<QNetworkAccessManager>(new QNetworkAccessManager());
    settingsCredentialsUpdated(); // Read settings
    QObject::connect(&mSettings, &Settings::credentialsUpdated,
                     this, &JiraClient::settingsCredentialsUpdated);
}

void JiraClient::setUsername(const QString username)
{
    mUsername = username;
}

void JiraClient::setToken(const QString token)
{
    mToken = token;
}

QUrl JiraClient::url(QString path, QString query=QString()) {
    QUrl result;
    result.setScheme("https");
    result.setHost(mSettings.hostname());
    result.setPath(path);
    result.setQuery(query);
    return result;
}

int JiraClient::httpCode(const QNetworkReply *reply)
{
    return reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
}

QString JiraClient::jqlDate(QDate date) {
    return date.toString("yyyy-MM-dd");
}

void JiraClient::myself()
{
    QUrl u = url("/rest/api/latest/myself");
    auto reply = get(u);
    QObject::connect(reply, &QNetworkReply::finished,
                     [this, reply] {

                         if (!hasHttpCode(reply, {200})) {
                             emit this->myselfFailed(httpCode(reply), reply->error(), reply->errorString());
                             return;
                         }

                         QSharedPointer<JiraUser> j = QSharedPointer<JiraUser>(new JiraUser(reply->readAll()));
                         emit this->myselfFinished(j);
                     });
}

void JiraClient::addWorklog(QSharedPointer<JiraWorklog> worklog)
{
    QUrl u = url(QString("/rest/api/3/issue/%1/worklog")
                     .arg(worklog->issueId()));
    auto reply = post(u, worklog->toJson());
    QObject::connect(reply, &QNetworkReply::finished,
                     [this, reply] {

                         if (!hasHttpCode(reply, {200, 201})) {
                             emit this->addWorklogFailed(httpCode(reply), reply->error(), reply->errorString());
                             return;
                         }

                         QSharedPointer<JiraWorklog> j = QSharedPointer<JiraWorklog>(new JiraWorklog(reply->readAll()));
                         emit this->addWorklogFinished(j);
                     });
}

void JiraClient::deleteWorklog(QSharedPointer<JiraWorklog> worklog)
{
    QUrl u = url(QString("/rest/api/3/issue/%1/worklog/%2")
                     .arg(worklog->issueId())
                     .arg(worklog->id()));
    auto reply = delete_request(u);
    QObject::connect(reply, &QNetworkReply::finished,
                     [this, reply] {

                         if (!hasHttpCode(reply, {200, 204})) {
                             emit this->deleteWorklogFailed(httpCode(reply), reply->error(), reply->errorString());
                             return;
                         }

                         emit this->deleteWorklogFinished(true);
                     });
}

bool JiraClient::hasHttpCode(QNetworkReply *reply, QList<int> httpCodes) {
    return httpCodes.contains(httpCode(reply));
}

void JiraClient::search(const QString query, int startAt, int maxResults)
{
    QUrl u = url("/rest/api/latest/search");
    QVariantMap json;
    json.insert("jql", query);
    json.insert("startAt", startAt);
    json.insert("maxResults", maxResults);
    QByteArray j = QJsonDocument::fromVariant(json).toJson();
    auto reply = post(u, j);


    QObject::connect(reply, &QNetworkReply::finished,
                     [this, reply] {

                         if (!hasHttpCode(reply, {200})) {
                             emit this->searchFailed(httpCode(reply), reply->error(), reply->errorString());
                             return;
                         }

                         QByteArray response = reply->readAll();
                         auto jsonDocument = QJsonDocument::fromJson(response).toVariant();
                         QVariantMap root = jsonDocument.toMap();
                         QVariant expandNode = root.value("expand");
                         Q_UNUSED(expandNode);
                         QVariant startAtNode = root.value("startAt");
                         Q_UNUSED(startAtNode);
                         QVariant maxResultsNode = root.value("maxResults");
                         Q_UNUSED(maxResultsNode);
                         QVariant issuesNode = root.value("issues");
                         QVariantList issuesList = issuesNode.toList();
                         QList<QSharedPointer<AbstractIssue>> issues = JiraIssue::fromJsonList(issuesList);
                         mIssueCache.putList(issues);
                         emit this->searchFinished(issues);
                     });

}

void JiraClient::settingsCredentialsUpdated()
{
    setUsername(mSettings.username());
    setToken(mSettings.secret());
}

void JiraClient::issueWorklogs(QSharedPointer<AbstractIssue> issue)
{
    QUrl u = url(QString("/rest/api/latest/issue/%1/worklog").arg(issue->key()));
    auto reply = get(u);
    QObject::connect(reply, &QNetworkReply::finished,
                     [this, reply] {

                        if (!hasHttpCode(reply, {200})) {
                            emit this->issueWorklogsFailed(httpCode(reply), reply->error(), reply->errorString());
                            return;
                        }

                        QByteArray response = reply->readAll();
                        QVariant json = QJsonDocument::fromJson(response).toVariant();
                        QVariantMap root = json.toMap();
                        QVariantList worklogsNode = root.value("worklogs").toList();
                        auto jws = JiraWorklog::fromJsonList(worklogsNode);
                        emit this->issueWorklogsFinished(jws);
                     });
}


QNetworkReply* JiraClient::delete_request(QUrl url) {
    const QByteArray authorization = "Basic " + QString("%1:%2").arg(mUsername).arg(mToken).toUtf8().toBase64();
    QNetworkRequest req;
    req.setUrl(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setRawHeader("Accept", "application/json");
    req.setRawHeader("Authorization", authorization);
#if QT_VERSION >= 0x050F00
    req.setTransferTimeout(TRANSFER_TIMEOUT_MS);
#endif

    QNetworkReply *reply = mNam->deleteResource(req);

    return reply;
}

QNetworkReply* JiraClient::post(QUrl url, QByteArray data) {
    const QByteArray authorization = "Basic " + QString("%1:%2").arg(mUsername).arg(mToken).toUtf8().toBase64();
    QNetworkRequest req;
    req.setUrl(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setRawHeader("Accept", "application/json");
    req.setRawHeader("Authorization", authorization);
#if QT_VERSION >= 0x050F00
    req.setTransferTimeout(TRANSFER_TIMEOUT_MS);
#endif

    QNetworkReply *reply = mNam->post(req, data);

    return reply;
}

QNetworkReply* JiraClient::get(QUrl url) {
    const QByteArray authorization = "Basic " + QString("%1:%2").arg(mUsername).arg(mToken).toUtf8().toBase64();
    QNetworkRequest req;
    req.setUrl(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setRawHeader("Authorization", authorization);
#if QT_VERSION >= 0x050F00
    req.setTransferTimeout(TRANSFER_TIMEOUT_MS);
#endif

    QNetworkReply *reply = mNam->get(req);

    return reply;
}
