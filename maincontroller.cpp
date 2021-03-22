#include "maincontroller.h"
#include <QUrl>
#include <QNetworkReply>

void MainController::setupConnections()
{
    QObject::connect(&mJiraClient, &JiraClient::myselfFinished,
                     this, &MainController::jiraClientMyselfFinished);
}

MainController::MainController(QObject *parent) : QObject(parent)
{
    setupConnections();
    mJiraClient.myself();
}

void MainController::jiraClientMyselfFinished(QSharedPointer<JiraUser> myself)
{
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
