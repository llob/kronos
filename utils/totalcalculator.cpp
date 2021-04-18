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
                     this, &TotalCalculator::update);

}


TotalCalculator::~TotalCalculator()
{
}
