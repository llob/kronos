#include "mainwindow.h"
#include <QBoxLayout>
#include <QGraphicsSvgItem>
#include <QSvgRenderer>
#include <QPainter>
#include <QPainterPath>
#include <QPixmap>
#include <QRegExp>
#include "ui_mainwindow.h"
#include "widgets/dailyworklogs.h"
#include "settings/settings.h"
#include "utils/colors.h"
#include "widgets/kronoscalendarwidget.h"
#include "utils/images.h"

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

    setupCalendarWidget();
    setupDailyRegistrations();
    setupCredentials();
    setupConnections();

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

void MainWindow::setupCredentials()
{
    ui->jiraUsernameLineEdit->setText(mSettings.username());
    ui->jiraTokenLineEdit->setText(mSettings.secret());
    ui->jiraHostnameLineEdit->setText(mSettings.hostname());
    ui->jiraHostnameLineEdit->setValidator(new JiraHostnameValidator());
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
    QObject::connect(ui->savePushButton, &QPushButton::clicked,
                     [this] {
                         mSettings.setUsername(this->ui->jiraUsernameLineEdit->text(), true);
                         mSettings.setSecret(this->ui->jiraTokenLineEdit->text(), true);
                         mSettings.setHostname(this->ui->jiraHostnameLineEdit->text(), false);
                     });
    QObject::connect(mCalendarWidget, &QCalendarWidget::selectionChanged,
                     [this] {
                         this->setCurrentDate(mCalendarWidget->selectedDate());
                     });
    QObject::connect(&mMonthlyTotalCalculator, &MonthlyTotalCalculator::updated,
                     this, &MainWindow::monthlyTotalCalculatorUpdated);
    QObject::connect(&mWeeklyTotalCalculator, &WeeklyTotalCalculator::updated,
                     this, &MainWindow::weeklyTotalCalculatorUpdated);
    QObject::connect(mDailyWorklogs, &DailyWorklogs::worklogAdded,
                     &mWeeklyTotalCalculator, &WeeklyTotalCalculator::update);
    QObject::connect(mDailyWorklogs, &DailyWorklogs::worklogDeleted,
                     &mWeeklyTotalCalculator, &WeeklyTotalCalculator::update);
    QObject::connect(mDailyWorklogs, &DailyWorklogs::worklogAdded,
                     &mMonthlyTotalCalculator, &MonthlyTotalCalculator::update);
    QObject::connect(mDailyWorklogs, &DailyWorklogs::worklogDeleted,
                     &mMonthlyTotalCalculator, &MonthlyTotalCalculator::update);

    QObject::connect(mDailyWorklogs, &DailyWorklogs::worklogAdded,
        [this] () {
            mCalendarWidget->updateWorklogData(mDailyWorklogs->currentDate());
        });
    QObject::connect(mDailyWorklogs, &DailyWorklogs::worklogDeleted,
        [this] () {
            mCalendarWidget->updateWorklogData(mDailyWorklogs->currentDate());
        });


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

void MainWindow::setupCalendarWidget()
{
    mCalendarWidget = new KronosCalendarWidget();
    mCalendarWidget->setSelectedDate(QDate::currentDate());
    ui->calendarWidgetVerticalLayout->addWidget(mCalendarWidget);
}

void MainWindow::setCurrentDate(QDate currentDate)
{
    mCurrentDate = currentDate;
    mDailyWorklogs->setCurrentDate(mCurrentDate);
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

void MainWindow::authenticationStateChanged(AuthenticationState::State oldState, AuthenticationState::State newState, const QString message)
{
    Q_UNUSED(oldState);
    switch (newState)
    {
    case AuthenticationState::AUTHENTICATED:
        mAuthenticationStatusLabel->setText("🤘 Authenticated");
        ui->jiraDisplayNameLabel->setText(mSettings.displayName());
        break;
    case AuthenticationState::AUTHENTICATING:
        mAuthenticationStatusLabel->setText(QString("🤞 Authenticating"));
        break;
    case AuthenticationState::DEAUTHENTICATED:
        ui->jiraDisplayNameLabel->setText("No user authenticated");
        mAuthenticationStatusLabel->setText(QString("🤬 Not authenticated: %1").arg(message));
        break;
    }
    this->setAvatar(mSettings.avatar());
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
    auto dailyRegistrationsModel = new DailyWorklogsModel(mJiraClient);
    this->mDailyWorklogs = new DailyWorklogs(
        dailyRegistrationsModel,
        QDate::currentDate(),
        ui->scrollArea);
    ui->scrollArea->setWidget(mDailyWorklogs);
    ui->scrollArea->ensureVisible(0, 300);
}


QValidator::State JiraHostnameValidator::validate(QString &hostname, int &) const
{
    // Technically, this does not match the only
    // the list of valid hostnames, but it is good
    // enough, and users do weird things in their local
    // DNS setups, so we don't want to get in way of that
    QRegExp re("^[\\w\\.\\-]{0,255}$");
    if (re.exactMatch(hostname)) {
        return QValidator::Acceptable;
    } else {
        return QValidator::Invalid;
    }
}
