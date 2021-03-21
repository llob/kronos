#include "jiraclient.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QNetworkRequest>

JiraClient::JiraClient()
{
    // worklogDate >= startOfMonth() and worklogAuthor = 557058:60fd2325-a1cb-4aab-8867-9fd89cb3a52a order by created DESC
    // https://combatstroke.atlassian.net/rest/api/latest/issue/PD-5328/worklog
    mNam = new QNetworkAccessManager();
    settingsUpdated(); // Read settings
    QObject::connect(&mSettings, &Settings::updated,
                     this, &JiraClient::settingsUpdated);
}

void JiraClient::setUsername(const QString username)
{
    qDebug() << "[JiraClient] Setting username to" << username;
    mUsername = username;
}

void JiraClient::setToken(const QString token)
{
    mToken = token;
}

QUrl JiraClient::url(QString path, QString query=QString()) {
    QUrl result;
    result.setScheme("https");
    result.setHost("combatstroke.atlassian.net");
    result.setPath(path);
    result.setQuery(query);
    return result;
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
                         QSharedPointer<JiraUser> j = QSharedPointer<JiraUser>(new JiraUser(reply->readAll()));
                         emit this->myselfFinished(j);
                     });
}

void JiraClient::addWorklog(QSharedPointer<JiraWorklog> worklog)
{
    QUrl u = url(QString("/rest/api/3/issue/%1/worklog")
                     .arg(worklog->issueId()));
    qDebug() << worklog->toJson();
    auto reply = post(u, worklog->toJson());
    QObject::connect(reply, &QNetworkReply::finished,
                     [this, reply] {
                         QSharedPointer<JiraWorklog> j = QSharedPointer<JiraWorklog>(new JiraWorklog(reply->readAll()));
                         emit this->addWorklogFinished(j);
                     });
}

void JiraClient::deleteWorklog(QSharedPointer<JiraWorklog> worklog)
{
    QUrl u = url(QString("/rest/api/3/issue/%1/worklog/%2")
                     .arg(worklog->issueId())
                     .arg(worklog->id()));
    auto reply = delete_request(u, worklog->toJson());
    QObject::connect(reply, &QNetworkReply::finished,
                     [this, reply] {
                         qDebug() << reply->readAll();
                         //QSharedPointer<JiraWorklog> j = QSharedPointer<JiraWorklog>(new JiraWorklog(reply->readAll()));
                         //emit this->addWorklogFinished(j); FIXME
                         emit this->deleteWorklogFinished(true);
                     });
}

void JiraClient::search(const QString query, int startAt, int maxResults)
{
    qDebug() << "[JiraClient] search()";
    QUrl u = url("/rest/api/latest/search");
//, QString("jql=%1&startAt=%2&maxResults=%3").arg(query).arg(startAt).arg(maxResults));
//                 QString("jql=worklogDate = %1 and worklogAuthor = 557058:60fd2325-a1cb-4aab-8867-9fd89cb3a52a order by created DESC").arg(jqlDate(date)));
    QVariantMap json;
    json.insert("jql", query);
    json.insert("startAt", startAt);
    json.insert("maxResults", maxResults);
    QByteArray j = QJsonDocument::fromVariant(json).toJson();
    auto reply = post(u, j);


    QObject::connect(reply, &QNetworkReply::finished,
                     [this, reply] {
                         qDebug() << "[JiraClient] Reply received";
                         QByteArray response = reply->readAll();
                         auto jsonDocument = QJsonDocument::fromJson(response).toVariant();
                         QVariantMap root = jsonDocument.toMap();
                         QVariant expandNode = root.value("expand");
                         QVariant startAtNode = root.value("startAt");
                         QVariant maxResultsNode = root.value("maxResults");
                         QVariant issuesNode = root.value("issues");
                         QVariantList issuesList = issuesNode.toList();
                         QList<QSharedPointer<JiraIssue>> issues = JiraIssue::fromJsonList(issuesList);
                         qDebug() << "[JiraClient] Found" << issues.length() << "issues";
                         emit this->searchFinished(issues);
                     });

}

void JiraClient::settingsUpdated()
{
    setUsername(mSettings.jiraUsername());
    setToken(mSettings.jiraToken());
}

void JiraClient::issueWorklogs(QSharedPointer<JiraIssue> issue)
{
    QUrl u = url(QString("/rest/api/latest/issue/%1/worklog").arg(issue->key()));
    auto reply = get(u);
    QObject::connect(reply, &QNetworkReply::finished,
                     [this, reply] {
                        QByteArray response = reply->readAll();
                        qDebug() << response;
                        QVariant json = QJsonDocument::fromJson(response).toVariant();
                        QVariantMap root = json.toMap();
                        QVariantList worklogsNode = root.value("worklogs").toList();
                        auto jws = JiraWorklog::fromJsonList(worklogsNode);
                        qDebug() << "[JiraClient] Worklog count is" << jws.length();
                        emit this->issueWorklogsFinished(jws);
                     });
}


QNetworkReply* JiraClient::delete_request(QUrl url, QByteArray data) {
    const QByteArray authorization = "Basic " + QString("%1:%2").arg(mUsername).arg(mToken).toUtf8().toBase64();
    QNetworkRequest req;
    req.setUrl(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setRawHeader("Accept", "application/json");
    req.setRawHeader("Authorization", authorization);

    qDebug() << "[JiraClient] DELETE'ing" << data << "to" << url;
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

    qDebug() << "[JiraClient] POST'ing" << data << "to" << url;
    QNetworkReply *reply = mNam->post(req, data);

    return reply;
}

QNetworkReply* JiraClient::get(QUrl url) {
    const QByteArray authorization = "Basic " + QString("%1:%2").arg(mUsername).arg(mToken).toUtf8().toBase64();
    QNetworkRequest req;
    req.setUrl(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setRawHeader("Authorization", authorization);

    qDebug() << "[JiraClient] GET'ing" << url;
    QNetworkReply *reply = mNam->get(req);

    return reply;
}
