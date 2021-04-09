#include "maincontroller.h"
#include <QUrl>
#include <QNetworkReply>
#include "utils/recentissues.h"
#include "cache/issuecache.h"

void MainController::setupConnections()
{

}

MainController::MainController() : QObject()
{
    setupConnections();
    RecentIssues ri;
    IssueCache ic;
    ic.putList(ri.issues());
}
