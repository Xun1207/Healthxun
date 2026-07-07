#include <QApplication>
#include <QFile>
#include <QDebug>
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
    } else {
        qDebug() << "Warning: Could not load style sheet:" << styleFile.errorString();
    }

    if (!SqliteHelper::getInstance()->openDB()) {
        qCritical() << "Failed to open database!";
        return -1;
    }

    LoginDialog loginDialog;
    MainWindow* mainWindow = nullptr;

    // 修复信号参数匹配 - 使用const引用避免拷贝和类型不匹配
    QObject::connect(&loginDialog, &LoginDialog::loginSuccess, [&](const User& user) {
        mainWindow = new MainWindow(user);
        // 先显示主窗口，再关闭登录对话框，避免程序因为没有可见窗口而退出
        mainWindow->show();
        loginDialog.close();
    });

    loginDialog.show();

    int result = a.exec();

    if (mainWindow) {
        delete mainWindow;
        mainWindow = nullptr;
    }

    SqliteHelper::getInstance()->closeDB();

    return result;
}
