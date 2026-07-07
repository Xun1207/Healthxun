#include "LoginDialog.h"
#include "RegisterDialog.h"
#include <QMessageBox>
#include <QGraphicsDropShadowEffect>

LoginDialog::LoginDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("健康管理系统 - 登录");
    setFixedSize(450, 500);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    // 设置对话框背景白色和圆角
    setStyleSheet("QDialog { background-color: white; border-radius: 12px; }");

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(50, 50, 50, 40);
    mainLayout->setSpacing(20);

    // Logo/标题区域
    QVBoxLayout* titleLayout = new QVBoxLayout();
    titleLayout->setSpacing(8);

    QLabel* iconLabel = new QLabel("🏥", this);
    iconLabel->setStyleSheet("font-size: 48px;");
    iconLabel->setAlignment(Qt::AlignCenter);
    titleLayout->addWidget(iconLabel);

    QLabel* titleLabel = new QLabel("健康管理系统", this);
    titleLabel->setStyleSheet("font-size: 28px; font-weight: bold; color: #303133;");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLayout->addWidget(titleLabel);

    QLabel* subtitleLabel = new QLabel("记录运动饮食，管理您的健康生活", this);
    subtitleLabel->setStyleSheet("color: #909399; font-size: 13px;");
    subtitleLabel->setAlignment(Qt::AlignCenter);
    titleLayout->addWidget(subtitleLabel);

    mainLayout->addLayout(titleLayout);
    mainLayout->addSpacing(20);

    // 用户名输入
    QHBoxLayout* userLayout = new QHBoxLayout();
    userLayout->setSpacing(0);
    QLabel* userIcon = new QLabel("👤", this);
    userIcon->setFixedSize(44, 44);
    userIcon->setAlignment(Qt::AlignCenter);
    userIcon->setStyleSheet("background-color: #f5f7fa; border: 1px solid #dcdfe6; border-right: none; border-top-left-radius: 8px; border-bottom-left-radius: 8px; font-size: 16px;");
    usernameEdit = new QLineEdit(this);
    usernameEdit->setPlaceholderText("请输入用户名");
    usernameEdit->setFixedHeight(44);
    usernameEdit->setStyleSheet("QLineEdit { border: 1px solid #dcdfe6; border-left: none; border-top-right-radius: 8px; border-bottom-right-radius: 8px; padding: 0 15px; font-size: 14px; } QLineEdit:focus { border-color: #409eff; }");
    userLayout->addWidget(userIcon);
    userLayout->addWidget(usernameEdit);
    mainLayout->addLayout(userLayout);

    // 密码输入
    QHBoxLayout* pwdLayout = new QHBoxLayout();
    pwdLayout->setSpacing(0);
    QLabel* pwdIcon = new QLabel("🔒", this);
    pwdIcon->setFixedSize(44, 44);
    pwdIcon->setAlignment(Qt::AlignCenter);
    pwdIcon->setStyleSheet("background-color: #f5f7fa; border: 1px solid #dcdfe6; border-right: none; border-top-left-radius: 8px; border-bottom-left-radius: 8px; font-size: 16px;");
    passwordEdit = new QLineEdit(this);
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setPlaceholderText("请输入密码");
    passwordEdit->setFixedHeight(44);
    passwordEdit->setStyleSheet("QLineEdit { border: 1px solid #dcdfe6; border-left: none; border-top-right-radius: 8px; border-bottom-right-radius: 8px; padding: 0 15px; font-size: 14px; } QLineEdit:focus { border-color: #409eff; }");
    pwdLayout->addWidget(pwdIcon);
    pwdLayout->addWidget(passwordEdit);
    mainLayout->addLayout(pwdLayout);

    mainLayout->addSpacing(10);

    // 登录按钮
    loginBtn = new QPushButton("登 录", this);
    loginBtn->setFixedHeight(46);
    loginBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #409eff;
            color: white;
            border: none;
            border-radius: 8px;
            font-size: 16px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #66b1ff;
        }
        QPushButton:pressed {
            background-color: #337ecc;
        }
    )");
    mainLayout->addWidget(loginBtn);

    // 注册按钮
    registerBtn = new QPushButton("还没有账号？立即注册", this);
    registerBtn->setFlat(true);
    registerBtn->setCursor(Qt::PointingHandCursor);
    registerBtn->setStyleSheet("QPushButton { color: #409eff; border: none; font-size: 13px; background-color: transparent; } QPushButton:hover { text-decoration: underline; }");
    mainLayout->addWidget(registerBtn);

    mainLayout->addStretch();

    // 版权信息
    QLabel* copyrightLabel = new QLabel("© 2026 健康管理系统 v1.0", this);
    copyrightLabel->setStyleSheet("color: #c0c4cc; font-size: 11px;");
    copyrightLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(copyrightLabel);

    connect(loginBtn, &QPushButton::clicked, this, &LoginDialog::onLoginClick);
    connect(registerBtn, &QPushButton::clicked, this, &LoginDialog::onRegisterClick);

    // 回车键登录
    connect(passwordEdit, &QLineEdit::returnPressed, loginBtn, &QPushButton::click);
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
        emit loginSuccess(user);
        accept();
    }
}

void LoginDialog::onRegisterClick()
{
    RegisterDialog* regDialog = new RegisterDialog(this);
    if (regDialog->exec() == QDialog::Accepted) {
        // 注册成功后自动填充用户名
        usernameEdit->setText(regDialog->getRegisteredUsername());
        passwordEdit->setFocus();
    }
    regDialog->deleteLater();
}
