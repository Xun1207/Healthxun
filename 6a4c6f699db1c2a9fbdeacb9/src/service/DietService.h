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

    bool addDiet(int userId, const QString& foodName, float weight, int mealType);
    bool updateDiet(int dietId, const QString& foodName, float weight, int mealType);
    bool deleteDiet(int dietId);
    QList<DietRecord> queryDietByDate(int userId, const QString& startDate, const QString& endDate);
    int calcDayTotalIntake(int userId, const QString& date);

private:
    DietDAO dao;
};

#endif