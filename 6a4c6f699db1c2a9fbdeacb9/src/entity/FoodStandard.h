#ifndef FOODSTANDARD_H
#define FOODSTANDARD_H

#include <QString>

class FoodStandard
{
public:
    FoodStandard();

    int foodId;
    QString foodName;
    int per100gCal;
};

#endif