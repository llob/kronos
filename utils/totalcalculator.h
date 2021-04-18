#ifndef TOTALCALCULATOR_H
#define TOTALCALCULATOR_H

#include <QObject>

#include "totalcalculatorprivate.h"
#include "authentication/authenticationstate.h"
#include "settings/settings.h"

class TotalCalculator : public QObject
{
    Q_OBJECT
private:
    virtual QDate firstDate() const = 0;
    AuthenticationState mAuthenticationState;
protected:
    void search(QString query);
    Settings mSettings;
public:
    explicit TotalCalculator();
    virtual ~TotalCalculator();
    virtual void update() = 0;

signals:
    /**
     * @brief updated Emitted whenever the total has been recalculated
     * @param seconds The number of seconds logged
     */
    void updated(int seconds);
};

#endif // TOTALCALCULATOR_H
