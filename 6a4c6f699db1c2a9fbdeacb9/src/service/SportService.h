#ifndef SPORTSERVICE_H
#define SPORTSERVICE_H

#include "../entity/SportRecord.h"
#include "../dao/SportDAO.h"
#include "../util/CalUtil.h"
#include <QDateTime>
#include <QList>

class SportService
{
public:
    SportService();

    bool addSport(int userId, const QString& sportType, float duration, float distance, const QString& recordDate = QString());
    bool updateSport(int recordId, const QString& sportType, float duration, float distance, const QString& recordDate = QString());
    bool deleteSport(int recordId);
    QList<SportRecord> querySportByCycle(int userId, int cycleType);
    QList<SportRecord> querySportByDateRange(int userId, const QString& startDate, const QString& endDate);
    int calcCycleSportTotal(int userId, int cycleType);
    int calcDateRangeSportTotal(int userId, const QString& startDate, const QString& endDate);

private:
    SportDAO dao;
};

#endif