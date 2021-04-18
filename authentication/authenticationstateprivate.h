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
public:
    explicit AuthenticationStatePrivate();
    /**
     * @brief state Get the current authentication state
     * @return
     */
    AuthenticationState::State state() const;
    /**
     * @brief update Redetermine authentication state, by
     *  contacting the backend.
     */
    void update();
private slots:
    void settingsCredentialsUpdated();
    void jiraClientMyselfFinished(QSharedPointer<JiraUser> myself);
    void jiraClientMyselfFailed(int httpCode, QNetworkReply::NetworkError error, QString message);
signals:
    /**
     * @brief stateChanged Emitted when the current authentication state changes
     * @param oldState The previous state
     * @param newState The new (current) state
     */
    void stateChanged(AuthenticationState::State oldState, AuthenticationState::State newState, const QString message);
};

#endif // AUTHENTICATIONSTATEPRIVATE_H
