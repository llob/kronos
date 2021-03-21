#ifndef SETTINGSPRIVATE_H
#define SETTINGSPRIVATE_H

#include <QObject>
#include <QSettings>
#include <QMutex>

class SettingsPrivate : public QObject
{
    Q_OBJECT
private:
    QSettings *mSettings;
    QMutex mMutex;
public:
    SettingsPrivate();
    void set(QString key, QVariant value);
    QVariant get(QString key);
signals:
    void updated();
};

#endif // SETTINGSPRIVATE_H
