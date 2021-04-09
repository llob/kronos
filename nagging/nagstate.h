#ifndef NAGSTATE_H
#define NAGSTATE_H

#include <QVariant>
#include <QStringList>
#include <QDate>
#include "nag.h"

/**
 * @brief The NagState class tracks daily nagging state.
 *  Which nags have been performed, that kind of thing.
 */
class NagState {
private:
    QDate mDate;
    QStringList mCompletedNagIds;

    void gc();
public:
    NagState();
    /**
     * @brief toVariant Encode nag state as a QVariant
     * @return
     */
    QVariantMap toVariant() const;
    /**
     * @brief fromVariant Update state based on data from
     *  QVariantMap
     * @param state
     */
    void fromVariant(const QVariantMap state);
    /**
     * @brief date Current date for which we are nagging
     *  Nags are reset once the date rolls over.
     * @return
     */
    QDate date();
    /**
     * @brief completedNagIds The list of completed nag ids
     *  for today.
     * @return
     */
    QStringList completedNagIds();
    /**
     * @brief naggedAbout Determine whether we already
     *  nagged the user about this nag.
     * @param nag The nag
     * @return True if we nagged the user already. False otherwise.
     */
    bool naggedAbout(PNag nag);
    /**
     * @brief nagged Mark this nag as done for today
     * @param nag
     */
    void nagged(const PNag nag);
};

#endif // NAGSTATE_H
