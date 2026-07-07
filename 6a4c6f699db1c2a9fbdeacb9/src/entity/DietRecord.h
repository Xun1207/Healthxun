#ifndef DIETRECORD_H
#define DIETRECORD_H

#include <QString>

class DietRecord
{
public:
    DietRecord();

    int dietId;
    int userId;
    QString foodName;
    float weight;
    int calorie;
    int mealType;
    QString recordTime;
    int foodId;
};

#endif