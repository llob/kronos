#ifndef DAILYWORKLOGS_H
#define DAILYWORKLOGS_H

#include <QWidget>
#include <QSharedPointer>
#include <QTime>
#include <QDate>
#include "dailyworklogsmodel.h"
#include "worklogdialog.h"
#include "deleteconfirmationdialog.h"

class DailyWorklogs;

/**
 * @brief The WorklogRect class represents a single rectangle
 *  which in turn represents a worklog
 */
class WorklogRect : public QObject {

    friend DailyWorklogs;

public:
    enum State {
        DEFAULT,
        MOUSE_OVER
    };
private:
    DailyWorklogs *mDailyWorklogs;
    QSharedPointer<JiraWorklog> mWorklog;
    DailyWorklogsModel *mModel;
    State mState;
    QRect mCloseButtonRect;
    QPoint startedPos() const;
    QPoint endedPos() const;
    QRect rect() const;
    QRect closeButtonRect() const;
    void draw(QPaintEvent *event, QPoint topLeft, QPoint bottomRight, QString text, bool drawDeleteButton);
public:
    WorklogRect(QSharedPointer<JiraWorklog> worklog, DailyWorklogsModel *model, DailyWorklogs *dailyWorklogs);
    void paintEvent(QPaintEvent *event) ;
    void mousePressEvent(QMouseEvent *event) ;
    void mouseReleaseEvent(QMouseEvent *event) ;
    void mouseMoveEvent(QMouseEvent *event) ;
};

typedef QSharedPointer<WorklogRect> PRegistrationRect;

class DailyWorklogs : public QWidget
{
    friend WorklogRect;

    Q_OBJECT
private:
    void drawTimes(QPaintEvent *event);
    void drawRegistrationInProgress(QPaintEvent *event);
    bool registrationInProgress();
    QPoint mRegistrationInProgressStartPos;
    QTime mRegistrationInProgressStartTime;
    QTime mRegistrationInProgressEndTime;
    QSharedPointer<WorklogRect> mRegistrationInProgressRect;
    QPoint mCurrentMousePos;
    QDate mDate;
    DailyWorklogsModel *mModel;
    WorklogDialog *mRegistrationDialog;
    DeleteConfirmationDialog *mDeleteConfirmationDialog;
    QTime timeFromPos(QPoint pos);
    QPoint posFromTime(QTime time);
    QList<QSharedPointer<WorklogRect>> mRegistrationRects;

    int pixelsPerHour();
    void drawRegistrations(QPaintEvent *event);
    QSharedPointer<JiraWorklog> worklogFromPos(QPoint pos);
    QTime round(const QTime time);

    /**
     * @brief lastWorklogEndTimeBefore Get the end time of the last worklog
     *  which has endtime before the specified time
     * @param time The end endtime of the worklog, or 23:59 if none was found
     * @return
     */
    QTime lastWorklogEndTimeBefore(QTime time);
    /**
     * @brief firstWorklogStartTimeAfter Get the start time of the first worklog
     *  which starts after specified time
     * @param time Start time
     * @return Start time of located worklog or 00:00:00 if no matching worklogs were found
     */
    QTime firstWorklogStartTimeAfter(QTime time);
    void showDeleteConfirmationDialog(PJiraWorklog worklog);
public:
    explicit DailyWorklogs(DailyWorklogsModel *model, QDate date, QWidget *parent = nullptr);
    void setCurrentDate(const QDate date);
    QDate currentDate() const;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

signals:
    void worklogAdded(QTime startTime, QTime endTime, QSharedPointer<AbstractIssue> issue);
    void worklogDeleted(QSharedPointer<JiraWorklog> worklog);

private slots:
    void modelUpdated();
};

#endif // DAILYWORKLOGS_H
