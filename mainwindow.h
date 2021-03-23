#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include "widgets/dailyregistrations.h"
#include "jira/jiraclient.h"
#include "settings/settings.h"
#include "maincontroller.h"
#include "utils/weeklytotalcalculator.h"
#include "utils/monthlytotalcalculator.h"
#include "authentication/authenticationstate.h"

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
    MonthlyTotalCalculator mMonthlyTotalCalculator;
    WeeklyTotalCalculator mWeeklyTotalCalculator;
    AuthenticationState mAuthenticationState;
    QLabel *mAuthenticationStatusLabel;

    void setupDailyRegistrations();
    void setupConnections();
    void setupCalendar();
    void setupCredentials();
    void setAvatar(QPixmap pixmap);
    void updateTotals();
private slots:
    void setCurrentDate(QDate currentDate);
    void monthlyTotalCalculatorUpdated(int seconds);
    void weeklyTotalCalculatorUpdated(int seconds);
    void authenticationStateChanged(AuthenticationState::State oldState, AuthenticationState::State newState);
};
#endif // MAINWINDOW_H
