#include "settingsprivate.h"
#include <QMutexLocker>

SettingsPrivate::SettingsPrivate()
{
    mSettings = QSharedPointer<QSettings>(new QSettings("fork.dk", "kronos"));
}

QVariant SettingsPrivate::get(QString key, QVariant fallback) {
    QMutexLocker lock(&mMutex);
    return mSettings->value(key, fallback);
}
void SettingsPrivate::set(QString key, QVariant value)
{
    mMutex.lock();
    mSettings->setValue(key, value);
    mMutex.unlock();
    emit updated();
}
