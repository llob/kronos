#include "mainwindow.h"
#include "maincontroller.h"

#include <QApplication>


/*
 *
 * TODO:
 * - Worklog resolution of 15 minutes
 * - Establish github project
 * - Disallow overlapping worklogs
 * - Styling
 * - Rebrand as "Kronos"
 * - Handle HTTP response codes when making REST requests
 * - "Working" overlay on DailyRegistrations
 * - Bug: When adding the first registration for an issue, it cannot be located
 *     in the list of cached issues, so nothing gets drawn until data for date
 *     is reloaded from the server
 * - Optional: Keyboard shortcuts
 * - Optional: Resize worklogs
 * - Optional: Rebrand registrations as worklogs
 */

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationDisplayName("Jira Worklogs");
    a.setApplicationVersion("0.0.1");
    a.setApplicationName("jira-worklogs");

    MainController c;

    MainWindow w(&c);
    w.show();
    return a.exec();
}
