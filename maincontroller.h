#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QSharedPointer>
#include <QObject>
#include <QNetworkAccessManager>
#include "jira/jiraclient.h"
#include "jira/jirauser.h"
#include "settings/settings.h"
#include "tray/trayiconcontroller.h"

class MainController : public QObject
{
    Q_OBJECT
private:
    Settings mSettings;
    TrayIconController mTrayIconController;
    void setupConnections();
public:
    explicit MainController(QObject *parent = nullptr);
signals:

};

#endif // MAINCONTROLLER_H
