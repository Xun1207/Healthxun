#include <QApplication>
#include <QFile>
#include "src/db/SqliteHelper.h"
#include "src/ui/LoginDialog.h"
#include "src/ui/MainWindow.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    // 加载全局样式表
    QFile styleFile(":/src/style.qss");
    if (styleFile.open(QFile::ReadOnly | QFile::Text)) {
        QString styleSheet = QLatin1String(styleFile.readAll());
        a.setStyleSheet(styleSheet);
        styleFile.close();
    }

    SqliteHelper::getInstance()->openDB();

    LoginDialog loginDialog;
    MainWindow* mainWindow = nullptr;

    QObject::connect(&loginDialog, &LoginDialog::loginSuccess, [&](const User& user) {
        loginDialog.hide();
        mainWindow = new MainWindow(user);
        mainWindow->show();
    });

    loginDialog.show();

    int result = a.exec();

    if (mainWindow) {
        delete mainWindow;
    }

    SqliteHelper::getInstance()->closeDB();

    return result;
}