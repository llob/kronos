#include "mainwindow.h"
#include "maincontroller.h"

#include <QApplication>


/*
 *
 * TODO:
 * - Disallow overlapping worklogs
 * - Styling
 * - Worklog rects should have rounded corners
 * - Handle HTTP response codes when making REST requests
 * - "Working" overlay on DailyRegistrations
 * - Label issues as "recent" or "search result" in registration dialog list
 * - Scroll to start-of-work-day automatically, when rendering a new date.
 * - Double clicking an issue should accept the registration dialog
 * - Bug: Weekly and monthly totals look at all worklogs
 *     not just those of current user
 * - Bug: When adding the first registration for an issue, it cannot be located
 *     in the list of cached issues, so nothing gets drawn until data for date
 *     is reloaded from the server
 * - Bug: Monthly total doesn't look correct at all times
 * - Bug: Restart required to load profile image and stuff
 * - Optional: Keyboard shortcuts
 * - Optional: Resize worklogs
 * - Optional: Rebrand registrations as worklogs
 */

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationDisplayName("Kronos");
    a.setApplicationVersion("0.0.1");
    a.setApplicationName("kronos");

    MainController c;

    MainWindow w(&c);
    w.show();
    return a.exec();
}
