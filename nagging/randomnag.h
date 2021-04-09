#ifndef RANDOMNAG_H
#define RANDOMNAG_H

#include "nag.h"

class RandomNag : public Nag
{
public:
    RandomNag();
    virtual ~RandomNag();

    // Nag interface
public:
    QString message() const override;
    QString id() const override;
    QTime startTime() const override;
    QTime endTime() const override;
};

#endif // RANDOMNAG_H
