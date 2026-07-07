#include "UserDAO.h"
#include "../db/SqliteHelper.h"

bool UserDAO::addUser(const User& user)
{
    SqliteHelper* dbHelper = SqliteHelper::getInstance();
    QString sql = QString("INSERT INTO user(username, pwd_md5, real_name, gender, age, height, weight, "
                          "medical_history, allergies, is_admin, is_frozen, create_time) "
                          "VALUES ('%1', '%2', '%3', %4, %5, %6, %7, '%8', '%9', %10, %11, '%12')")
                      .arg(user.username)
                      .arg(user.pwdMd5)
                      .arg(user.realName)
                      .arg(user.gender)
                      .arg(user.age)
                      .arg(user.height)
                      .arg(user.weight)
                      .arg(user.medicalHistory)
                      .arg(user.allergies)
                      .arg(user.isAdmin ? 1 : 0)
                      .arg(user.isFrozen ? 1 : 0)
                      .arg(user.createTime);
    return dbHelper->execSqlNoQuery(sql);
}

User UserDAO::queryUserByName(const QString& username)
{
    User user;
    SqliteHelper* dbHelper = SqliteHelper::getInstance();
    QString sql = QString("SELECT * FROM user WHERE username = '%1'").arg(username);
    QSqlQuery query = dbHelper->execSql(sql);

    if (query.next()) {
        user.userId = query.value("user_id").toInt();
        user.username = query.value("username").toString();
        user.pwdMd5 = query.value("pwd_md5").toString();
        user.realName = query.value("real_name").toString();
        user.gender = query.value("gender").toInt();
        user.age = query.value("age").toInt();
        user.height = query.value("height").toFloat();
        user.weight = query.value("weight").toFloat();
        user.medicalHistory = query.value("medical_history").toString();
        user.allergies = query.value("allergies").toString();
        user.isAdmin = query.value("is_admin").toInt() == 1;
        user.isFrozen = query.value("is_frozen").toInt() == 1;
        user.createTime = query.value("create_time").toString();
    }

    return user;
}

User UserDAO::queryUserById(int userId)
{
    User user;
    SqliteHelper* dbHelper = SqliteHelper::getInstance();
    QString sql = QString("SELECT * FROM user WHERE user_id = %1").arg(userId);
    QSqlQuery query = dbHelper->execSql(sql);

    if (query.next()) {
        user.userId = query.value("user_id").toInt();
        user.username = query.value("username").toString();
        user.pwdMd5 = query.value("pwd_md5").toString();
        user.realName = query.value("real_name").toString();
        user.gender = query.value("gender").toInt();
        user.age = query.value("age").toInt();
        user.height = query.value("height").toFloat();
        user.weight = query.value("weight").toFloat();
        user.medicalHistory = query.value("medical_history").toString();
        user.allergies = query.value("allergies").toString();
        user.isAdmin = query.value("is_admin").toInt() == 1;
        user.isFrozen = query.value("is_frozen").toInt() == 1;
        user.createTime = query.value("create_time").toString();
    }

    return user;
}

bool UserDAO::updateUserInfo(const User& user)
{
    SqliteHelper* dbHelper = SqliteHelper::getInstance();
    QString sql = QString("UPDATE user SET real_name='%1', gender=%2, age=%3, height=%4, weight=%5, "
                          "medical_history='%6', allergies='%7' WHERE user_id=%8")
                      .arg(user.realName)
                      .arg(user.gender)
                      .arg(user.age)
                      .arg(user.height)
                      .arg(user.weight)
                      .arg(user.medicalHistory)
                      .arg(user.allergies)
                      .arg(user.userId);
    return dbHelper->execSqlNoQuery(sql);
}

bool UserDAO::freezeUserById(int userId, bool freeze)
{
    SqliteHelper* dbHelper = SqliteHelper::getInstance();
    QString sql = QString("UPDATE user SET is_frozen=%1 WHERE user_id=%2")
                      .arg(freeze ? 1 : 0)
                      .arg(userId);
    return dbHelper->execSqlNoQuery(sql);
}

bool UserDAO::resetPwdById(int userId, const QString& newPwdMd5)
{
    SqliteHelper* dbHelper = SqliteHelper::getInstance();
    QString sql = QString("UPDATE user SET pwd_md5='%1' WHERE user_id=%2")
                      .arg(newPwdMd5)
                      .arg(userId);
    return dbHelper->execSqlNoQuery(sql);
}

QList<User> UserDAO::queryAllUser()
{
    QList<User> users;
    SqliteHelper* dbHelper = SqliteHelper::getInstance();
    QString sql = "SELECT * FROM user ORDER BY user_id";
    QSqlQuery query = dbHelper->execSql(sql);

    while (query.next()) {
        User user;
        user.userId = query.value("user_id").toInt();
        user.username = query.value("username").toString();
        user.pwdMd5 = query.value("pwd_md5").toString();
        user.realName = query.value("real_name").toString();
        user.gender = query.value("gender").toInt();
        user.age = query.value("age").toInt();
        user.height = query.value("height").toFloat();
        user.weight = query.value("weight").toFloat();
        user.medicalHistory = query.value("medical_history").toString();
        user.allergies = query.value("allergies").toString();
        user.isAdmin = query.value("is_admin").toInt() == 1;
        user.isFrozen = query.value("is_frozen").toInt() == 1;
        user.createTime = query.value("create_time").toString();
        users.append(user);
    }

    return users;
}