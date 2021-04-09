#include "afternoonnag.h"

AfternoonNag::AfternoonNag()
{

}

AfternoonNag::~AfternoonNag()
{

}


QString AfternoonNag::message() const
{
    return "It's getting late. Consider logging your hours.";
}

QString AfternoonNag::id() const
{
    return "afternoon2";
}

QTime AfternoonNag::startTime() const
{
    return QTime(15, 0);
}

QTime AfternoonNag::endTime() const
{
    return QTime(16, 0);
}
