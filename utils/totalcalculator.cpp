#include "totalcalculator.h"

void TotalCalculator::search(QString query)
{
    // This TotalCalculator instance deletes itself once it has completed its calculations
    TotalCalculatorPrivate *calculator = new TotalCalculatorPrivate(query, firstDate());
    QObject::connect(calculator, &TotalCalculatorPrivate::updated,
                     this, &TotalCalculator::updated);
    calculator->calculate();
}

TotalCalculator::TotalCalculator() : QObject()
{
    QObject::connect(&mAuthenticationState, &AuthenticationState::stateChanged,
        [this] (AuthenticationState::State oldState, AuthenticationState::State newState, const QString message) {
            Q_UNUSED(oldState);
            Q_UNUSED(message);
            if (newState == AuthenticationState::AUTHENTICATED) {
                this->update();
            }
        });
}


TotalCalculator::~TotalCalculator()
{
}
