#include "LoginDialog.h"
#include "RegisterDialog.h"
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("健康管理系统 - 登录");
    setFixedSize(400, 300);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QLabel* titleLabel = new QLabel("健康管理系统", this);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; text-align: center;");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);
    mainLayout->addSpacing(30);

    QHBoxLayout* userLayout = new QHBoxLayout();
    QLabel* userLabel = new QLabel("用户名:", this);
    usernameEdit = new QLineEdit(this);
    userLayout->addWidget(userLabel);
    userLayout->addWidget(usernameEdit);
    mainLayout->addLayout(userLayout);

    QHBoxLayout* pwdLayout = new QHBoxLayout();
    QLabel* pwdLabel = new QLabel("密  码:", this);
    passwordEdit = new QLineEdit(this);
    passwordEdit->setEchoMode(QLineEdit::Password);
    pwdLayout->addWidget(pwdLabel);
    pwdLayout->addWidget(passwordEdit);
    mainLayout->addLayout(pwdLayout);

    mainLayout->addSpacing(20);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    loginBtn = new QPushButton("登录", this);
    registerBtn = new QPushButton("注册", this);
    btnLayout->addWidget(loginBtn);
    btnLayout->addWidget(registerBtn);
    mainLayout->addLayout(btnLayout);

    connect(loginBtn, &QPushButton::clicked, this, &LoginDialog::onLoginClick);
    connect(registerBtn, &QPushButton::clicked, this, &LoginDialog::onRegisterClick);
}

LoginDialog::~LoginDialog()
{
}

void LoginDialog::onLoginClick()
{
    QString username = usernameEdit->text().trimmed();
    QString password = passwordEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "提示", "用户名和密码不能为空");
        return;
    }

    User user = userService.userLogin(username, password);

    if (user.userId == 0) {
        QMessageBox::warning(this, "错误", "用户名或密码错误");
    } else if (user.userId == -1) {
        QMessageBox::warning(this, "错误", "账号已被冻结");
    } else {
        QMessageBox::information(this, "成功", "登录成功");
        emit loginSuccess(user);
        close();
    }
}

void LoginDialog::onRegisterClick()
{
    RegisterDialog* regDialog = new RegisterDialog(this);
    regDialog->show();
}