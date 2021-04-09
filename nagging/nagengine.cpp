#include "nagengine.h"
#include "morningnag.h"
#include "afternoonnag.h"
#include "randomnag.h"

NagEngine::NagEngine() : QObject()
{
    setupNags();
    setupConnections();
    mNagState.fromVariant(mSettings.nagState());
    //mNagTimer.start(1000 * 60 * 10);
    mNagTimer.start(1000);
}


void NagEngine::setupConnections()
{
    QObject::connect(&mNagTimer, &QTimer::timeout,
                     this, &NagEngine::nagTimerTimeout);
}

void NagEngine::setupNags()
{
    mNags.append(PNag(new MorningNag()));
    mNags.append(PNag(new RandomNag()));
    mNags.append(PNag(new AfternoonNag()));
}

void NagEngine::nagTimerTimeout()
{
    QTime now = QTime::currentTime();
    foreach (PNag n, mNags) {
        if (mNagState.naggedAbout(n)) {
            // We already did this nag today
            continue;
        }
        if (now < n->startTime() || now > n->endTime()) {
            // Nag not relevant now
            continue;
        }

        mNagState.nagged(n);
        emit nag(n->message());
    }

    // Persist current nag state
    mSettings.setNagState(mNagState.toVariant());
}

