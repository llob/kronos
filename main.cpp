#include "mainwindow.h"
#include "maincontroller.h"

#include <QApplication>


/*
 *
 * TODO:
 * - Disallow overlapping worklogs
 * - Styling
 * - Handle HTTP response codes when making REST requests
 * - "Working" overlay on DailyRegistrations
 * - Label issues as "recent" or "search result" in registration dialog list
 * - Scroll to start-of-work-day automatically, when rendering a new date.
 * - Double clicking an issue should accept the registration dialog
 * - Annoying tray icon which nags about logging work
 * - Bug: Restart required to load profile image and stuff
 * - Optional: Persist recently used issues list
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
