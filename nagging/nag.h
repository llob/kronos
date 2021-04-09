#ifndef NAG_H
#define NAG_H

#include <QString>
#include <QSharedPointer>
#include <QTime>

/**
 * @brief The Nag class Super class for nags.
 *  Each subclass represents a type of nag. A nag
 *  will occur once a day between startTime() and endTime()
 */
class Nag {
public:
    Nag();
    virtual QString message() const = 0;
    virtual QString id() const = 0;
    virtual QTime startTime() const = 0;
    virtual QTime endTime() const = 0;
};

typedef QSharedPointer<Nag> PNag;

#endif // NAG_H
