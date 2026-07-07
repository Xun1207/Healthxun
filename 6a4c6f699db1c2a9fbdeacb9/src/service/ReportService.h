#ifndef REPORTSERVICE_H
#define REPORTSERVICE_H

#include "../entity/User.h"
#include "../entity/SportRecord.h"
#include "../entity/DietRecord.h"
#include "../dao/UserDAO.h"
#include "../dao/SportDAO.h"
#include "../dao/DietDAO.h"
#include "../util/CalUtil.h"
#include <QMap>
#include <QString>

class ReportService
{
public:
    ReportService();

    QMap<QString, QString> generateMonthReportData(int userId, int year, int month);
    QString buildHealthSuggest(int userId, int year, int month);

private:
    UserDAO userDao;
    SportDAO sportDao;
    DietDAO dietDao;
};

#endif