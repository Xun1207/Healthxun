#include "FileTool.h"
#include "../db/SqliteHelper.h"
#include <QFile>
#include <QTextStream>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDir>

bool FileTool::exportToTxt(const QString& path, const QString& content)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream out(&file);
    out << content;
    file.close();
    return true;
}

bool FileTool::backupAllTableData(const QString& savePath)
{
    QString dirPath = QFileInfo(savePath).dir().path();
    QDir dir(dirPath);
    if (!dir.exists()) {
        dir.mkpath(dirPath);
    }

    QFile file(savePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream out(&file);
    SqliteHelper* dbHelper = SqliteHelper::getInstance();

    QStringList tables = {"user", "sport_record", "diet_record", "food_standard", "remind", "feedback"};

    for (const QString& table : tables) {
        out << "===== TABLE: " << table << " =====\n";
        QString sql = QString("SELECT * FROM %1").arg(table);
        QSqlQuery query = dbHelper->execSql(sql);

        QSqlRecord record = query.record();
        int colCount = record.count();
        QStringList headers;
        for (int i = 0; i < colCount; i++) {
            headers << record.fieldName(i);
        }
        out << headers.join("\t") << "\n";

        while (query.next()) {
            QStringList rowData;
            for (int i = 0; i < colCount; i++) {
                rowData << query.value(i).toString();
            }
            out << rowData.join("\t") << "\n";
        }
        out << "\n";
    }

    file.close();
    return true;
}

bool FileTool::restoreDB(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    SqliteHelper* dbHelper = SqliteHelper::getInstance();
    QTextStream in(&file);

    QString currentTable;
    QStringList headers;

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();

        if (line.startsWith("===== TABLE:")) {
            currentTable = line.mid(14, line.length() - 14 - 6).trimmed();
            headers.clear();
            continue;
        }

        if (headers.isEmpty() && !currentTable.isEmpty()) {
            headers = line.split("\t");
            QString clearSql = QString("DELETE FROM %1").arg(currentTable);
            dbHelper->execSqlNoQuery(clearSql);
            continue;
        }

        if (!currentTable.isEmpty() && !headers.isEmpty()) {
            QStringList values = line.split("\t");
            if (values.size() == headers.size()) {
                QStringList placeholders;
                QStringList insertValues;
                for (int i = 0; i < values.size(); i++) {
                    if (headers[i] == "user_id" || headers[i] == "record_id" ||
                        headers[i] == "diet_id" || headers[i] == "food_id" ||
                        headers[i] == "remind_id" || headers[i] == "feedback_id") {
                        continue;
                    }
                    placeholders << QString(":%1").arg(headers[i]);
                    insertValues << headers[i];
                }

                QString sql = QString("INSERT INTO %1 (%2) VALUES (%3)")
                                  .arg(currentTable)
                                  .arg(insertValues.join(", "))
                                  .arg(placeholders.join(", "));

                QSqlQuery query(dbHelper->getDatabase());
                query.prepare(sql);
                for (int i = 0; i < headers.size(); i++) {
                    if (headers[i] == "user_id" || headers[i] == "record_id" ||
                        headers[i] == "diet_id" || headers[i] == "food_id" ||
                        headers[i] == "remind_id" || headers[i] == "feedback_id") {
                        continue;
                    }
                    query.bindValue(QString(":%1").arg(headers[i]), values[i]);
                }
                query.exec();
            }
        }
    }

    file.close();
    return true;
}