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

    bool addSport(int userId, const QString& sportType, float duration, float distance);
    bool updateSport(int recordId, const QString& sportType, float duration, float distance);
    bool deleteSport(int recordId);
    QList<SportRecord> querySportByCycle(int userId, int cycleType);
    int calcCycleSportTotal(int userId, int cycleType);

private:
    SportDAO dao;
};

#endif