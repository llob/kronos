#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QResizeEvent>
#include <QMoveEvent>
#include "widgets/dailyworklogs.h"
#include "widgets/kronoscalendarwidget.h"
#include "jira/jiraclient.h"
#include "settings/settings.h"
#include "maincontroller.h"
#include "utils/weeklytotalcalculator.h"
#include "utils/monthlytotalcalculator.h"
#include "authentication/authenticationstate.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @brief The JiraHostnameValidator class helps validate
 *  that Jira hostnames entered are valid. Users appear to
 *  want to enter URLs
 */
class JiraHostnameValidator : public QValidator {
public:
    State validate(QString &, int &) const override;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(MainController *mainController, QWidget *parent = nullptr);
    ~MainWindow();
protected:
    void resizeEvent(QResizeEvent *event);
    void moveEvent(QMoveEvent *event);
private:
    Ui::MainWindow *ui;
    DailyWorklogs *mDailyWorklogs;
    KronosCalendarWidget *mCalendarWidget;
    QDate mCurrentDate;
    QSharedPointer<JiraClient> mJiraClient;
    Settings mSettings;
    MainController *mMainController;
    MonthlyTotalCalculator mMonthlyTotalCalculator;
    WeeklyTotalCalculator mWeeklyTotalCalculator;
    AuthenticationState mAuthenticationState;
    QLabel *mAuthenticationStatusLabel;

    void setupCalendarWidget();
    void setupDailyRegistrations();
    void setupConnections();
    void setupCredentials();
    void setAvatar(QPixmap pixmap);
    void showCredentials(bool visible);
    void updateVisibilityPushButtonIcon();

private slots:
    void setCurrentDate(QDate currentDate);
    void monthlyTotalCalculatorUpdated(int seconds);
    void weeklyTotalCalculatorUpdated(int seconds);
    void authenticationStateChanged(AuthenticationState::State oldState, AuthenticationState::State newState, const QString message);
    void toggleVisbilityPushButtonClicked();
};
#endif // MAINWINDOW_H
