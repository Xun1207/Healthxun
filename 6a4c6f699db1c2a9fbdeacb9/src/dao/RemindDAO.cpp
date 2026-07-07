#include "RemindDAO.h"
#include "../db/SqliteHelper.h"

bool RemindDAO::addRemind(const Remind& remind)
{
    SqliteHelper* dbHelper = SqliteHelper::getInstance();
    QString sql = QString("INSERT INTO remind(user_id, remind_type, remind_time, is_open) "
                          "VALUES (%1, '%2', '%3', %4)")
                      .arg(remind.userId)
                      .arg(remind.remindType)
                      .arg(remind.remindTime)
                      .arg(remind.isOpen ? 1 : 0);
    return dbHelper->execSqlNoQuery(sql);
}

bool RemindDAO::deleteRemindByUserType(int userId, const QString& type)
{
    SqliteHelper* dbHelper = SqliteHelper::getInstance();
    QString sql = QString("DELETE FROM remind WHERE user_id=%1 AND remind_type='%2'")
                      .arg(userId)
                      .arg(type);
    return dbHelper->execSqlNoQuery(sql);
}

QList<Remind> RemindDAO::queryUserAllRemind(int userId)
{
    QList<Remind> reminds;
    SqliteHelper* dbHelper = SqliteHelper::getInstance();
    QString sql = QString("SELECT * FROM remind WHERE user_id=%1").arg(userId);
    QSqlQuery query = dbHelper->execSql(sql);

    while (query.next()) {
        Remind remind;
        remind.remindId = query.value("remind_id").toInt();
        remind.userId = query.value("user_id").toInt();
        remind.remindType = query.value("remind_type").toString();
        remind.remindTime = query.value("remind_time").toString();
        remind.isOpen = query.value("is_open").toInt() == 1;
        reminds.append(remind);
    }

    return reminds;
}

bool RemindDAO::deleteRemindById(int remindId)
{
    SqliteHelper* dbHelper = SqliteHelper::getInstance();
    QString sql = QString("DELETE FROM remind WHERE remind_id=%1").arg(remindId);
    return dbHelper->execSqlNoQuery(sql);
}