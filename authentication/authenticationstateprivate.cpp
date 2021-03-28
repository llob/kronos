#include "authenticationstateprivate.h"

AuthenticationStatePrivate::AuthenticationStatePrivate()
{
    mState = AuthenticationState::DEAUTHENTICATED;
    QObject::connect(&mSettings, &Settings::updated,
                     this, &AuthenticationStatePrivate::settingsUpdated);
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
    mJiraClient.myself();
}

void AuthenticationStatePrivate::settingsUpdated()
{
    // Check whether anything changed in the settings
    // since we were last notified
    bool updateState = false;
    updateState |= mOldJiraToken != mSettings.jiraToken();
    updateState |= mOldJiraUsername != mSettings.username();
    updateState |= mOldJiraHostname != mSettings.jiraHostname();
    if (updateState) {
        // One or more fields changed, so we need to update
        // our authenticated state.
        mOldJiraHostname = mSettings.jiraHostname();
        mOldJiraToken = mSettings.jiraToken();
        mOldJiraUsername = mSettings.username();
        update();
    }
}

void AuthenticationStatePrivate::jiraClientMyselfFinished(QSharedPointer<JiraUser> myself)
{
    auto oldState = mState;
    mState = AuthenticationState::AUTHENTICATED;
    emit stateChanged(oldState, mState);
    mSettings.setJiraDisplayName(myself->displayName());
    mSettings.setJiraAccountId(myself->accountId());
    if (!myself->avatarUrls().isEmpty()) {
        auto reply = mNam.get(QNetworkRequest(QUrl(myself->avatarUrls().last())));
        QObject::connect(reply, &QNetworkReply::finished,
                         [this, reply] {
                             QByteArray bytes = reply->readAll();
                             this->mSettings.setJiraAvatar(QImage::fromData(bytes));
                         });
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
    mSettings.setJiraDisplayName("");
    mSettings.setJiraAccountId("");
    mSettings.setJiraAvatar(QImage());
    emit stateChanged(oldState, mState);
}
