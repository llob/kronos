#ifndef SETTINGSPRIVATE_H
#define SETTINGSPRIVATE_H

#include <QObject>
#include <QSettings>
#include <QMutex>
#include <QSharedPointer>

/**
 * @brief The SettingsPrivate class manages low level
 *  storage of settings values based on QSettings.
 *
 *  Notifies all Settings instances when settings change
 *
 *  This class is thread safe.
 */
class SettingsPrivate : public QObject
{
    Q_OBJECT
private:
    QSharedPointer<QSettings> mSettings;
    QMutex mMutex;
public:
    SettingsPrivate();
    /**
     * @brief set Sets the settings value associated with a certain key
     * @param key The key
     * @param value The value
     * @param suppressUpdate If set to true, no update signal is emitted
     */
    void set(const QString key, const QVariant value, const bool suppressUpdate = false);
    /**
     * @brief setCredential Set a settings value which constitutes part of credentials
     * @param key The key
     * @param value The value
     * @param suppresaUpdate Whether to suppress the credentialsUpdated signal
     */
    void setCredential(const QString key, const QVariant value, const bool suppressUpdate);
    /**
     * @brief get Retrieve the value associated with a specific key
     * @param key The key
     * @return The value or an invalid QVariant if no value is stored for key
     */
    QVariant get(const QString key, const QVariant fallback = QVariant());
    /**
     * @brief clear Clears all stored settings values
     */
    void clear();
    /**
     * @brief clear Clear value stored under key, revert to default value
     * @param key
     */
    void clear(const QString key);
    /**
     * @brief has Determine whether a key exists in settings
     * @param key
     * @return True if key exists. False otherwise.
     */
    bool has(const QString key);
signals:
    /**
     * @brief updated Emitted whenever a settings value is updated
     */
    void updated();
    /**
     * @brief credentialsUpdated Emitted when one of the credentials values change
     */
    void credentialsUpdated();
};

#endif // SETTINGSPRIVATE_H
