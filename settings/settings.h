#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QPixmap>
#include <QByteArray>
#include <QBuffer>
#include "settingsprivate.h"

class Settings : public QObject
{
    Q_OBJECT
private:
    static SettingsPrivate *mSettings;
public:
    Settings();
    QString jiraUsername() const;
    void setJiraUsername(const QString username);
    QString jiraToken() const;
    void setJiraToken(const QString token);
    QString jiraAccountId() const;
    void setJiraAccountId(const QString accountId) const;
    QPixmap jiraAvatar() const;
    void setJiraAvatar(const QImage avatar);
    bool hasJiraAvatar() const;
    void setJiraDisplayName(const QString displayName);
    QString jiraDisplayName() const;

signals:
    void updated();
};

#endif // SETTINGS_H
