#ifndef DIETSERVICE_H
#define DIETSERVICE_H

#include "../entity/DietRecord.h"
#include "../dao/DietDAO.h"
#include "../util/CalUtil.h"
#include <QDateTime>
#include <QList>

class DietService
{
public:
    DietService();

    bool addDiet(int userId, const QString& foodName, float weight, int mealType, const QString& recordTime = QString());
    bool updateDiet(int dietId, const QString& foodName, float weight, int mealType, const QString& recordTime = QString());
    bool deleteDiet(int dietId);
    QList<DietRecord> queryDietByDate(int userId, const QString& startDate, const QString& endDate);
    QList<DietRecord> queryDietByCycle(int userId, int cycleType);
    int calcDayTotalIntake(int userId, const QString& date);
    int calcDateRangeTotalIntake(int userId, const QString& startDate, const QString& endDate);
    int calcCycleTotalIntake(int userId, int cycleType);

private:
    DietDAO dao;
};

#endif