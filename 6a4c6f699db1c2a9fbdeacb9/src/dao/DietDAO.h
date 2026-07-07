#ifndef DIETDAO_H
#define DIETDAO_H

#include "../entity/DietRecord.h"
#include <QList>

class DietDAO
{
public:
    bool addDietRecord(const DietRecord& record);
    bool updateDietRecord(const DietRecord& record);
    bool deleteDietRecord(int dietId);
    QList<DietRecord> queryByUserAndDate(int userId, const QString& startDate, const QString& endDate);
    int calcDayTotalIntake(int userId, const QString& date);
    int calcDateRangeTotalIntake(int userId, const QString& startDate, const QString& endDate);
};

#endif