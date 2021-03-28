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
     */
    void set(QString key, QVariant value);
    /**
     * @brief get Retrieve the value associated with a specific key
     * @param key The key
     * @return The value or an invalid QVariant if no value is stored for key
     */
    QVariant get(QString key);
signals:
    /**
     * @brief updated Emitted whenever a settings value is updated
     */
    void updated();
};

#endif // SETTINGSPRIVATE_H
