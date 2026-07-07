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

    // 按值捕获user，避免引用悬空
    QObject::connect(&loginDialog, &LoginDialog::loginSuccess, [&](const User& user) {
        mainWindow = new MainWindow(user);
        mainWindow->show();
        loginDialog.hide();
    });

    if (loginDialog.exec() == QDialog::Accepted) {
        // 登录成功后运行主循环
        a.exec();
        if (mainWindow) {
            delete mainWindow;
            mainWindow = nullptr;
        }
    }

    SqliteHelper::getInstance()->closeDB();

    return 0;
}