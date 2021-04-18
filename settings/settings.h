#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QPixmap>
#include <QByteArray>
#include <QBuffer>
#include <QSharedPointer>
#include "settingsprivate.h"
#include "abstractmodels/abstractissue.h"

/**
 * @brief The Settings class manages data which is persisted
 *  between application restarts.
 *  This includes user controllable settings as well as cached
 *  data.
 *
 *  The updated() signal is emitted, if settings are changed via
 *  any instance of the Settings class.
 *
 *  The Settings class relies on the platform independent
 *  QSettings system for its heavy lifting.
 */
class Settings : public QObject
{
    Q_OBJECT
private:
    QSharedPointer<SettingsPrivate> mSettings;
    QByteArray imageToByteArray(const QImage image) const;
public:
    Settings();
    /**
     * @brief username The backend username
     * @return
     */
    QString username() const;
    /**
     * @brief setUsername Set the backend username
     * @param username
     */
    void setUsername(const QString username, bool suppressUpdate);
    /**
     * @brief secret Get the backend secret (password, token, etc.)
     * @return
     */
    QString secret() const;
    /**
     * @brief setSecret Set the backend secret
     * @param token
     */
    void setSecret(const QString token, bool suppressUpdate);
    /**
     * @brief accountId Get the backend account id
     * @return
     */
    QString accountId() const;
    /**
     * @brief setAccountId Set the backend account id
     * @param accountId
     */
    void setAccountId(const QString accountId) const;
    /**
     * @brief unsetAccountId Clear the stored account id
     */
    void unsetAccountId();
    /**
     * @brief avatar Get the avatar to use for logged in user
     * @return
     */
    QPixmap avatar() const;
    /**
     * @brief setAvatar Set the avatar to use for the current user
     * @param avatar
     */
    void setAvatar(const QImage avatar);
    /**
     * @brief unsetAvatar Unset the avatar image
     */
    void unsetAvatar();
    /**
     * @brief hasAvatar Determine if an avatar is available
     *  in the settings
     * @return
     */
    bool hasAvatar() const;
    /**
     * @brief setDisplayName The the displayed name of the current user
     * @param displayName
     */
    void setDisplayName(const QString displayName);
    /**
     * @brief unsetDisplayName Clear the stored displayname
     */
    void unsetDisplayName();
    /**
     * @brief displayName Get the displayed name of the current user
     * @return
     */
    QString displayName() const;
    /**
     * @brief hostname Get the backend hostname
     * @return
     */
    QString hostname() const;
    /**
     * @brief setHostname Set the backend hostname
     * @param jiraHostname
     */
    void setHostname(QString jiraHostname, bool suppressUpdate);
    /**
     * @brief recentIssues Get any stored recent issues
     * @return
     */
    QList<QSharedPointer<AbstractIssue> > recentIssues() const;
    /**
     * @brief setRecentIssues Set any stored recent issues
     * @param issues
     */
    void setRecentIssues(QList<QSharedPointer<AbstractIssue> > issues);
    /**
     * @brief setWindowGeometry Set main window geometry (obtained from QMainWindow::windowGeometry)
     * @param geometry
     */
    void setWindowGeometry(const QByteArray geometry);
    /**
     * @brief windowGeometry Retrieve stored main window geometry
     * @return
     */
    QByteArray windowGeometry() const;
    void setShowCredentials(bool show);
    bool showCredentials() const;

    /**
     * @brief nagState Retrieve stored nag state
     * @return
     */
    QVariantMap nagState() const;
    /**
     * @brief setNagState Set current nag state
     * @param state
     */
    void setNagState(QVariantMap state);

    /**
     * @brief clear Removes all settings values, thereby reverting to defaults
     */
    void clear();

signals:
    /**
     * @brief updated Emitted whenever a settings value has changed
     */
    void updated();
    /**
     * @brief credentialsUpdated Emitted when one of the credentials values are updated
     */
    void credentialsUpdated();
};

#endif // SETTINGS_H
