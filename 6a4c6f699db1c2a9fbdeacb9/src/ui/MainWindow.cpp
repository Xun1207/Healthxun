#include "MainWindow.h"
#include "../util/NotifyTool.h"
#include "../service/RemindService.h"

MainWindow::MainWindow(const User& user, QWidget* parent)
    : QMainWindow(parent), currentUser(user)
{
    setWindowTitle("健康管理系统");
    setMinimumSize(800, 600);

    QMenuBar* menuBar = new QMenuBar(this);

    QMenu* fileMenu = new QMenu("文件", this);
    QAction* logoutAction = new QAction("退出登录", this);
    fileMenu->addAction(logoutAction);
    menuBar->addMenu(fileMenu);

    QMenu* homeMenu = new QMenu("首页", this);
    QAction* infoAction = new QAction("个人档案", this);
    homeMenu->addAction(infoAction);
    menuBar->addMenu(homeMenu);

    QMenu* recordMenu = new QMenu("记录", this);
    QAction* sportDietAction = new QAction("运动饮食记录", this);
    recordMenu->addAction(sportDietAction);
    menuBar->addMenu(recordMenu);

    QMenu* remindMenu = new QMenu("提醒", this);
    QAction* remindAction = new QAction("健康提醒设置", this);
    remindMenu->addAction(remindAction);
    menuBar->addMenu(remindMenu);

    QMenu* reportMenu = new QMenu("报表", this);
    QAction* reportAction = new QAction("月度健康报表", this);
    reportMenu->addAction(reportAction);
    menuBar->addMenu(reportMenu);

    QAction* adminAction = nullptr;
    if (user.isAdmin) {
        QMenu* adminMenu = new QMenu("管理", this);
        adminAction = new QAction("管理员后台", this);
        adminMenu->addAction(adminAction);
        menuBar->addMenu(adminMenu);
    }

    setMenuBar(menuBar);

    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    infoWidget = new InfoWidget(user);
    stackedWidget->addWidget(infoWidget);

    sportDietWidget = new SportDietWidget(user.userId);
    stackedWidget->addWidget(sportDietWidget);

    remindWidget = new RemindWidget(user.userId);
    stackedWidget->addWidget(remindWidget);

    reportWidget = new ReportWidget(user.userId);
    stackedWidget->addWidget(reportWidget);

    if (user.isAdmin) {
        adminWidget = new AdminWidget();
        stackedWidget->addWidget(adminWidget);
    }

    connect(logoutAction, &QAction::triggered, this, &MainWindow::onLogout);
    connect(infoAction, &QAction::triggered, this, [this]() { stackedWidget->setCurrentWidget(infoWidget); });
    connect(sportDietAction, &QAction::triggered, this, [this]() { stackedWidget->setCurrentWidget(sportDietWidget); });
    connect(remindAction, &QAction::triggered, this, [this]() { stackedWidget->setCurrentWidget(remindWidget); });
    connect(reportAction, &QAction::triggered, this, [this]() { stackedWidget->setCurrentWidget(reportWidget); });

    if (user.isAdmin && adminAction != nullptr) {
        connect(adminAction, &QAction::triggered, this, [this]() { stackedWidget->setCurrentWidget(adminWidget); });
    }

    RemindService remindService;
    remindService.reloadAllTimer(user.userId);
}

MainWindow::~MainWindow()
{
}

void MainWindow::onMenuClicked(QAction* action)
{
}

void MainWindow::onLogout()
{
    NotifyTool::getInstance()->stopAllRemind();
    close();
}