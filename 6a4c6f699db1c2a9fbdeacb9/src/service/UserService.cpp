#include "UserService.h"

UserService::UserService()
{
}

int UserService::userRegister(const QString& username, const QString& password, const QString& confirmPwd)
{
    if (username.isEmpty() || password.isEmpty() || confirmPwd.isEmpty()) {
        return -1;
    }

    if (password != confirmPwd) {
        return -2;
    }

    User existing = dao.queryUserByName(username);
    if (existing.userId != 0) {
        return -3;
    }

    User user;
    user.username = username;
    user.pwdMd5 = EncryptTool::md5Encrypt(password);
    user.createTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

    if (dao.addUser(user)) {
        return 1;
    }
    return 0;
}

User UserService::userLogin(const QString& username, const QString& password)
{
    if (username.isEmpty() || password.isEmpty()) {
        return User();
    }

    User user = dao.queryUserByName(username);
    if (user.userId == 0) {
        return User();
    }

    if (user.isFrozen) {
        User frozenUser;
        frozenUser.userId = -1;
        return frozenUser;
    }

    QString encryptedPwd = EncryptTool::md5Encrypt(password);
    if (user.pwdMd5 == encryptedPwd) {
        return user;
    }

    return User();
}

bool UserService::updateUserInfo(const User& user)
{
    return dao.updateUserInfo(user);
}

bool UserService::freezeUser(int userId, bool freeze)
{
    return dao.freezeUserById(userId, freeze);
}

bool UserService::resetUserPassword(int userId)
{
    QString defaultPwd = "123456";
    QString md5Pwd = EncryptTool::md5Encrypt(defaultPwd);
    return dao.resetPwdById(userId, md5Pwd);
}

QList<User> UserService::queryAllUser()
{
    return dao.queryAllUser();
}

User UserService::queryUserById(int userId)
{
    return dao.queryUserById(userId);
}