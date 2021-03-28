#include "mainwindow.h"
#include <QBoxLayout>
#include <QPainter>
#include <QPainterPath>
#include "ui_mainwindow.h"
#include "widgets/dailyregistrations.h"
#include "settings/settings.h"

MainWindow::MainWindow(MainController *mainController, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    mMainController = mainController;
    mJiraClient = QSharedPointer<JiraClient>(new JiraClient());

    ui->setupUi(this);

    ui->avatarLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    ui->avatarLabel->setStyleSheet("background-color: rgba(0,0,0,0%)");
    ui->avatarLabel->setAutoFillBackground(false);

    ui->menubar->setVisible(false);

    mAuthenticationStatusLabel = new QLabel();
    ui->statusbar->addPermanentWidget(mAuthenticationStatusLabel);

    setupDailyRegistrations();
    setupConnections();
    setupCalendar();
    setupCredentials();
    mWeeklyTotalCalculator.update();
    mMonthlyTotalCalculator.update();
}

void MainWindow::setupCalendar()
{
    ui->calendarWidget->setSelectedDate(QDate::currentDate());
}

void MainWindow::setupCredentials()
{
    ui->jiraUsernameLineEdit->setText(mSettings.username());
    ui->jiraTokenLineEdit->setText(mSettings.jiraToken());
    ui->jiraHostnameLineEdit->setText(mSettings.jiraHostname());
    if (mSettings.hasJiraAvatar()) {
        this->setAvatar(mSettings.jiraAvatar());
    }

}

void MainWindow::setAvatar(QPixmap pixmap)
{
    QImage maskedImage(pixmap.size(), QImage::Format_ARGB32); // Supports argb
    maskedImage.fill(qRgba(0, 0, 0, 0)); // Ensure that image is filled with transparent pixels

    QPainter painter(&maskedImage);
    painter.setPen(Qt::NoPen);
    QPainterPath path;
    path.addEllipse(pixmap.rect());
    painter.setClipPath(path);
    painter.setOpacity(1.0);
    painter.drawPixmap(0, 0, pixmap);
    painter.drawRect(pixmap.rect());

    this->ui->avatarLabel->setPixmap(QPixmap::fromImage(maskedImage));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupConnections()
{
    QObject::connect(&mAuthenticationState, &AuthenticationState::stateChanged,
                     this, &MainWindow::authenticationStateChanged);
    QObject::connect(&mSettings, &Settings::updated,
                     [this] {
                         if (mSettings.hasJiraAvatar()) {
                             this->setAvatar(mSettings.jiraAvatar());
                         }
                         this->ui->jiraDisplayNameLabel->setText(mSettings.jiraDisplayName());
                     });
    QObject::connect(ui->savePushButton, &QPushButton::clicked,
                     [this] {
                         mSettings.setUsername(this->ui->jiraUsernameLineEdit->text());
                         mSettings.setJiraToken(this->ui->jiraTokenLineEdit->text());
                         mSettings.setJiraHostname(this->ui->jiraHostnameLineEdit->text());
                     });
    QObject::connect(ui->calendarWidget, &QCalendarWidget::selectionChanged,
                     [this] {
                         this->setCurrentDate(this->ui->calendarWidget->selectedDate());
                     });
    QObject::connect(&mMonthlyTotalCalculator, &MonthlyTotalCalculator::updated,
                     this, &MainWindow::monthlyTotalCalculatorUpdated);
    QObject::connect(&mWeeklyTotalCalculator, &WeeklyTotalCalculator::updated,
                     this, &MainWindow::weeklyTotalCalculatorUpdated);

}

void MainWindow::setCurrentDate(QDate currentDate)
{
    mCurrentDate = currentDate;
    dailyRegistrations->setCurrentDate(mCurrentDate);
    mWeeklyTotalCalculator.update();
    mMonthlyTotalCalculator.update();
    ui->scrollArea->ensureVisible(0, 700); // Scroll to around start of work day
}

void MainWindow::monthlyTotalCalculatorUpdated(int seconds)
{
    int hours = seconds / 3600;
    int minutes = (seconds % 3600) / 60;
    ui->monthlyTotalLabel->setText(QString("Current monthly total: %1 hours %2 minutes").arg(hours).arg(minutes));
}

void MainWindow::weeklyTotalCalculatorUpdated(int seconds)
{
    int hours = seconds / 3600;
    int minutes = (seconds % 3600) / 60;
    ui->weeklyTotalLabel->setText(QString("Current weekly total: %1 hours %2 minutes").arg(hours).arg(minutes));
}

void MainWindow::authenticationStateChanged(AuthenticationState::State oldState, AuthenticationState::State newState)
{
    Q_UNUSED(oldState);
    if (newState == AuthenticationState::AUTHENTICATED) {
        mAuthenticationStatusLabel->setText("Authenticated");
    } else {
        mAuthenticationStatusLabel->setText("Not authenticated");
    }
}

void MainWindow::setupDailyRegistrations()
{
    auto dailyRegistrationsModel = new DailyRegistrationsModel(mJiraClient);
    this->dailyRegistrations = new DailyRegistrations(
        dailyRegistrationsModel,
        QDate::currentDate(),
        ui->scrollArea);
    ui->scrollArea->setWidget(dailyRegistrations);
    ui->scrollArea->ensureVisible(0, 300);
}

void MainWindow::updateTotals() {
    // and worklogAuthor = 557058:60fd2325-a1cb-4aab-8867-9fd89cb3a52a
//
    mMonthlyTotalCalculator.update();
    mWeeklyTotalCalculator.update();
}
