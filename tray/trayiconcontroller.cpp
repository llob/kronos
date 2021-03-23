#include "trayiconcontroller.h"

TrayIconController::TrayIconController(QObject *parent) : QObject(parent)
{
    setupTrayIcon();
    setupConnections();
}

void TrayIconController::setupTrayIcon()
{
    mSystemTrayIcon.setIcon(QIcon(QPixmap::fromImage(QImage(":/resources/watch.svg"))));
    mSystemTrayIcon.show();
}

void TrayIconController::setupConnections()
{
    QObject::connect(&mSystemTrayIcon, &QSystemTrayIcon::messageClicked,
                     [] { });
    QObject::connect(&mSystemTrayIcon, &QSystemTrayIcon::activated,
                     [] { });
}

void TrayIconController::setupTimer()
{
    mNotificationTimer.setInterval(10000);
    QObject::connect(&mNotificationTimer, &QTimer::timeout,
                     [] {

                     });
}
