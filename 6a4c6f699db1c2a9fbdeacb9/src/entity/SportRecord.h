#ifndef SPORTRECORD_H
#define SPORTRECORD_H

#include <QString>

class SportRecord
{
public:
    SportRecord();

    int recordId;
    int userId;
    QString sportType;
    float duration;
    float distance;
    int calorieBurn;
    QString recordDate;
};

#endif