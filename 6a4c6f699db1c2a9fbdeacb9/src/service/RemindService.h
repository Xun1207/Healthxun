#ifndef REMINDSERVICE_H
#define REMINDSERVICE_H

#include "../entity/Remind.h"
#include "../dao/RemindDAO.h"
#include "../util/NotifyTool.h"
#include <QList>

class RemindService
{
public:
    RemindService();

    bool saveUserRemind(int userId, const QString& remindType, const QString& remindTime, bool isOpen);
    bool deleteOldRemind(int userId, const QString& type);
    void reloadAllTimer(int userId);

private:
    RemindDAO dao;
};

#endif