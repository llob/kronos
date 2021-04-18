#include "authenticationstateprivate.h"

AuthenticationStatePrivate::AuthenticationStatePrivate()
{
    mState = AuthenticationState::DEAUTHENTICATED;
    QObject::connect(&mSettings, &Settings::credentialsUpdated,
                     this, &AuthenticationStatePrivate::settingsCredentialsUpdated);
    QObject::connect(&mJiraClient, &JiraClient::myselfFinished,
                     this, &AuthenticationStatePrivate::jiraClientMyselfFinished);
    QObject::connect(&mJiraClient, &JiraClient::myselfFailed,
                     this, &AuthenticationStatePrivate::jiraClientMyselfFailed);
    update();
}

AuthenticationState::State AuthenticationStatePrivate::state() const
{
    return mState;
}

void AuthenticationStatePrivate::update()
{
    emit stateChanged(mState, AuthenticationState::AUTHENTICATING, "Authenticating");
    mJiraClient.myself();
}

void AuthenticationStatePrivate::settingsCredentialsUpdated()
{
    auto oldState = mState;
    mState = AuthenticationState::DEAUTHENTICATED;
    emit stateChanged(oldState, mState, "Credentials changed");
    update();
}

void AuthenticationStatePrivate::jiraClientMyselfFinished(QSharedPointer<JiraUser> myself)
{
    auto oldState = mState;
    mState = AuthenticationState::AUTHENTICATED;
    mSettings.setDisplayName(myself->displayName());
    mSettings.setAccountId(myself->accountId());
    if (!myself->avatarUrls().isEmpty()) {
        auto reply = mNam.get(QNetworkRequest(QUrl(myself->avatarUrls().last())));
        QObject::connect(reply, &QNetworkReply::finished,
                         [this, reply, oldState] {
                             QByteArray bytes = reply->readAll();
                             this->mSettings.setAvatar(QImage::fromData(bytes));
                             emit stateChanged(oldState, mState, "Successfully authenticated");
                         });
    } else {
        emit stateChanged(oldState, mState, "Successfully authenticated");
    }
}

void AuthenticationStatePrivate::jiraClientMyselfFailed(int httpCode, QNetworkReply::NetworkError error, QString message)
{
    Q_UNUSED(httpCode);
    Q_UNUSED(error);
    Q_UNUSED(message);
    qWarning()  << "[AuthenticationStatePrivate] myself request failed with http code" << httpCode << ":" << error << message;
    auto oldState = mState;
    mState = AuthenticationState::DEAUTHENTICATED;
    mSettings.unsetDisplayName();
    mSettings.unsetAccountId();
    mSettings.unsetAvatar();

    emit stateChanged(oldState, mState, QString("%1 %2").arg(httpCode).arg(message));
}
