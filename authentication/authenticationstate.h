#ifndef AUTHENTICATIONSTATE_H
#define AUTHENTICATIONSTATE_H

#include <QObject>
#include <QSharedPointer>

class AuthenticationStatePrivate;

class AuthenticationState : public QObject
{
    Q_OBJECT

public:
    enum State {
        DEAUTHENTICATED,
        AUTHENTICATING,
        AUTHENTICATED
    };
    explicit AuthenticationState(QObject *parent = nullptr);
    State state() const;
    bool authenticated() const;
private:
    QSharedPointer<AuthenticationStatePrivate> mAuthenticationStatePrivate;
signals:
    void stateChanged(AuthenticationState::State oldState, AuthenticationState::State newState);
};

#endif // AUTHENTICATIONSTATE_H
