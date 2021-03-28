#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QPixmap>
#include <QByteArray>
#include <QBuffer>
#include <QSharedPointer>
#include "settingsprivate.h"

class JiraIssue;

/**
 * @brief The Settings class manages data which is persisted
 *  between application restarts.
 *  This includes user controllable settings as well as cached
 *  data.
 *
 *  The Settings class relies on the platform independent
 *  QSettings system for its heavy lifting.
 */
class Settings : public QObject
{
    Q_OBJECT
private:
    QSharedPointer<SettingsPrivate> mSettings;
public:
    Settings();
    QString username() const;
    void setUsername(const QString username);
    QString jiraToken() const;
    void setJiraToken(const QString token);
    QString jiraAccountId() const;
    void setJiraAccountId(const QString accountId) const;
    QPixmap jiraAvatar() const;
    void setJiraAvatar(const QImage avatar);
    bool hasJiraAvatar() const;
    void setJiraDisplayName(const QString displayName);
    QString jiraDisplayName() const;
    QString jiraHostname() const;
    void setJiraHostname(QString jiraHostname);
    QList<QSharedPointer<JiraIssue>> recentIssues() const;
    void setRecentIssues(QList<QSharedPointer<JiraIssue>> issues);

signals:
    void updated();
};

#endif // SETTINGS_H
