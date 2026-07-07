#include "ReportService.h"
#include <QDate>
#include <QSet>

ReportService::ReportService()
{
}

QMap<QString, QString> ReportService::generateMonthReportData(int userId, int year, int month)
{
    QMap<QString, QString> report;

    User user = userDao.queryUserById(userId);

    QString startDate = QString("%1-%2-%3").arg(year).arg(month, 2, 10, QChar('0')).arg("01");
    QDate lastDay = QDate(year, month, 1).addMonths(1).addDays(-1);
    QString endDate = QString("%1-%2-%3").arg(year).arg(month, 2, 10, QChar('0')).arg(lastDay.day());

    QList<SportRecord> sportRecords = sportDao.queryByUserAndDate(userId, startDate, endDate);
    QList<DietRecord> dietRecords = dietDao.queryByUserAndDate(userId, startDate, endDate);

    int totalSportCal = sportDao.calcTotalCalorie(userId, startDate, endDate);
    int totalDietCal = 0;
    for (const DietRecord& d : dietRecords) {
        totalDietCal += d.calorie;
    }

    float avgBMI = CalUtil::calcBMI(user.height, user.weight);

    int sportDays = 0;
    QSet<QString> sportDateSet;
    for (const SportRecord& s : sportRecords) {
        sportDateSet.insert(s.recordDate);
    }
    sportDays = sportDateSet.size();

    int totalDays = QDate(year, month, 1).daysInMonth();
    float sportRate = (float)sportDays / totalDays * 100;

    float avgDailyCal = totalDietCal / totalDays;
    int balanceScore = 50;
    if (avgDailyCal > 0) {
        int baseMetabolism = CalUtil::calcBaseMetabolism(user);
        if (avgDailyCal >= baseMetabolism * 0.9 && avgDailyCal <= baseMetabolism * 1.1) {
            balanceScore = 90;
        } else if (avgDailyCal >= baseMetabolism * 0.8 && avgDailyCal <= baseMetabolism * 1.2) {
            balanceScore = 70;
        }
    }

    report["avgBMI"] = QString::number(avgBMI, 'f', 2);
    report["totalSportCal"] = QString::number(totalSportCal);
    report["totalDietCal"] = QString::number(totalDietCal);
    report["sportDays"] = QString::number(sportDays);
    report["sportRate"] = QString::number(sportRate, 'f', 1) + "%";
    report["balanceScore"] = QString::number(balanceScore);
    report["totalDays"] = QString::number(totalDays);

    return report;
}

QString ReportService::buildHealthSuggest(int userId, int year, int month)
{
    QMap<QString, QString> report = generateMonthReportData(userId, year, month);
    QString suggest;

    float avgBMI = report["avgBMI"].toFloat();
    if (avgBMI < 18.5) {
        suggest += "您的BMI偏低，建议适当增加营养摄入，保持均衡饮食。\n";
    } else if (avgBMI >= 24 && avgBMI < 28) {
        suggest += "您的BMI偏胖，建议控制饮食，增加运动量。\n";
    } else if (avgBMI >= 28) {
        suggest += "您的BMI肥胖，建议严格控制饮食，加强运动锻炼。\n";
    } else {
        suggest += "您的BMI处于正常范围，继续保持！\n";
    }

    float sportRate = report["sportRate"].replace("%", "").toFloat();
    if (sportRate < 30) {
        suggest += "本月运动频率偏低，建议每周至少运动3次。\n";
    } else if (sportRate >= 50) {
        suggest += "本月运动频率良好，继续保持规律运动！\n";
    }

    int balanceScore = report["balanceScore"].toInt();
    if (balanceScore < 70) {
        suggest += "饮食均衡度有待提高，建议注意饮食搭配。\n";
    } else {
        suggest += "饮食均衡度良好，继续保持健康饮食习惯！\n";
    }

    return suggest;
}