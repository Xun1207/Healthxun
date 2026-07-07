#include "DietService.h"

DietService::DietService()
{
}

bool DietService::addDiet(int userId, const QString& foodName, float weight, int mealType)
{
    int calorie = CalUtil::calcFoodIntake(foodName, weight);

    DietRecord record;
    record.userId = userId;
    record.foodName = foodName;
    record.weight = weight;
    record.calorie = calorie;
    record.mealType = mealType;
    record.recordTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

    return dao.addDietRecord(record);
}

bool DietService::updateDiet(int dietId, const QString& foodName, float weight, int mealType)
{
    int calorie = CalUtil::calcFoodIntake(foodName, weight);

    DietRecord record;
    record.dietId = dietId;
    record.foodName = foodName;
    record.weight = weight;
    record.calorie = calorie;
    record.mealType = mealType;

    return dao.updateDietRecord(record);
}

bool DietService::deleteDiet(int dietId)
{
    return dao.deleteDietRecord(dietId);
}

QList<DietRecord> DietService::queryDietByDate(int userId, const QString& startDate, const QString& endDate)
{
    return dao.queryByUserAndDate(userId, startDate, endDate);
}

int DietService::calcDayTotalIntake(int userId, const QString& date)
{
    return dao.calcDayTotalIntake(userId, date);
}