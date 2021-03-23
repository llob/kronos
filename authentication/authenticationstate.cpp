#include "authenticationstate.h"
#include "authenticationstateprivate.h"

AuthenticationState::AuthenticationState(QObject *parent) : QObject(parent)
{
    static AuthenticationStatePrivate* authenticationStatePrivate = new AuthenticationStatePrivate();
    mAuthenticationStatePrivate = authenticationStatePrivate;
    QObject::connect(mAuthenticationStatePrivate, &AuthenticationStatePrivate::stateChanged,
                     this, &AuthenticationState::stateChanged);
}

AuthenticationState::State AuthenticationState::state() const
{
    return mAuthenticationStatePrivate->state();
}

bool AuthenticationState::authenticated() const
{
    return state() == AuthenticationState::AUTHENTICATED;
}
