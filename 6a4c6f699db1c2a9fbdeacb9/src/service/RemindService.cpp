#include "RemindService.h"

RemindService::RemindService()
{
}

bool RemindService::saveUserRemind(int userId, const QString& remindType, const QString& remindTime, bool isOpen)
{
    dao.deleteRemindByUserType(userId, remindType);

    Remind remind;
    remind.userId = userId;
    remind.remindType = remindType;
    remind.remindTime = remindTime;
    remind.isOpen = isOpen;

    return dao.addRemind(remind);
}

bool RemindService::deleteOldRemind(int userId, const QString& type)
{
    return dao.deleteRemindByUserType(userId, type);
}

void RemindService::reloadAllTimer(int userId)
{
    QList<Remind> reminds = dao.queryUserAllRemind(userId);
    NotifyTool::getInstance()->loadReminds(reminds);
    NotifyTool::getInstance()->startTimerTask();
}