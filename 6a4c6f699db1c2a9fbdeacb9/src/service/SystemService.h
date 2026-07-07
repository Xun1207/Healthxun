#ifndef SYSTEMSERVICE_H
#define SYSTEMSERVICE_H

#include "../entity/FoodStandard.h"
#include "../dao/FoodStdDAO.h"
#include "../util/FileTool.h"
#include <QList>
#include <QMap>

class SystemService
{
public:
    SystemService();

    bool updateFoodStd(const FoodStandard& food);
    bool addFoodStd(const FoodStandard& food);
    bool deleteFoodStd(int foodId);
    QList<FoodStandard> getAllFoodStd();
    bool backupAllDatabase(const QString& savePath);
    bool restoreDBFromFile(const QString& filePath);
    QList<QMap<QString, QString>> getAllFeedback();
    bool addFeedback(int userId, const QString& content);

private:
    FoodStdDAO dao;
};

#endif