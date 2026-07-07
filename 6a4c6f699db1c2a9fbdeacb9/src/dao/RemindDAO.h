#ifndef REMINDAO_H
#define REMINDAO_H

#include "../entity/Remind.h"
#include <QList>

class RemindDAO
{
public:
    bool addRemind(const Remind& remind);
    bool deleteRemindByUserType(int userId, const QString& type);
    QList<Remind> queryUserAllRemind(int userId);
    bool deleteRemindById(int remindId);
};

#endif