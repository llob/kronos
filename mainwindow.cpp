#include "mainwindow.h"
#include <QBoxLayout>
#include <QGraphicsSvgItem>
#include <QSvgRenderer>
#include <QPainter>
#include <QPainterPath>
#include <QPixmap>
#include "ui_mainwindow.h"
#include "widgets/dailyregistrations.h"
#include "settings/settings.h"
#include "utils/colors.h"

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
    setupCalendar();
    setupCredentials();
    setupConnections();

    mWeeklyTotalCalculator.update();
    mMonthlyTotalCalculator.update();

    showCredentials(mSettings.showCredentials());

    restoreGeometry(mSettings.windowGeometry());
    updateVisibilityPushButtonIcon();
}

void MainWindow::updateVisibilityPushButtonIcon() {
    QSvgRenderer svgRenderer(QString(":/resources/chevron_right.svg"));
    // We use minimumSize() here, as the buttons size is not correctly computed
    // when it is first rendered, and as a consequence, the size of the output
    // image is incorrectly calculated. Since we do not resize
    // the button, this is fine for now.
    QImage out(ui->toggleVisibilityPushButton->minimumSize(), QImage::Format_ARGB32);
    out.fill(Colors::ivory());
    QPainter painter(&out);
    // Check whether we should show credentials
    if (mSettings.showCredentials()) {
        // Draw the "expanded" state of the button image
        // with the chevron rotated 90 degress clock wise
        painter.translate(out.width(), 0);
        painter.rotate(90);
    }
    svgRenderer.render(&painter, out.rect());
    QPalette palette;
    palette.setBrush(ui->toggleVisibilityPushButton->backgroundRole(),
                     QBrush(QPixmap::fromImage(out)));
    ui->toggleVisibilityPushButton->setPalette(palette);
}

void MainWindow::setupCalendar()
{
    ui->calendarWidget->setSelectedDate(QDate::currentDate());
}

void MainWindow::setupCredentials()
{
    ui->jiraUsernameLineEdit->setText(mSettings.username());
    ui->jiraTokenLineEdit->setText(mSettings.secret());
    ui->jiraHostnameLineEdit->setText(mSettings.hostname());
    if (mSettings.hasAvatar()) {
        this->setAvatar(mSettings.avatar());
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
                         if (mSettings.hasAvatar()) {
                             this->setAvatar(mSettings.avatar());
                         }
                         this->ui->jiraDisplayNameLabel->setText(mSettings.displayName());
                     });
    QObject::connect(ui->savePushButton, &QPushButton::clicked,
                     [this] {
                         mSettings.setUsername(this->ui->jiraUsernameLineEdit->text());
                         mSettings.setSecret(this->ui->jiraTokenLineEdit->text());
                         mSettings.setHostname(this->ui->jiraHostnameLineEdit->text());
                     });
    QObject::connect(ui->calendarWidget, &QCalendarWidget::selectionChanged,
                     [this] {
                         this->setCurrentDate(this->ui->calendarWidget->selectedDate());
                     });
    QObject::connect(&mMonthlyTotalCalculator, &MonthlyTotalCalculator::updated,
                     this, &MainWindow::monthlyTotalCalculatorUpdated);
    QObject::connect(&mWeeklyTotalCalculator, &WeeklyTotalCalculator::updated,
                     this, &MainWindow::weeklyTotalCalculatorUpdated);
    QObject::connect(dailyRegistrations, &DailyRegistrations::registrationAdded,
                     &mWeeklyTotalCalculator, &WeeklyTotalCalculator::update);
    QObject::connect(dailyRegistrations, &DailyRegistrations::registrationAdded,
                     &mMonthlyTotalCalculator, &MonthlyTotalCalculator::update);

    QObject::connect(ui->toggleVisibilityPushButton, &QPushButton::clicked,
                     this, &MainWindow::toggleVisbilityPushButtonClicked);
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event);
    mSettings.setWindowGeometry(saveGeometry());
}

void MainWindow::moveEvent(QMoveEvent *event)
{
    Q_UNUSED(event);
    mSettings.setWindowGeometry(saveGeometry());
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
        mAuthenticationStatusLabel->setText("🤘 Authenticated");
    } else {
        mAuthenticationStatusLabel->setText("🤬 Not authenticated");
    }
}

void MainWindow::toggleVisbilityPushButtonClicked()
{
    bool show = !mSettings.showCredentials();
    mSettings.setShowCredentials(show);
    showCredentials(show);
}

void MainWindow::showCredentials(bool visible) {

    updateVisibilityPushButtonIcon();

    ui->jiraHostnameLabel->setVisible(visible);
    ui->jiraTokenLabel->setVisible(visible);
    ui->jiraUsernameLabel->setVisible(visible);

    ui->jiraHostnameLineEdit->setVisible(visible);
    ui->jiraTokenLineEdit->setVisible(visible);
    ui->jiraUsernameLineEdit->setVisible(visible);

    ui->savePushButton->setVisible(visible);
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
