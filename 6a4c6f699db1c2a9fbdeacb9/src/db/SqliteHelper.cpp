#include "SqliteHelper.h"

SqliteHelper* SqliteHelper::instance = nullptr;

SqliteHelper::SqliteHelper()
{
    dbPath = QDir::currentPath() + "/health.db";
}

SqliteHelper::~SqliteHelper()
{
    closeDB();
}

SqliteHelper* SqliteHelper::getInstance()
{
    if (instance == nullptr) {
        instance = new SqliteHelper();
    }
    return instance;
}

bool SqliteHelper::openDB()
{
    if (db.isOpen()) {
        return true;
    }

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);

    if (!db.open()) {
        qDebug() << "Database open error:" << db.lastError().text();
        return false;
    }

    initDatabase();
    return true;
}

void SqliteHelper::closeDB()
{
    if (db.isOpen()) {
        db.close();
    }
}

QSqlQuery SqliteHelper::execSql(const QString& sql)
{
    QSqlQuery query(db);
    if (!query.exec(sql)) {
        qDebug() << "SQL exec error:" << query.lastError().text() << ", SQL:" << sql;
    }
    return query;
}

bool SqliteHelper::execSqlNoQuery(const QString& sql)
{
    QSqlQuery query(db);
    return query.exec(sql);
}

QSqlDatabase SqliteHelper::getDatabase()
{
    return db;
}

void SqliteHelper::initDatabase()
{
    QStringList tables = db.tables();

    if (!tables.contains("user")) {
        QString sql = "CREATE TABLE user ("
                      "user_id INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "username TEXT UNIQUE NOT NULL,"
                      "pwd_md5 TEXT NOT NULL,"
                      "real_name TEXT,"
                      "gender INTEGER DEFAULT 0,"
                      "age INTEGER DEFAULT 0,"
                      "height REAL DEFAULT 0,"
                      "weight REAL DEFAULT 0,"
                      "medical_history TEXT,"
                      "allergies TEXT,"
                      "is_admin INTEGER DEFAULT 0,"
                      "is_frozen INTEGER DEFAULT 0,"
                      "create_time TEXT NOT NULL)";
        execSqlNoQuery(sql);

        QString adminPwd = "21232f297a57a5a743894a0e4a801fc3";
        QString insertAdmin = QString("INSERT INTO user(username, pwd_md5, real_name, is_admin, is_frozen, create_time) "
                                      "VALUES ('admin', '%1', '系统管理员', 1, 0, datetime('now'))").arg(adminPwd);
        execSqlNoQuery(insertAdmin);
    }

    if (!tables.contains("sport_record")) {
        QString sql = "CREATE TABLE sport_record ("
                      "record_id INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "user_id INTEGER NOT NULL,"
                      "sport_type TEXT NOT NULL,"
                      "duration REAL NOT NULL,"
                      "distance REAL DEFAULT 0,"
                      "calorie_burn INTEGER NOT NULL,"
                      "record_date TEXT NOT NULL,"
                      "FOREIGN KEY(user_id) REFERENCES user(user_id) ON DELETE CASCADE)";
        execSqlNoQuery(sql);
    }

    if (!tables.contains("diet_record")) {
        QString sql = "CREATE TABLE diet_record ("
                      "diet_id INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "user_id INTEGER NOT NULL,"
                      "food_name TEXT NOT NULL,"
                      "weight REAL NOT NULL,"
                      "calorie INTEGER NOT NULL,"
                      "meal_type INTEGER NOT NULL,"
                      "record_time TEXT NOT NULL,"
                      "food_id INTEGER,"
                      "FOREIGN KEY(user_id) REFERENCES user(user_id) ON DELETE CASCADE,"
                      "FOREIGN KEY(food_id) REFERENCES food_standard(food_id))";
        execSqlNoQuery(sql);
    }

    if (!tables.contains("food_standard")) {
        QString sql = "CREATE TABLE food_standard ("
                      "food_id INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "food_name TEXT UNIQUE NOT NULL,"
                      "per_100g_cal INTEGER NOT NULL)";
        execSqlNoQuery(sql);

        QStringList initFoods = {
            "INSERT INTO food_standard(food_name, per_100g_cal) VALUES ('米饭', 116)",
            "INSERT INTO food_standard(food_name, per_100g_cal) VALUES ('面条', 286)",
            "INSERT INTO food_standard(food_name, per_100g_cal) VALUES ('馒头', 221)",
            "INSERT INTO food_standard(food_name, per_100g_cal) VALUES ('鸡蛋', 143)",
            "INSERT INTO food_standard(food_name, per_100g_cal) VALUES ('牛奶', 54)",
            "INSERT INTO food_standard(food_name, per_100g_cal) VALUES ('苹果', 52)",
            "INSERT INTO food_standard(food_name, per_100g_cal) VALUES ('香蕉', 91)",
            "INSERT INTO food_standard(food_name, per_100g_cal) VALUES ('鸡胸肉', 167)",
            "INSERT INTO food_standard(food_name, per_100g_cal) VALUES ('牛肉', 125)",
            "INSERT INTO food_standard(food_name, per_100g_cal) VALUES ('鱼肉', 113)",
            "INSERT INTO food_standard(food_name, per_100g_cal) VALUES ('西红柿', 18)",
            "INSERT INTO food_standard(food_name, per_100g_cal) VALUES ('黄瓜', 16)",
            "INSERT INTO food_standard(food_name, per_100g_cal) VALUES ('西兰花', 34)",
            "INSERT INTO food_standard(food_name, per_100g_cal) VALUES ('土豆', 77)",
            "INSERT INTO food_standard(food_name, per_100g_cal) VALUES ('红薯', 86)"
        };
        for (const QString& insertSql : initFoods) {
            execSqlNoQuery(insertSql);
        }
    }

    if (!tables.contains("remind")) {
        QString sql = "CREATE TABLE remind ("
                      "remind_id INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "user_id INTEGER NOT NULL,"
                      "remind_type TEXT NOT NULL,"
                      "remind_time TEXT NOT NULL,"
                      "is_open INTEGER DEFAULT 1,"
                      "FOREIGN KEY(user_id) REFERENCES user(user_id) ON DELETE CASCADE)";
        execSqlNoQuery(sql);
    }

    if (!tables.contains("feedback")) {
        QString sql = "CREATE TABLE feedback ("
                      "feedback_id INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "user_id INTEGER NOT NULL,"
                      "content TEXT NOT NULL,"
                      "create_time TEXT NOT NULL,"
                      "FOREIGN KEY(user_id) REFERENCES user(user_id) ON DELETE CASCADE)";
        execSqlNoQuery(sql);
    }
}