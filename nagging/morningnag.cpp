#include "morningnag.h"

MorningNag::MorningNag()
{

}

MorningNag::~MorningNag()
{

}


QString MorningNag::message() const
{
    return "Do the world a favor, remember to log your work.";
}

QString MorningNag::id() const
{
    return "morning";
}

QTime MorningNag::startTime() const
{
    return QTime(8, 0);
}

QTime MorningNag::endTime() const
{
    return QTime(10, 0);
}
