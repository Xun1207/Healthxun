#ifndef FILETOOL_H
#define FILETOOL_H

#include <QString>

class FileTool
{
public:
    static bool exportToTxt(const QString& path, const QString& content);
    static bool backupAllTableData(const QString& savePath);
    static bool restoreDB(const QString& filePath);
};

#endif