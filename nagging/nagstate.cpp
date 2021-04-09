#include "nagstate.h"

void NagState::gc()
{
    QDate today = QDate::currentDate();
    if (today != mDate) {
        mDate = today;
        mCompletedNagIds.clear();
    }
}

NagState::NagState()
{

}

QVariantMap NagState::toVariant() const
{
    QVariantMap result;
    result.insert("date", mDate);
    result.insert("completedNagIds", mCompletedNagIds);
    return result;
}

void NagState::fromVariant(const QVariantMap state)
{
    if (state.contains("date")) {
        mDate = state.value("date").toDate();
    }
    if (state.contains("completedNagIds")) {
        mCompletedNagIds = state.value("completedNagIds").toStringList();
    }
}

QDate NagState::date()
{
    gc();
    return mDate;
}

QStringList NagState::completedNagIds()
{
    gc();
    return mCompletedNagIds;
}

bool NagState::naggedAbout(PNag nag)
{
    gc();
    return mCompletedNagIds.contains(nag->id());
}

void NagState::nagged(const PNag nag)
{
    gc();
    mCompletedNagIds.removeAll(nag->id());
    mCompletedNagIds.append(nag->id());
}
