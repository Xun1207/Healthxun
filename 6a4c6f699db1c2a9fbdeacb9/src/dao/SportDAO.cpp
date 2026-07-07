#include "SportDAO.h"
#include "../db/SqliteHelper.h"

bool SportDAO::addSportRecord(const SportRecord& record)
{
    SqliteHelper* dbHelper = SqliteHelper::getInstance();
    QString sql = QString("INSERT INTO sport_record(user_id, sport_type, duration, distance, "
                          "calorie_burn, record_date) VALUES (%1, '%2', %3, %4, %5, '%6')")
                      .arg(record.userId)
                      .arg(record.sportType)
                      .arg(record.duration)
                      .arg(record.distance)
                      .arg(record.calorieBurn)
                      .arg(record.recordDate);
    return dbHelper->execSqlNoQuery(sql);
}

bool SportDAO::updateSportRecord(const SportRecord& record)
{
    SqliteHelper* dbHelper = SqliteHelper::getInstance();
    QString sql = QString("UPDATE sport_record SET sport_type='%1', duration=%2, distance=%3, "
                          "calorie_burn=%4, record_date='%5' WHERE record_id=%6")
                      .arg(record.sportType)
                      .arg(record.duration)
                      .arg(record.distance)
                      .arg(record.calorieBurn)
                      .arg(record.recordDate)
                      .arg(record.recordId);
    return dbHelper->execSqlNoQuery(sql);
}

bool SportDAO::deleteSportRecord(int recordId)
{
    SqliteHelper* dbHelper = SqliteHelper::getInstance();
    QString sql = QString("DELETE FROM sport_record WHERE record_id=%1").arg(recordId);
    return dbHelper->execSqlNoQuery(sql);
}

QList<SportRecord> SportDAO::queryByUserAndDate(int userId, const QString& startDate, const QString& endDate)
{
    QList<SportRecord> records;
    SqliteHelper* dbHelper = SqliteHelper::getInstance();
    QString sql = QString("SELECT * FROM sport_record WHERE user_id=%1 AND record_date >= '%2' AND record_date <= '%3' ORDER BY record_date DESC")
                      .arg(userId)
                      .arg(startDate)
                      .arg(endDate);
    QSqlQuery query = dbHelper->execSql(sql);

    while (query.next()) {
        SportRecord record;
        record.recordId = query.value("record_id").toInt();
        record.userId = query.value("user_id").toInt();
        record.sportType = query.value("sport_type").toString();
        record.duration = query.value("duration").toFloat();
        record.distance = query.value("distance").toFloat();
        record.calorieBurn = query.value("calorie_burn").toInt();
        record.recordDate = query.value("record_date").toString();
        records.append(record);
    }

    return records;
}

int SportDAO::calcTotalCalorie(int userId, const QString& startDate, const QString& endDate)
{
    SqliteHelper* dbHelper = SqliteHelper::getInstance();
    QString sql = QString("SELECT SUM(calorie_burn) FROM sport_record WHERE user_id=%1 AND record_date >= '%2' AND record_date <= '%3'")
                      .arg(userId)
                      .arg(startDate)
                      .arg(endDate);
    QSqlQuery query = dbHelper->execSql(sql);

    if (query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}