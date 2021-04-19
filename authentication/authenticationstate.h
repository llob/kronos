#ifndef AUTHENTICATIONSTATE_H
#define AUTHENTICATIONSTATE_H

#include <QObject>
#include <QSharedPointer>

class AuthenticationStatePrivate;

/**
 * @brief The AuthenticationState class tracks the current
 *  authentication state of the backend in use. This can be
 *  used throughout the application to react to changes in
 *  authentication state.
 *
 *  Instances of this class can be freely created and destroyed,
 *  as state is tracked by a private global object instance behind
 *  the scenes.
 *
 *  @see AuthenticationStatePrivate
 */
class AuthenticationState : public QObject
{
    Q_OBJECT

public:
    enum State {
        DEAUTHENTICATED,
        AUTHENTICATING,
        AUTHENTICATED
    };
    explicit AuthenticationState();
    /**
     * @brief state Get the current authentication state
     * @return
     */
    State state() const;
    /**
     * @brief authenticated Determine if application is authenticated
     *  with the backend
     * @return
     */
    bool authenticated() const;
private:
    QSharedPointer<AuthenticationStatePrivate> mAuthenticationStatePrivate;
signals:
    /**
     * @brief stateChanged Emitted when the authentication state changes
     * @param oldState The previous authentication state
     * @param newState The new (current) authentication state
     */
    void stateChanged(AuthenticationState::State oldState, AuthenticationState::State newState, const QString message);
};

#endif // AUTHENTICATIONSTATE_H
