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
    mJiraClient = new JiraClient();
    ui->setupUi(this);
    ui->avatarLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    ui->avatarLabel->setStyleSheet("background-color: rgba(0,0,0,0%)");
    ui->avatarLabel->setAutoFillBackground(false);
    setupDailyRegistrations();
    setupConnections();
    setupCalendar();
    setupCredentials();
}

void MainWindow::setupCalendar()
{
    ui->calendarWidget->setSelectedDate(QDate::currentDate());
}

void MainWindow::setupCredentials()
{
    ui->jiraUsernameLineEdit->setText(mSettings.jiraUsername());
    ui->jiraTokenLineEdit->setText(mSettings.jiraToken());
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
    QObject::connect(&mSettings, &Settings::updated,
                     [this] {
                         if (mSettings.hasJiraAvatar()) {
                             this->setAvatar(mSettings.jiraAvatar());
                         }
                         this->ui->jiraDisplayNameLabel->setText(mSettings.jiraDisplayName());
                     });
    QObject::connect(ui->savePushButton, &QPushButton::clicked,
                     [this] {
                         mSettings.setJiraUsername(this->ui->jiraUsernameLineEdit->text());
                         mSettings.setJiraToken(this->ui->jiraTokenLineEdit->text());
                     });
    QObject::connect(ui->calendarWidget, &QCalendarWidget::selectionChanged,
                     [this] {
                         this->setCurrentDate(this->ui->calendarWidget->selectedDate());
                     });

}

void MainWindow::setCurrentDate(QDate currentDate)
{
    mCurrentDate = currentDate;
    dailyRegistrations->setCurrentDate(mCurrentDate);
}

void MainWindow::setupDailyRegistrations()
{
    auto dailyRegistrationsModel = new DailyRegistrationsModel(mJiraClient);
    this->dailyRegistrations = new DailyRegistrations(
        dailyRegistrationsModel,
        QDate::currentDate(),
        this);
    ui->scrollArea->setWidget(dailyRegistrations);
}
