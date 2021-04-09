#include "randomnag.h"

RandomNag::RandomNag()
{

}

RandomNag::~RandomNag()
{

}


QString RandomNag::message() const
{
    return "Did you remember to log your work?";
}

QString RandomNag::id() const
{
    return "random";
}

QTime RandomNag::startTime() const
{
    return QTime(12, 0);
}

QTime RandomNag::endTime() const
{
    return QTime(13, 0);
}
