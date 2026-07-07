#ifndef FOODSTDDAO_H
#define FOODSTDDAO_H

#include "../entity/FoodStandard.h"
#include <QList>

class FoodStdDAO
{
public:
    bool addFood(const FoodStandard& food);
    bool updateFoodCal(const FoodStandard& food);
    bool deleteFood(int foodId);
    QList<FoodStandard> queryAllFoodStd();
    FoodStandard queryFoodByName(const QString& foodName);
};

#endif