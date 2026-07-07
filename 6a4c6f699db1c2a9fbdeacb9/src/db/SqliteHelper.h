#ifndef SQLITEHELPER_H
#define SQLITEHELPER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QFile>
#include <QDir>
#include <QDebug>

class SqliteHelper
{
public:
    static SqliteHelper* getInstance();
    ~SqliteHelper();

    bool openDB();
    void closeDB();
    QSqlQuery execSql(const QString& sql);
    bool execSqlNoQuery(const QString& sql);
    QSqlDatabase getDatabase();

private:
    SqliteHelper();
    SqliteHelper(const SqliteHelper&) = delete;
    SqliteHelper& operator=(const SqliteHelper&) = delete;

    static SqliteHelper* instance;
    QSqlDatabase db;
    QString dbPath;

    void initDatabase();
};

#endif