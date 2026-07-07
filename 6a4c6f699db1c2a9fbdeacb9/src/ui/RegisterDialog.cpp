#include "RegisterDialog.h"
#include <QMessageBox>

RegisterDialog::RegisterDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("用户注册");
    setFixedSize(420, 420);
    setStyleSheet("QDialog { background-color: white; border-radius: 12px; }");

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(40, 40, 40, 30);
    mainLayout->setSpacing(18);

    QLabel* iconLabel = new QLabel("📝", this);
    iconLabel->setStyleSheet("font-size: 42px;");
    iconLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(iconLabel);

    QLabel* titleLabel = new QLabel("创建新账号", this);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #303133;");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    QLabel* subtitleLabel = new QLabel("注册后即可开始记录您的健康数据", this);
    subtitleLabel->setStyleSheet("color: #909399; font-size: 13px;");
    subtitleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(subtitleLabel);
    mainLayout->addSpacing(10);

    // 用户名
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

    // 密码
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

    // 确认密码
    QHBoxLayout* confirmLayout = new QHBoxLayout();
    confirmLayout->setSpacing(0);
    QLabel* confirmIcon = new QLabel("🔐", this);
    confirmIcon->setFixedSize(44, 44);
    confirmIcon->setAlignment(Qt::AlignCenter);
    confirmIcon->setStyleSheet("background-color: #f5f7fa; border: 1px solid #dcdfe6; border-right: none; border-top-left-radius: 8px; border-bottom-left-radius: 8px; font-size: 16px;");
    confirmPwdEdit = new QLineEdit(this);
    confirmPwdEdit->setEchoMode(QLineEdit::Password);
    confirmPwdEdit->setPlaceholderText("请再次输入密码");
    confirmPwdEdit->setFixedHeight(44);
    confirmPwdEdit->setStyleSheet("QLineEdit { border: 1px solid #dcdfe6; border-left: none; border-top-right-radius: 8px; border-bottom-right-radius: 8px; padding: 0 15px; font-size: 14px; } QLineEdit:focus { border-color: #409eff; }");
    confirmLayout->addWidget(confirmIcon);
    confirmLayout->addWidget(confirmPwdEdit);
    mainLayout->addLayout(confirmLayout);

    mainLayout->addSpacing(15);

    // 按钮区域
    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(12);
    submitBtn = new QPushButton("立即注册", this);
    submitBtn->setFixedHeight(42);
    submitBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #67c23a;
            color: white;
            border: none;
            border-radius: 8px;
            font-size: 15px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #85ce61;
        }
        QPushButton:pressed {
            background-color: #529b2e;
        }
    )");

    cancelBtn = new QPushButton("取消", this);
    cancelBtn->setFixedHeight(42);
    cancelBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #f5f7fa;
            color: #606266;
            border: 1px solid #dcdfe6;
            border-radius: 8px;
            font-size: 15px;
        }
        QPushButton:hover {
            background-color: #ecf5ff;
            color: #409eff;
            border-color: #c6e2ff;
        }
        QPushButton:pressed {
            background-color: #d9ecff;
        }
    )");
    btnLayout->addWidget(submitBtn);
    btnLayout->addWidget(cancelBtn);
    mainLayout->addLayout(btnLayout);

    connect(submitBtn, &QPushButton::clicked, this, &RegisterDialog::submitReg);
    connect(cancelBtn, &QPushButton::clicked, this, &RegisterDialog::reject);
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
        registeredUsername = username;
        QMessageBox::information(this, "成功", "注册成功，请登录");
        accept();
    } else {
        QMessageBox::warning(this, "错误", "注册失败");
    }
}
