#ifndef USERSERVICE_H
#define USERSERVICE_H

#include "../entity/User.h"
#include "../dao/UserDAO.h"
#include "../util/EncryptTool.h"
#include <QDateTime>
#include <QList>

class UserService
{
public:
    UserService();

    int userRegister(const QString& username, const QString& password, const QString& confirmPwd);
    User userLogin(const QString& username, const QString& password);
    bool updateUserInfo(const User& user);
    bool freezeUser(int userId, bool freeze);
    bool resetUserPassword(int userId);
    QList<User> queryAllUser();
    User queryUserById(int userId);

private:
    UserDAO dao;
};

#endif