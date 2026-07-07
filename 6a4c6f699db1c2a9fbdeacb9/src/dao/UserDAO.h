#ifndef USERDAO_H
#define USERDAO_H

#include "../entity/User.h"
#include <QList>

class UserDAO
{
public:
    bool addUser(const User& user);
    User queryUserByName(const QString& username);
    User queryUserById(int userId);
    bool updateUserInfo(const User& user);
    bool freezeUserById(int userId, bool freeze);
    bool resetPwdById(int userId, const QString& newPwdMd5);
    QList<User> queryAllUser();
};

#endif