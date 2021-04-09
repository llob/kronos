#ifndef AFTERNOONNAG_H
#define AFTERNOONNAG_H

#include "nag.h"

class AfternoonNag : public Nag
{
public:
    AfternoonNag();
    virtual ~AfternoonNag();

    // Nag interface
public:
    QString message() const override;
    QString id() const override;
    QTime startTime() const override;
    QTime endTime() const override;
};

#endif // AFTERNOONNAG_H
