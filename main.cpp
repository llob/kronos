#include "mainwindow.h"
#include "maincontroller.h"
#include <settings/settings.h>


#include <QApplication>
#include <QDebug>
#include <QCommandLineParser>

void parseCommandLine(const QApplication &a)
{
    Settings settings;

    QCommandLineParser parser;

    QCommandLineOption clearSettingsOption("clear-settings", "Clear stored settings before starting application");

    parser.addOption(clearSettingsOption);

    if (!parser.parse(a.arguments())) {
        qWarning() << "Failed to parse command line:" << parser.errorText();
        exit(1);
    }

    // Settings parsed correctly, so handle them
    if (parser.isSet(clearSettingsOption)) {
        settings.clear();
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationDisplayName("Kronos");
    a.setApplicationVersion("1.0");
    a.setApplicationName("kronos");

    parseCommandLine(a);

    MainController c;

    MainWindow w(&c);
    w.show();
    return a.exec();
}
