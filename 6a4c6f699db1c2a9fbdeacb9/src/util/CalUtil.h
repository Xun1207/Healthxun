#ifndef CALUTIL_H
#define CALUTIL_H

#include <QString>
#include "../entity/User.h"

class CalUtil
{
public:
    static float calcBMI(float height, float weight);
    static int calcSportBurn(const QString& type, float duration);
    static int calcFoodIntake(const QString& foodName, float weight);
    static int calcBaseMetabolism(const User& user);
    static int calcHeatSurplusGap(int intake, int burn, int base);

private:
    CalUtil();
};

#endif