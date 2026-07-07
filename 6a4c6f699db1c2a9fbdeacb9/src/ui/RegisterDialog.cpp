#include "RegisterDialog.h"
#include <QMessageBox>

RegisterDialog::RegisterDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("用户注册");
    setFixedSize(400, 350);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QLabel* titleLabel = new QLabel("用户注册", this);
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold; text-align: center;");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);
    mainLayout->addSpacing(20);

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

    QHBoxLayout* confirmLayout = new QHBoxLayout();
    QLabel* confirmLabel = new QLabel("确认密码:", this);
    confirmPwdEdit = new QLineEdit(this);
    confirmPwdEdit->setEchoMode(QLineEdit::Password);
    confirmLayout->addWidget(confirmLabel);
    confirmLayout->addWidget(confirmPwdEdit);
    mainLayout->addLayout(confirmLayout);

    mainLayout->addSpacing(20);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    submitBtn = new QPushButton("注册", this);
    cancelBtn = new QPushButton("取消", this);
    btnLayout->addWidget(submitBtn);
    btnLayout->addWidget(cancelBtn);
    mainLayout->addLayout(btnLayout);

    connect(submitBtn, &QPushButton::clicked, this, &RegisterDialog::submitReg);
    connect(cancelBtn, &QPushButton::clicked, this, &RegisterDialog::close);
}

RegisterDialog::~RegisterDialog()
{
}

void RegisterDialog::submitReg()
{
    QString username = usernameEdit->text().trimmed();
    QString password = passwordEdit->text();
    QString confirmPwd = confirmPwdEdit->text();

    int result = userService.userRegister(username, password, confirmPwd);

    if (result == -1) {
        QMessageBox::warning(this, "提示", "用户名和密码不能为空");
    } else if (result == -2) {
        QMessageBox::warning(this, "提示", "两次输入的密码不一致");
    } else if (result == -3) {
        QMessageBox::warning(this, "提示", "用户名已被占用");
    } else if (result == 1) {
        QMessageBox::information(this, "成功", "注册成功，请登录");
        close();
    } else {
        QMessageBox::warning(this, "错误", "注册失败");
    }
}