#include "dailyworklogs.h"
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
#include "worklogdialog.h"

DailyWorklogs::DailyWorklogs(DailyWorklogsModel *model, QDate date, QWidget *parent) : QWidget(parent)
{
    mModel = model;
    mDate = date;
    setMinimumHeight(1200);
    mRegistrationDialog = new WorklogDialog(this);
    mDeleteConfirmationDialog = new DeleteConfirmationDialog(this);

    QObject::connect(this, &DailyWorklogs::worklogAdded,
                     mModel, &DailyWorklogsModel::addWorklog);
    QObject::connect(this, &DailyWorklogs::worklogDeleted,
                     mModel, &DailyWorklogsModel::deleteWorklog);
    QObject::connect(model, &DailyWorklogsModel::updated,
                     this, &DailyWorklogs::modelUpdated);
    QObject::connect(mRegistrationDialog, &QDialog::accepted,
        [this] {
            emit this->worklogAdded(
                             this->mRegistrationDialog->startTime(),
                             this->mRegistrationDialog->endTime(),
                             this->mRegistrationDialog->jiraIssue());
        });
    QObject::connect(mDeleteConfirmationDialog, &QDialog::accepted,
                     [this] {
                         emit this->worklogDeleted(mDeleteConfirmationDialog->worklog());
                     });
}

void DailyWorklogs::setCurrentDate(const QDate date)
{
    mDate = date;
    mModel->setCurrentDate(date);
}

QDate DailyWorklogs::currentDate() const
{
    return mDate;
}

void DailyWorklogs::setWorking(bool working)
{
    mWorking = working;
    repaint();
}

int DailyWorklogs::pixelsPerHour() {
    return height()/24;
}

void DailyWorklogs::drawRegistrationRect(QPaintEvent *event, QPoint topLeft, QPoint bottomRight, QString text=QString()) {
    Q_UNUSED(event);
    QPainter painter(this);
    QRect rect(topLeft, bottomRight);
    QPainterPath path;
    path.addRoundedRect(rect, 10, 10);
    QPen pen(Colors::khakiWeb(), 1);
    painter.setPen(pen);
    painter.setOpacity(1.0);
    painter.fillPath(path, Colors::radicalRed());
    painter.drawPath(path);

    painter.setPen(Colors::ivory());
    painter.setOpacity(1.0);
    painter.drawText(rect.adjusted(5, 5, -5, -5), Qt::AlignLeft|Qt::AlignTop, text);
}

QTime DailyWorklogs::round(const QTime time) {
    int pastRounding = time.minute() % 15;
    if (time.minute() % 15 > 7) {
        return time.addSecs(60 * (15 - pastRounding));
    } else {
        return time.addSecs(60 * -pastRounding);
    }
}

QTime DailyWorklogs::lastWorklogEndTimeBefore(QTime time) {
    QTime result(0, 0);
    foreach (QSharedPointer<JiraWorklog> worklog, mModel->worklogs()) {
        QTime worklogEndTime = worklog->started().addSecs(worklog->timeSpentSeconds()).time();
        if (worklogEndTime > time) {
            continue;
        }
        if (qAbs(time.secsTo(result)) > qAbs(time.secsTo(worklogEndTime))) {
            // This worklog ends later than any we saw previously
            result = worklogEndTime;
        }
    }
    return result;
}

QTime DailyWorklogs::firstWorklogStartTimeAfter(QTime time) {
    QTime result(23, 59);
    foreach (QSharedPointer<JiraWorklog> worklog, mModel->worklogs()) {
        QTime worklogStartTime = worklog->started().time();
        if (worklogStartTime < time) {
            continue;
        }
        if (qAbs(time.secsTo(result)) > qAbs(time.secsTo(worklogStartTime))) {
            // This worklog ends before any we saw previously
            result = worklogStartTime;
        }
    }
    return result;
}

void DailyWorklogs::drawRegistrationInProgress(QPaintEvent *event) {
    if (registrationInProgress()) {
        QTime startTime = timeFromPos(mRegistrationInProgressStartPos);
        QTime endTime = timeFromPos(mCurrentMousePos);

        // Determine if span from startTime to endTime collides
        // with an existing worklog, in which case we have to adjust
        // endTime to avoid overlapping
        if (startTime > endTime) {
            // Worklog was dragged from end time to start time
            QTime precedingWorklogEndTime = lastWorklogEndTimeBefore(startTime);
            endTime = qMax(precedingWorklogEndTime, endTime);
        } else {
            // Worklog was dragged from start time to end time
            QTime followingWorklogStartTime = firstWorklogStartTimeAfter(startTime);
            endTime = qMin(followingWorklogStartTime, endTime);
        }

        mRegistrationInProgressStartTime = round(qMin(startTime, endTime));
        mRegistrationInProgressEndTime = round(qMax(startTime, endTime));

        QPoint roundStartPos = posFromTime(mRegistrationInProgressStartTime);
        QPoint roundEndPos = posFromTime(mRegistrationInProgressEndTime);
        drawRegistrationRect(event,
                             QPoint(50, roundStartPos.y()),
                             QPoint(width() - 10, roundEndPos.y()));
    }
}

bool DailyWorklogs::registrationInProgress()
{
    return mRegistrationInProgressStartPos.y() != 0;
}

QSharedPointer<JiraWorklog> DailyWorklogs::worklogFromPos(QPoint pos)
{
    QTime t = timeFromPos(pos);
    foreach (QSharedPointer<JiraWorklog> worklog, mModel->worklogs()) {
        if (worklog->contains(QDateTime(mDate, t))) {
            return worklog;
        }
    }
    return nullptr;
}

QTime DailyWorklogs::timeFromPos(QPoint pos)
{
    float pph = static_cast<float>(pixelsPerHour());
    int hours = pos.y() / pph;
    int minutes = (pos.y() % pixelsPerHour()) / (pph/60.0);
    QTime result(hours, minutes);
    return result;
}

QPoint DailyWorklogs::posFromTime(QTime time)
{
    QPoint result(0, time.hour() * pixelsPerHour() + time.minute() * (pixelsPerHour()/60.0));
    return result;
}

void DailyWorklogs::drawTimes(QPaintEvent *event) {
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

void DailyWorklogs::drawRegistrations(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    foreach (QSharedPointer<JiraWorklog> worklog, mModel->worklogs()) {
        QDateTime started = worklog->started();
        QDateTime ended = started.addSecs(worklog->timeSpentSeconds());
        QPoint startedPos = posFromTime(started.time());
        QSharedPointer<AbstractIssue> issue = mModel->issueById(worklog->issueId());
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

void DailyWorklogs::drawWorkingOverlay(QPaintEvent *event) {
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

void DailyWorklogs::paintEvent(QPaintEvent *event)
{
    drawTimes(event);
    drawRegistrationInProgress(event);
    drawRegistrations(event);
    drawWorkingOverlay(event);
}

void DailyWorklogs::mousePressEvent(QMouseEvent *event)
{
    // Check if mouse was pressed inside an existing worklog
    if (event->button() == Qt::LeftButton) {
        if (!worklogFromPos(event->pos())) {
            mRegistrationInProgressStartPos = event->pos();
        }
    }
}

void DailyWorklogs::mouseReleaseEvent(QMouseEvent *event)
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
        mRegistrationInProgressStartPos = QPoint(); // Make a note that we are done with creating a new registration
        if (mRegistrationInProgressStartTime.secsTo(mRegistrationInProgressEndTime) > 0) {
            mRegistrationDialog->setRecentIssues(mModel->recentIssues());
            mRegistrationDialog->setDate(mDate);
            mRegistrationDialog->setStartTime(mRegistrationInProgressStartTime);
            mRegistrationDialog->setEndTime(mRegistrationInProgressEndTime);
            mRegistrationDialog->show();
        }
    }
}

void DailyWorklogs::mouseMoveEvent(QMouseEvent *event)
{
    if (mRegistrationInProgressStartPos.y() == 0) {
        return;
    }
    mCurrentMousePos = event->pos();
    repaint();
}

void DailyWorklogs::modelUpdated()
{
    repaint();
}
