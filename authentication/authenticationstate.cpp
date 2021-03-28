#include "authenticationstate.h"
#include "authenticationstateprivate.h"

AuthenticationState::AuthenticationState()
{
    static QSharedPointer<AuthenticationStatePrivate> authenticationStatePrivate(new AuthenticationStatePrivate());
    mAuthenticationStatePrivate = authenticationStatePrivate;
    QObject::connect(mAuthenticationStatePrivate.data(), &AuthenticationStatePrivate::stateChanged,
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
