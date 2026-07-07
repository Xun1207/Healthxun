#ifndef REMIND_H
#define REMIND_H

#include <QString>

class Remind
{
public:
    Remind();

    int remindId;
    int userId;
    QString remindType;
    QString remindTime;
    bool isOpen;
};

#endif