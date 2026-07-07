#include "EncryptTool.h"
#include <QCryptographicHash>

QString EncryptTool::md5Encrypt(const QString& rawPwd)
{
    QByteArray ba = rawPwd.toUtf8();
    ba = QCryptographicHash::hash(ba, QCryptographicHash::Md5);
    return ba.toHex();
}