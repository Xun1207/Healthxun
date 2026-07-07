#include "DietService.h"

DietService::DietService()
{
}

bool DietService::addDiet(int userId, const QString& foodName, float weight, int mealType, const QString& recordTime)
{
    int calorie = CalUtil::calcFoodIntake(foodName, weight);

    DietRecord record;
    record.userId = userId;
    record.foodName = foodName;
    record.weight = weight;
    record.calorie = calorie;
    record.mealType = mealType;
    record.recordTime = recordTime.isEmpty() ? QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") : recordTime;

    return dao.addDietRecord(record);
}

bool DietService::updateDiet(int dietId, const QString& foodName, float weight, int mealType, const QString& recordTime)
{
    int calorie = CalUtil::calcFoodIntake(foodName, weight);

    DietRecord record;
    record.dietId = dietId;
    record.foodName = foodName;
    record.weight = weight;
    record.calorie = calorie;
    record.mealType = mealType;
    if (!recordTime.isEmpty()) {
        record.recordTime = recordTime;
    }

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

QList<DietRecord> DietService::queryDietByCycle(int userId, int cycleType)
{
    QDate today = QDate::currentDate();
    QString startDate, endDate;

    if (cycleType == 1) {
        startDate = today.toString("yyyy-MM-dd");
        endDate = today.toString("yyyy-MM-dd");
    } else if (cycleType == 2) {
        int dayOfWeek = today.dayOfWeek();
        startDate = today.addDays(1 - dayOfWeek).toString("yyyy-MM-dd");
        endDate = today.addDays(7 - dayOfWeek).toString("yyyy-MM-dd");
    } else {
        startDate = today.toString("yyyy-MM-01");
        endDate = today.toString("yyyy-MM-dd");
    }

    return dao.queryByUserAndDate(userId, startDate, endDate);
}

int DietService::calcDayTotalIntake(int userId, const QString& date)
{
    return dao.calcDayTotalIntake(userId, date);
}

int DietService::calcDateRangeTotalIntake(int userId, const QString& startDate, const QString& endDate)
{
    return dao.calcDateRangeTotalIntake(userId, startDate, endDate);
}

int DietService::calcCycleTotalIntake(int userId, int cycleType)
{
    QDate today = QDate::currentDate();
    QString startDate, endDate;

    if (cycleType == 1) {
        startDate = today.toString("yyyy-MM-dd");
        endDate = today.toString("yyyy-MM-dd");
    } else if (cycleType == 2) {
        int dayOfWeek = today.dayOfWeek();
        startDate = today.addDays(1 - dayOfWeek).toString("yyyy-MM-dd");
        endDate = today.addDays(7 - dayOfWeek).toString("yyyy-MM-dd");
    } else {
        startDate = today.toString("yyyy-MM-01");
        endDate = today.toString("yyyy-MM-dd");
    }

    return dao.calcDateRangeTotalIntake(userId, startDate, endDate);
}
