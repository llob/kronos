#include "maincontroller.h"
#include <QUrl>
#include <QNetworkReply>

void MainController::setupConnections()
{

}

MainController::MainController(QObject *parent) : QObject(parent)
{
    setupConnections();
}
