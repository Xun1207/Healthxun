#ifndef USER_H
#define USER_H

#include <QString>

class User
{
public:
    User();

    int userId;
    QString username;
    QString pwdMd5;
    QString realName;
    int gender;
    int age;
    float height;
    float weight;
    QString medicalHistory;
    QString allergies;
    bool isAdmin;
    bool isFrozen;
    QString createTime;

    float calculateBMI() const;
};

#endif