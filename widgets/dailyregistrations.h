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
    QPoint mCurrentMousePos;
    QDate mDate;
    DailyRegistrationsModel *mModel;
    RegistrationDialog *mRegistrationDialog;
    DeleteConfirmationDialog *mDeleteConfirmationDialog;

    QTime timeFromPos(QPoint pos);
    QPoint posFromTime(QTime time);

    int pixelsPerHour();
    void drawRegistrations(QPaintEvent *event);
    void drawRegistrationRect(QPaintEvent *event, QPoint topLeft, QPoint bottomRight, QString text);
    QSharedPointer<JiraWorklog> worklogFromPos(QPoint pos);
    QTime round(const QTime time);
public:
    explicit DailyRegistrations(DailyRegistrationsModel *model, QDate date, QWidget *parent = nullptr);
    void setCurrentDate(const QDate date);
    QDate currentDate() const;

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
