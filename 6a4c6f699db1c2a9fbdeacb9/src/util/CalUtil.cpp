#include "CalUtil.h"
#include "../dao/FoodStdDAO.h"

CalUtil::CalUtil()
{
}

float CalUtil::calcBMI(float height, float weight)
{
    if (height <= 0 || weight <= 0) {
        return 0;
    }
    float heightInMeters = height / 100.0f;
    return weight / (heightInMeters * heightInMeters);
}

int CalUtil::calcSportBurn(const QString& type, float duration)
{
    int perMinuteCal = 0;
    if (type == "跑步") {
        perMinuteCal = 10;
    } else if (type == "游泳") {
        perMinuteCal = 12;
    } else if (type == "瑜伽") {
        perMinuteCal = 4;
    } else if (type == "力量训练") {
        perMinuteCal = 7;
    } else if (type == "骑行") {
        perMinuteCal = 8;
    } else if (type == "跳绳") {
        perMinuteCal = 15;
    } else if (type == "步行") {
        perMinuteCal = 3;
    } else if (type == "篮球") {
        perMinuteCal = 9;
    } else if (type == "羽毛球") {
        perMinuteCal = 6;
    } else {
        perMinuteCal = 5;
    }
    return perMinuteCal * duration;
}

int CalUtil::calcFoodIntake(const QString& foodName, float weight)
{
    FoodStdDAO dao;
    FoodStandard food = dao.queryFoodByName(foodName);
    if (food.foodId == 0 || weight <= 0) {
        return 0;
    }
    return (food.per100gCal * weight) / 100;
}

int CalUtil::calcBaseMetabolism(const User& user)
{
    if (user.weight <= 0 || user.height <= 0 || user.age <= 0) {
        return 0;
    }

    if (user.gender == 1) {
        return 10 * user.weight + 6.25 * user.height - 5 * user.age + 5;
    } else {
        return 10 * user.weight + 6.25 * user.height - 5 * user.age - 161;
    }
}

int CalUtil::calcHeatSurplusGap(int intake, int burn, int base)
{
    return intake - burn - base;
}