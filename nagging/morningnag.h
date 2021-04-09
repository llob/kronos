#ifndef MORNINGNAG_H
#define MORNINGNAG_H

#include "nag.h"

class MorningNag : public Nag
{
public:
    MorningNag();
    virtual ~MorningNag();

    // Nag interface
public:
    QString message() const override;
    QString id() const override;
    QTime startTime() const override;
    QTime endTime() const override;
};

#endif // MORNINGNAG_H
