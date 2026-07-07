#ifndef ENCRYPTTOOL_H
#define ENCRYPTTOOL_H

#include <QString>

class EncryptTool
{
public:
    static QString md5Encrypt(const QString& rawPwd);
};

#endif