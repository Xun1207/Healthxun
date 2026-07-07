#include "SportService.h"

SportService::SportService()
{
}

bool SportService::addSport(int userId, const QString& sportType, float duration, float distance)
{
    int calorie = CalUtil::calcSportBurn(sportType, duration);

    SportRecord record;
    record.userId = userId;
    record.sportType = sportType;
    record.duration = duration;
    record.distance = distance;
    record.calorieBurn = calorie;
    record.recordDate = QDateTime::currentDateTime().toString("yyyy-MM-dd");

    return dao.addSportRecord(record);
}

bool SportService::updateSport(int recordId, const QString& sportType, float duration, float distance)
{
    int calorie = CalUtil::calcSportBurn(sportType, duration);

    SportRecord record;
    record.recordId = recordId;
    record.sportType = sportType;
    record.duration = duration;
    record.distance = distance;
    record.calorieBurn = calorie;

    return dao.updateSportRecord(record);
}

bool SportService::deleteSport(int recordId)
{
    return dao.deleteSportRecord(recordId);
}

QList<SportRecord> SportService::querySportByCycle(int userId, int cycleType)
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

int SportService::calcCycleSportTotal(int userId, int cycleType)
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

    return dao.calcTotalCalorie(userId, startDate, endDate);
}