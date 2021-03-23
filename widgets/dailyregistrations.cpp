#include "dailyregistrations.h"
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QSizePolicy>
#include <QRect>
#include <QDateTime>
#include <QScrollArea>
#include <QTime>
#include "utils/colors.h"
#include "jira/jiraworklog.h"
#include "registrationdialog.h"

DailyRegistrations::DailyRegistrations(DailyRegistrationsModel *model, QDate date, QWidget *parent) : QWidget(parent)
{
    mModel = model;
    mDate = date;
    setMinimumHeight(1200);
    mRegistrationDialog = new RegistrationDialog(this);
    mDeleteConfirmationDialog = new DeleteConfirmationDialog(this);

    QObject::connect(this, &DailyRegistrations::registrationAdded,
                     mModel, &DailyRegistrationsModel::createRegistration);
    QObject::connect(this, &DailyRegistrations::registrationDeleted,
                     mModel, &DailyRegistrationsModel::deleteRegistration);
    QObject::connect(model, &DailyRegistrationsModel::updated,
                     this, &DailyRegistrations::modelUpdated);
    QObject::connect(mRegistrationDialog, &QDialog::accepted,
        [this] {
            emit this->registrationAdded(
                             this->mRegistrationDialog->startTime(),
                             this->mRegistrationDialog->endTime(),
                             this->mRegistrationDialog->jiraIssue());
        });
    QObject::connect(mDeleteConfirmationDialog, &QDialog::accepted,
                     [this] {
                         emit this->registrationDeleted(mDeleteConfirmationDialog->worklog());
                     });
}

void DailyRegistrations::setCurrentDate(const QDate date)
{
    mDate = date;
    mModel->setCurrentDate(date);
}

QDate DailyRegistrations::currentDate() const
{
    return mDate;
}

void DailyRegistrations::setWorking(bool working)
{
    mWorking = working;
    repaint();
}

int DailyRegistrations::pixelsPerHour() {
    return height()/24;
}

void DailyRegistrations::drawRegistrationRect(QPaintEvent *event, QPoint topLeft, QPoint bottomRight, QString text=QString()) {
    Q_UNUSED(event);
    QPainter painter(this);
    QRect rect(topLeft, bottomRight);
//    painter.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.addRoundedRect(rect, 10, 10);
    QPen pen(Colors::color1(), 1);
    painter.setPen(pen);
    painter.setOpacity(1.0);
    painter.fillPath(path, Colors::color2());
    painter.drawPath(path);

    painter.setPen(Colors::color5());
    painter.setOpacity(1.0);
    painter.drawText(rect.adjusted(5, 5, -5, -5), Qt::AlignLeft|Qt::AlignTop, text);
}

QTime DailyRegistrations::round(const QTime time) {
    int pastRounding = time.minute() % 15;
    if (time.minute() % 15 > 7) {
        return time.addSecs(60 * (15 - pastRounding));
    } else {
        return time.addSecs(60 * -pastRounding);
    }
}

void DailyRegistrations::drawRegistrationInProgress(QPaintEvent *event) {
    if (registrationInProgress()) {
        QPoint roundStartPos = posFromTime(round(timeFromPos(mRegistrationInProgressStartPos)));
        QPoint roundEndPos = posFromTime(round(timeFromPos(mCurrentMousePos)));
        drawRegistrationRect(event,
                             QPoint(50, roundStartPos.y()),
                             QPoint(width() - 10, roundEndPos.y()));
    }
}

bool DailyRegistrations::registrationInProgress()
{
    return mRegistrationInProgressStartPos.y() != 0;
}

QSharedPointer<JiraWorklog> DailyRegistrations::worklogFromPos(QPoint pos)
{
    QTime t = timeFromPos(pos);
    foreach (QSharedPointer<JiraWorklog> worklog, mModel->worklogs()) {
        if (worklog->contains(QDateTime(mDate, t))) {
            return worklog;
        }
    }
    return nullptr;
}

QTime DailyRegistrations::timeFromPos(QPoint pos)
{
    float pph = static_cast<float>(pixelsPerHour());
    int hours = pos.y() / pph;
    int minutes = (pos.y() % pixelsPerHour()) / (pph/60.0);
    QTime result(hours, minutes);
    return result;
}

QPoint DailyRegistrations::posFromTime(QTime time)
{
    QPoint result(0, time.hour() * pixelsPerHour() + time.minute() * (pixelsPerHour()/60.0));
    return result;
}

void DailyRegistrations::drawTimes(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    int currentWidth = width();
    int currentPixelsPerHour = pixelsPerHour();
    // Make room for all 24 hours
    for (int i=0; i < 24; i++) {
        int yOffset = currentPixelsPerHour * i;
        painter.drawText(QPoint(10, yOffset), QString("%1:00").arg(i));
        painter.drawLine(QPoint(50, yOffset), QPoint(currentWidth, yOffset));
    }
}

void DailyRegistrations::drawRegistrations(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    foreach (QSharedPointer<JiraWorklog> worklog, mModel->worklogs()) {
        QDateTime started = worklog->started();
        QDateTime ended = started.addSecs(worklog->timeSpentSeconds());
        QPoint startedPos = posFromTime(started.time());
        QSharedPointer<JiraIssue> issue = mModel->issueById(worklog->issueId());
        if (issue.isNull()) {
            qWarning() << "Failed to retrieve issue with id" << worklog->issueId();
            continue;
        }
        startedPos.setX(50);
        QPoint endedPos = posFromTime(ended.time());
        endedPos.setX(width() - 10);
        drawRegistrationRect(event, startedPos, endedPos, issue->toString());
    }
}

void DailyRegistrations::drawWorkingOverlay(QPaintEvent *event) {
    Q_UNUSED(event);
    return;
    if (!mWorking) {
        return;
    }
    QPainter painter(this);
    painter.setOpacity(0.8);
    painter.fillRect(rect(), Qt::black);
    QPen pen;
    pen.setColor(Qt::white);
    pen.setWidth(5);
    painter.setPen(pen);
    painter.drawEllipse(100, 100, 100, 100);

    pen.setColor(Qt::black);
    painter.setPen(pen);
    painter.drawArc(100, 100, 100, 100, 0, 320);
}

void DailyRegistrations::paintEvent(QPaintEvent *event)
{
    drawTimes(event);
    drawRegistrationInProgress(event);
    drawRegistrations(event);
    drawWorkingOverlay(event);
}

void DailyRegistrations::mousePressEvent(QMouseEvent *event)
{
    // Check if mouse was pressed inside an existing worklog
    if (event->button() == Qt::LeftButton) {
        if (!worklogFromPos(event->pos())) {
            mRegistrationInProgressStartPos = event->pos();
        }
    }
}

void DailyRegistrations::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        auto worklog = worklogFromPos(event->pos());
        if (worklog) {
            // Clicked in a worklog, so handle that
            QSharedPointer<JiraWorklog> worklog = worklogFromPos(event->pos());
            mDeleteConfirmationDialog->setWorklog(worklog);
            mDeleteConfirmationDialog->show();
        }
    } else if (event->button() == Qt::LeftButton) {
        if (mRegistrationInProgressStartPos.isNull()) {
            return;
        }
        QPoint registrationInProgressEndPos = event->pos();
        QTime startTime;
        QTime endTime;
        if (registrationInProgressEndPos.y() > mRegistrationInProgressStartPos.y()) {
            // Start pos is before end pos
            startTime = timeFromPos(mRegistrationInProgressStartPos);
            endTime = timeFromPos(registrationInProgressEndPos);
        } else {
            // End pos is before start pos
            startTime = timeFromPos(registrationInProgressEndPos);
            endTime = timeFromPos(mRegistrationInProgressStartPos);
        }
        mRegistrationInProgressStartPos = QPoint(); // Make a note that we are done with creating a new registration
        if (startTime.secsTo(endTime) > 0) {
            mRegistrationDialog->setRecentIssues(mModel->recentIssues());
            mRegistrationDialog->setDate(mDate);
            mRegistrationDialog->setStartTime(round(startTime));
            mRegistrationDialog->setEndTime(round(endTime));
            mRegistrationDialog->show();
        }
    }
}

void DailyRegistrations::mouseMoveEvent(QMouseEvent *event)
{
    if (mRegistrationInProgressStartPos.y() == 0) {
        return;
    }
    mCurrentMousePos = event->pos();
    repaint();
}

void DailyRegistrations::modelUpdated()
{
    repaint();
}
