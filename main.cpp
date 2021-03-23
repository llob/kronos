#include "mainwindow.h"
#include "maincontroller.h"

#include <QApplication>


/*
 *
 * TODO:
 * - Disallow overlapping worklogs
 * - "Working" overlay on DailyRegistrations
 * - Label issues as "recent" or "search result" in registration dialog list
 * - Double clicking an issue should accept the registration dialog
 * - Typing "Enter" should perform search immediately in registration dialog
 * - Styling
 * - Annoying tray icon which nags about logging work
 * - Optional: Collapse authentication widgets
 * - Optional: Persist window size and location across restarts
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
