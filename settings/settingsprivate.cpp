#include "settingsprivate.h"
#include <QMutexLocker>
#include <QMutex>

SettingsPrivate::SettingsPrivate()
{
    mSettings = new QSettings("fork.dk", "register-time");
}

QVariant SettingsPrivate::get(QString key) {
    QMutexLocker lock(&mMutex);
    return mSettings->value(key);
}
void SettingsPrivate::set(QString key, QVariant value)
{
    mMutex.lock();
    mSettings->setValue(key, value);
    mMutex.unlock();
    emit updated();
}
