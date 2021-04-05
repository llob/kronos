#include "mainwindow.h"
#include "maincontroller.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationDisplayName("Kronos");
    a.setApplicationVersion("1.0");
    a.setApplicationName("kronos");

    MainController c;

    MainWindow w(&c);
    w.show();
    return a.exec();
}
