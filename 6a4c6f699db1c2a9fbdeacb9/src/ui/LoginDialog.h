#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "../service/UserService.h"
#include "../entity/User.h"

class LoginDialog : public QDialog
{
    Q_OBJECT
public:
    LoginDialog(QWidget* parent = nullptr);
    ~LoginDialog();

signals:
    void loginSuccess(const User& user);

private slots:
    void onLoginClick();
    void onRegisterClick();

private:
    QLineEdit* usernameEdit;
    QLineEdit* passwordEdit;
    QPushButton* loginBtn;
    QPushButton* registerBtn;
    UserService userService;
};

#endif