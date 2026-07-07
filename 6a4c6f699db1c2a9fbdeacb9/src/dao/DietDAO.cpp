#include "DietDAO.h"
#include "../db/SqliteHelper.h"

bool DietDAO::addDietRecord(const DietRecord& record)
{
    SqliteHelper* dbHelper = SqliteHelper::getInstance();
    QString sql = QString("INSERT INTO diet_record(user_id, food_name, weight, calorie, meal_type, "
                          "record_time, food_id) VALUES (%1, '%2', %3, %4, %5, '%6', %7)")
                      .arg(record.userId)
                      .arg(record.foodName)
                      .arg(record.weight)
                      .arg(record.calorie)
                      .arg(record.mealType)
                      .arg(record.recordTime)
                      .arg(record.foodId);
    return dbHelper->execSqlNoQuery(sql);
}

bool DietDAO::updateDietRecord(const DietRecord& record)
{
    SqliteHelper* dbHelper = SqliteHelper::getInstance();
    QString sql = QString("UPDATE diet_record SET food_name='%1', weight=%2, calorie=%3, "
                          "meal_type=%4, record_time='%5', food_id=%6 WHERE diet_id=%7")
                      .arg(record.foodName)
                      .arg(record.weight)
                      .arg(record.calorie)
                      .arg(record.mealType)
                      .arg(record.recordTime)
                      .arg(record.foodId)
                      .arg(record.dietId);
    return dbHelper->execSqlNoQuery(sql);
}

bool DietDAO::deleteDietRecord(int dietId)
{
    SqliteHelper* dbHelper = SqliteHelper::getInstance();
    QString sql = QString("DELETE FROM diet_record WHERE diet_id=%1").arg(dietId);
    return dbHelper->execSqlNoQuery(sql);
}

QList<DietRecord> DietDAO::queryByUserAndDate(int userId, const QString& startDate, const QString& endDate)
{
    QList<DietRecord> records;
    SqliteHelper* dbHelper = SqliteHelper::getInstance();
    QString sql = QString("SELECT * FROM diet_record WHERE user_id=%1 AND record_time >= '%2' AND record_time <= '%3' ORDER BY record_time DESC")
                      .arg(userId)
                      .arg(startDate + " 00:00:00")
                      .arg(endDate + " 23:59:59");
    QSqlQuery query = dbHelper->execSql(sql);

    while (query.next()) {
        DietRecord record;
        record.dietId = query.value("diet_id").toInt();
        record.userId = query.value("user_id").toInt();
        record.foodName = query.value("food_name").toString();
        record.weight = query.value("weight").toFloat();
        record.calorie = query.value("calorie").toInt();
        record.mealType = query.value("meal_type").toInt();
        record.recordTime = query.value("record_time").toString();
        record.foodId = query.value("food_id").toInt();
        records.append(record);
    }

    return records;
}

int DietDAO::calcDayTotalIntake(int userId, const QString& date)
{
    SqliteHelper* dbHelper = SqliteHelper::getInstance();
    QString sql = QString("SELECT SUM(calorie) FROM diet_record WHERE user_id=%1 AND record_time >= '%2 00:00:00' AND record_time <= '%2 23:59:59'")
                      .arg(userId)
                      .arg(date);
    QSqlQuery query = dbHelper->execSql(sql);

    if (query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}