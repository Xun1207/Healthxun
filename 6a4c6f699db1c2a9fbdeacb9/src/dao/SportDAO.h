#ifndef SPORTDAO_H
#define SPORTDAO_H

#include "../entity/SportRecord.h"
#include <QList>

class SportDAO
{
public:
    bool addSportRecord(const SportRecord& record);
    bool updateSportRecord(const SportRecord& record);
    bool deleteSportRecord(int recordId);
    QList<SportRecord> queryByUserAndDate(int userId, const QString& startDate, const QString& endDate);
    int calcTotalCalorie(int userId, const QString& startDate, const QString& endDate);
};

#endif