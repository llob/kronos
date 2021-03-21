#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "widgets/dailyregistrations.h"
#include "jira/jiraclient.h"
#include "settings/settings.h"
#include "maincontroller.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(MainController *mainController, QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    DailyRegistrations *dailyRegistrations;
    QDate mCurrentDate;
    JiraClient *mJiraClient;
    Settings mSettings;
    MainController *mMainController;
    void setupDailyRegistrations();
    void setupConnections();
    void setupCalendar();
    void setupCredentials();
    void setAvatar(QPixmap pixmap);
private slots:
    void setCurrentDate(QDate currentDate);
};
#endif // MAINWINDOW_H
