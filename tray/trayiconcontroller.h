#ifndef TRAYICONCONTROLLER_H
#define TRAYICONCONTROLLER_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QTimer>
#include "jira/jiraclient.h"
#include "authentication/authenticationstate.h"
#include "nagging/nagengine.h"

class TrayIconController : public QObject
{
    Q_OBJECT
public:
    explicit TrayIconController(QObject *parent = nullptr);
private:
    QSystemTrayIcon mSystemTrayIcon;
    QTimer mNotificationTimer;
    AuthenticationState mAuthenticationState;
    NagEngine mNagEngine;

    void setupTrayIcon();
    void setupConnections();
    void setupTimer();
private slots:
    void nagEngineNag(QString message);
signals:

};

#endif // TRAYICONCONTROLLER_H
