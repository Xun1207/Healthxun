#include "MainWindow.h"
#include "../util/NotifyTool.h"
#include "../service/RemindService.h"
#include "../service/SystemService.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QButtonGroup>

MainWindow::MainWindow(const User& user, QWidget* parent)
    : QMainWindow(parent), currentUser(user)
{
    setWindowTitle("健康管理系统");
    setMinimumSize(1000, 700);

    // 创建中心部件和主布局
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // ========== 左侧侧边栏 ==========
    QWidget* sidebar = new QWidget(this);
    sidebar->setFixedWidth(220);
    sidebar->setObjectName("sidebar");
    sidebar->setStyleSheet("background-color: #304156;");
    QVBoxLayout* sidebarLayout = new QVBoxLayout(sidebar);
    sidebarLayout->setContentsMargins(0, 0, 0, 0);
    sidebarLayout->setSpacing(0);

    // Logo区域
    QWidget* logoWidget = new QWidget(sidebar);
    logoWidget->setFixedHeight(80);
    logoWidget->setStyleSheet("background-color: #263445;");
    QVBoxLayout* logoLayout = new QVBoxLayout(logoWidget);
    logoLayout->setAlignment(Qt::AlignCenter);
    QLabel* logoLabel = new QLabel("🏥 健康管理", logoWidget);
    logoLabel->setStyleSheet("color: white; font-size: 20px; font-weight: bold;");
    logoLabel->setAlignment(Qt::AlignCenter);
    QLabel* logoSubLabel = new QLabel("Health Manager", logoWidget);
    logoSubLabel->setStyleSheet("color: #909399; font-size: 11px;");
    logoSubLabel->setAlignment(Qt::AlignCenter);
    logoLayout->addWidget(logoLabel);
    logoLayout->addWidget(logoSubLabel);
    sidebarLayout->addWidget(logoWidget);

    // 用户信息区域
    QWidget* userWidget = new QWidget(sidebar);
    userWidget->setFixedHeight(70);
    userWidget->setStyleSheet("background-color: #1f2d3d; border-bottom: 1px solid #263445;");
    QHBoxLayout* userLayout = new QHBoxLayout(userWidget);
    QLabel* avatarLabel = new QLabel(user.isAdmin ? "👨‍💼" : "👤", userWidget);
    avatarLabel->setFixedSize(40, 40);
    avatarLabel->setStyleSheet("font-size: 24px; background-color: #409eff; border-radius: 20px;");
    avatarLabel->setAlignment(Qt::AlignCenter);
    QVBoxLayout* userInfoLayout = new QVBoxLayout();
    QLabel* nameLabel = new QLabel(user.realName.isEmpty() ? user.username : user.realName, userWidget);
    nameLabel->setStyleSheet("color: white; font-weight: bold; font-size: 13px;");
    QLabel* roleLabel = new QLabel(user.isAdmin ? "管理员" : "普通用户", userWidget);
    roleLabel->setStyleSheet("color: #909399; font-size: 11px;");
    userInfoLayout->addWidget(nameLabel);
    userInfoLayout->addWidget(roleLabel);
    userLayout->addWidget(avatarLabel);
    userLayout->addLayout(userInfoLayout);
    sidebarLayout->addWidget(userWidget);

    // 导航按钮区域
    sidebarLayout->addSpacing(10);
    QButtonGroup* navGroup = new QButtonGroup(this);
    navGroup->setExclusive(true);

    // 定义导航项
    struct NavItem {
        QString icon;
        QString text;
        QWidget* widget;
    };

    QList<NavItem> navItems;
    navItems.append({"📋", "个人档案", nullptr});
    navItems.append({"🏃", "运动饮食记录", nullptr});
    navItems.append({"⏰", "健康提醒", nullptr});
    navItems.append({"📊", "健康报表", nullptr});
    if (user.isAdmin) {
        navItems.append({"⚙️", "管理后台", nullptr});
    }

    // 创建内容区域
    stackedWidget = new QStackedWidget(this);

    // 创建各个页面
    infoWidget = new InfoWidget(user);
    stackedWidget->addWidget(infoWidget);
    navItems[0].widget = infoWidget;

    sportDietWidget = new SportDietWidget(user.userId);
    stackedWidget->addWidget(sportDietWidget);
    navItems[1].widget = sportDietWidget;

    remindWidget = new RemindWidget(user.userId);
    stackedWidget->addWidget(remindWidget);
    navItems[2].widget = remindWidget;

    reportWidget = new ReportWidget(user.userId);
    stackedWidget->addWidget(reportWidget);
    navItems[3].widget = reportWidget;

    if (user.isAdmin) {
        adminWidget = new AdminWidget();
        stackedWidget->addWidget(adminWidget);
        navItems[4].widget = adminWidget;
    }

    // 创建导航按钮
    for (int i = 0; i < navItems.size(); ++i) {
        const NavItem& item = navItems[i];
        QPushButton* navBtn = new QPushButton(item.icon + "  " + item.text, sidebar);
        navBtn->setCheckable(true);
        navBtn->setMinimumHeight(48);
        navBtn->setCursor(Qt::PointingHandCursor);
        navBtn->setStyleSheet(R"(
            QPushButton {
                text-align: left;
                padding-left: 24px;
                border: none;
                color: #bfcbd9;
                font-size: 14px;
                background-color: transparent;
            }
            QPushButton:hover {
                background-color: #263445;
                color: white;
            }
            QPushButton:checked {
                background-color: #409eff;
                color: white;
                font-weight: bold;
                border-left: 4px solid #66b1ff;
                padding-left: 20px;
            }
        )");
        navGroup->addButton(navBtn, i);
        sidebarLayout->addWidget(navBtn);

        if (i == 0) {
            navBtn->setChecked(true);
        }
    }

    sidebarLayout->addStretch();

    // 退出登录按钮
    QPushButton* logoutBtn = new QPushButton("🚪  退出登录", sidebar);
    logoutBtn->setMinimumHeight(48);
    logoutBtn->setCursor(Qt::PointingHandCursor);
    logoutBtn->setStyleSheet(R"(
        QPushButton {
            text-align: left;
            padding-left: 24px;
            border: none;
            color: #f56c6c;
            font-size: 14px;
            background-color: transparent;
            border-top: 1px solid #263445;
        }
        QPushButton:hover {
            background-color: #263445;
        }
    )");
    sidebarLayout->addWidget(logoutBtn);

    // 关于按钮
    QPushButton* aboutBtn = new QPushButton("ℹ️  关于系统", sidebar);
    aboutBtn->setMinimumHeight(48);
    aboutBtn->setCursor(Qt::PointingHandCursor);
    aboutBtn->setStyleSheet(R"(
        QPushButton {
            text-align: left;
            padding-left: 24px;
            border: none;
            color: #bfcbd9;
            font-size: 14px;
            background-color: transparent;
        }
        QPushButton:hover {
            background-color: #263445;
            color: white;
        }
    )");
    sidebarLayout->addWidget(aboutBtn);

    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(stackedWidget, 1);

    // ========== 连接信号 ==========
    connect(navGroup, &QButtonGroup::idClicked, this, [this, navItems](int id) {
        if (id >= 0 && id < navItems.size()) {
            stackedWidget->setCurrentWidget(navItems[id].widget);
        }
    });

    connect(logoutBtn, &QPushButton::clicked, this, &MainWindow::onLogout);

    // 意见反馈
    connect(aboutBtn, &QPushButton::clicked, this, [this]() {
        QMessageBox::about(this, "关于健康管理系统",
            "🏥 健康管理系统 v1.0\n\n"
            "一款用于记录运动饮食、管理健康的桌面工具。\n\n"
            "✨ 功能特性：\n"
            "  • 个人健康档案管理\n"
            "  • 运动记录与热量统计\n"
            "  • 饮食记录与营养分析\n"
            "  • 定时健康提醒\n"
            "  • 月度健康报表\n\n"
            "© 2026");
    });

    // 启动时加载提醒
    RemindService remindService;
    remindService.reloadAllTimer(user.userId);
}

MainWindow::~MainWindow()
{
}

void MainWindow::onMenuClicked(QAction* /*action*/)
{
}

void MainWindow::onLogout()
{
    NotifyTool::getInstance()->stopAllRemind();
    close();
}
