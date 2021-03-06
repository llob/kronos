#include "settingsprivate.h"
#include <QMutexLocker>

SettingsPrivate::SettingsPrivate()
{
    mSettings = QSharedPointer<QSettings>(new QSettings("fork.dk", "kronos"));
}

bool SettingsPrivate::has(const QString key)
{
    QMutexLocker lock(&mMutex);
    return mSettings->contains(key);
}

QVariant SettingsPrivate::get(const QString key, const QVariant fallback)
{
    QMutexLocker lock(&mMutex);
    return mSettings->value(key, fallback);
}

void SettingsPrivate::clear()
{
    QMutexLocker lock(&mMutex);
    mSettings->clear();
}

void SettingsPrivate::clear(const QString key)
{
    QMutexLocker lock(&mMutex);
    mSettings->remove(key);
}

void SettingsPrivate::set(const QString key, const QVariant value, const bool suppressUpdate)
{
    mMutex.lock();
    mSettings->setValue(key, value);
    mMutex.unlock();
    if (!suppressUpdate) {
        emit updated();
    }
}

void SettingsPrivate::setCredential(const QString key, const QVariant value, const bool suppressUpdate)
{
    set(key, value);
    if (!suppressUpdate) {
        emit credentialsUpdated();
    }
}
