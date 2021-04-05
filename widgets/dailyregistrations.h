#ifndef DAILYREGISTRATIONS_H
#define DAILYREGISTRATIONS_H

#include <QWidget>
#include <QSharedPointer>
#include <QTime>
#include <QDate>
#include "dailyregistrationsmodel.h"
#include "registrationdialog.h"
#include "deleteconfirmationdialog.h"

class DailyRegistrations : public QWidget
{
    Q_OBJECT
private:
    void drawTimes(QPaintEvent *event);
    void drawRegistrationInProgress(QPaintEvent *event);
    bool registrationInProgress();
    QPoint mRegistrationInProgressStartPos;
    QTime mRegistrationInProgressStartTime;
    QTime mRegistrationInProgressEndTime;
    QPoint mCurrentMousePos;
    QDate mDate;
    DailyRegistrationsModel *mModel;
    RegistrationDialog *mRegistrationDialog;
    DeleteConfirmationDialog *mDeleteConfirmationDialog;
    bool mWorking;

    QTime timeFromPos(QPoint pos);
    QPoint posFromTime(QTime time);

    int pixelsPerHour();
    void drawRegistrations(QPaintEvent *event);
    void drawRegistrationRect(QPaintEvent *event, QPoint topLeft, QPoint bottomRight, QString text);
    QSharedPointer<JiraWorklog> worklogFromPos(QPoint pos);
    QTime round(const QTime time);
    void drawWorkingOverlay(QPaintEvent *event);
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
public:
    explicit DailyRegistrations(DailyRegistrationsModel *model, QDate date, QWidget *parent = nullptr);
    void setCurrentDate(const QDate date);
    QDate currentDate() const;
    void setWorking(bool working);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

signals:
    void registrationAdded(QTime startTime, QTime endTime, QSharedPointer<JiraIssue> issue);
    void registrationDeleted(QSharedPointer<JiraWorklog> worklog);

private slots:
    void modelUpdated();
};

#endif // DAILYREGISTRATIONS_H
