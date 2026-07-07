#include "User.h"

User::User()
    : userId(0), gender(0), age(0), height(0), weight(0), isAdmin(false), isFrozen(false)
{
}

float User::calculateBMI() const
{
    if (height <= 0 || weight <= 0) {
        return 0;
    }
    float heightInMeters = height / 100.0f;
    return weight / (heightInMeters * heightInMeters);
}