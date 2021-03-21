#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QSharedPointer>
#include <QObject>
#include <QNetworkAccessManager>
#include "jira/jiraclient.h"
#include "jira/jirauser.h"
#include "settings/settings.h"

class MainController : public QObject
{
    Q_OBJECT
private:
    JiraClient mJiraClient;
    Settings mSettings;
    QNetworkAccessManager mNam;
    void setupConnections();
public:
    explicit MainController(QObject *parent = nullptr);
private slots:
    void jiraClientMyselfFinished(QSharedPointer<JiraUser> mysefl);
signals:

};

#endif // MAINCONTROLLER_H
