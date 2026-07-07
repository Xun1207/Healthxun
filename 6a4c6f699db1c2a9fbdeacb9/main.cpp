#include <QApplication>
#include "src/db/SqliteHelper.h"
#include "src/ui/LoginDialog.h"
#include "src/ui/MainWindow.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    SqliteHelper::getInstance()->openDB();

    LoginDialog loginDialog;
    MainWindow* mainWindow = nullptr;

    QObject::connect(&loginDialog, &LoginDialog::loginSuccess, [&](const User& user) {
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