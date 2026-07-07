#include "FoodStdDAO.h"
#include "../db/SqliteHelper.h"

bool FoodStdDAO::addFood(const FoodStandard& food)
{
    SqliteHelper* dbHelper = SqliteHelper::getInstance();
    QString sql = QString("INSERT INTO food_standard(food_name, per_100g_cal) VALUES ('%1', %2)")
                      .arg(food.foodName)
                      .arg(food.per100gCal);
    return dbHelper->execSqlNoQuery(sql);
}

bool FoodStdDAO::updateFoodCal(const FoodStandard& food)
{
    SqliteHelper* dbHelper = SqliteHelper::getInstance();
    QString sql = QString("UPDATE food_standard SET food_name='%1', per_100g_cal=%2 WHERE food_id=%3")
                      .arg(food.foodName)
                      .arg(food.per100gCal)
                      .arg(food.foodId);
    return dbHelper->execSqlNoQuery(sql);
}

bool FoodStdDAO::deleteFood(int foodId)
{
    SqliteHelper* dbHelper = SqliteHelper::getInstance();
    QString sql = QString("DELETE FROM food_standard WHERE food_id=%1").arg(foodId);
    return dbHelper->execSqlNoQuery(sql);
}

QList<FoodStandard> FoodStdDAO::queryAllFoodStd()
{
    QList<FoodStandard> foods;
    SqliteHelper* dbHelper = SqliteHelper::getInstance();
    QString sql = "SELECT * FROM food_standard ORDER BY food_name";
    QSqlQuery query = dbHelper->execSql(sql);

    while (query.next()) {
        FoodStandard food;
        food.foodId = query.value("food_id").toInt();
        food.foodName = query.value("food_name").toString();
        food.per100gCal = query.value("per_100g_cal").toInt();
        foods.append(food);
    }

    return foods;
}

FoodStandard FoodStdDAO::queryFoodByName(const QString& foodName)
{
    FoodStandard food;
    SqliteHelper* dbHelper = SqliteHelper::getInstance();
    QString sql = QString("SELECT * FROM food_standard WHERE food_name='%1'").arg(foodName);
    QSqlQuery query = dbHelper->execSql(sql);

    if (query.next()) {
        food.foodId = query.value("food_id").toInt();
        food.foodName = query.value("food_name").toString();
        food.per100gCal = query.value("per_100g_cal").toInt();
    }

    return food;
}