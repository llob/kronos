#ifndef AUTHENTICATIONSTATEPRIVATE_H
#define AUTHENTICATIONSTATEPRIVATE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "authentication/authenticationstate.h"
#include "settings/settings.h"
#include "jira/jiraclient.h"

class AuthenticationStatePrivate : public QObject
{
    Q_OBJECT
private:
    Settings mSettings;
    JiraClient mJiraClient;
    AuthenticationState::State mState;
    QNetworkAccessManager mNam;
    QString mOldJiraUsername;
    QString mOldJiraToken;
    QString mOldJiraHostname;
public:
    explicit AuthenticationStatePrivate(QObject *parent = nullptr);
    AuthenticationState::State state() const;
    void update();
private slots:
    void settingsUpdated();
    void jiraClientMyselfFinished(QSharedPointer<JiraUser> myself);
    void jiraClientMyselfFailed(int httpCode, QNetworkReply::NetworkError error, QString message);
signals:
    void stateChanged(AuthenticationState::State oldState, AuthenticationState::State newState);
};

#endif // AUTHENTICATIONSTATEPRIVATE_H
