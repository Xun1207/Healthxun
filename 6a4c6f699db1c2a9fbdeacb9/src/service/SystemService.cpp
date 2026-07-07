#include "SystemService.h"
#include "../db/SqliteHelper.h"
#include <QSqlQuery>

SystemService::SystemService()
{
}

bool SystemService::updateFoodStd(const FoodStandard& food)
{
    return dao.updateFoodCal(food);
}

bool SystemService::addFoodStd(const FoodStandard& food)
{
    return dao.addFood(food);
}

bool SystemService::deleteFoodStd(int foodId)
{
    return dao.deleteFood(foodId);
}

QList<FoodStandard> SystemService::getAllFoodStd()
{
    return dao.queryAllFoodStd();
}

bool SystemService::backupAllDatabase(const QString& savePath)
{
    return FileTool::backupAllTableData(savePath);
}

bool SystemService::restoreDBFromFile(const QString& filePath)
{
    return FileTool::restoreDB(filePath);
}

QList<QMap<QString, QString>> SystemService::getAllFeedback()
{
    QList<QMap<QString, QString>> feedbackList;
    SqliteHelper* dbHelper = SqliteHelper::getInstance();
    QString sql = "SELECT f.feedback_id, f.user_id, f.content, f.create_time, u.username "
                  "FROM feedback f JOIN user u ON f.user_id = u.user_id ORDER BY f.create_time DESC";
    QSqlQuery query = dbHelper->execSql(sql);

    while (query.next()) {
        QMap<QString, QString> feedback;
        feedback["feedbackId"] = query.value("feedback_id").toString();
        feedback["userId"] = query.value("user_id").toString();
        feedback["username"] = query.value("username").toString();
        feedback["content"] = query.value("content").toString();
        feedback["createTime"] = query.value("create_time").toString();
        feedbackList.append(feedback);
    }

    return feedbackList;
}