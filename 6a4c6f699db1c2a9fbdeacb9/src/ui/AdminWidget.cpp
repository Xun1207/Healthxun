#include "AdminWidget.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>

AdminWidget::AdminWidget(QWidget* parent)
    : QWidget(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    tabWidget = new QTabWidget(this);
    mainLayout->addWidget(tabWidget);

    userTab = new QWidget(this);
    QVBoxLayout* userLayout = new QVBoxLayout(userTab);

    QHBoxLayout* userBtnLayout = new QHBoxLayout();
    freezeBtn = new QPushButton("冻结/解冻", this);
    resetPwdBtn = new QPushButton("重置密码", this);
    userBtnLayout->addWidget(freezeBtn);
    userBtnLayout->addWidget(resetPwdBtn);
    userLayout->addLayout(userBtnLayout);

    userTable = new QTableWidget(this);
    userTable->setColumnCount(6);
    userTable->setHorizontalHeaderLabels({"ID", "用户名", "姓名", "年龄", "管理员", "冻结状态"});
    userLayout->addWidget(userTable);

    tabWidget->addTab(userTab, "用户管理");

    foodTab = new QWidget(this);
    QVBoxLayout* foodLayout = new QVBoxLayout(foodTab);

    QHBoxLayout* foodBtnLayout = new QHBoxLayout();
    addFoodBtn = new QPushButton("新增食物", this);
    editFoodBtn = new QPushButton("修改热量", this);
    deleteFoodBtn = new QPushButton("删除食物", this);
    foodBtnLayout->addWidget(addFoodBtn);
    foodBtnLayout->addWidget(editFoodBtn);
    foodBtnLayout->addWidget(deleteFoodBtn);
    foodLayout->addLayout(foodBtnLayout);

    foodTable = new QTableWidget(this);
    foodTable->setColumnCount(3);
    foodTable->setHorizontalHeaderLabels({"ID", "食物名称", "每100g热量(卡)"});
    foodLayout->addWidget(foodTable);

    tabWidget->addTab(foodTab, "食物标准库");

    backupTab = new QWidget(this);
    QVBoxLayout* backupLayout = new QVBoxLayout(backupTab);

    backupBtn = new QPushButton("全局数据备份", this);
    restoreBtn = new QPushButton("恢复数据库", this);
    backupLayout->addWidget(backupBtn);
    backupLayout->addWidget(restoreBtn);

    tabWidget->addTab(backupTab, "数据备份");

    feedbackTab = new QWidget(this);
    QVBoxLayout* feedbackLayout = new QVBoxLayout(feedbackTab);

    feedbackTable = new QTableWidget(this);
    feedbackTable->setColumnCount(4);
    feedbackTable->setHorizontalHeaderLabels({"ID", "用户名", "反馈内容", "提交时间"});
    feedbackLayout->addWidget(feedbackTable);

    tabWidget->addTab(feedbackTab, "用户反馈");

    connect(freezeBtn, &QPushButton::clicked, this, &AdminWidget::freezeUserAccount);
    connect(resetPwdBtn, &QPushButton::clicked, this, &AdminWidget::resetUserPwd);

    connect(addFoodBtn, &QPushButton::clicked, this, &AdminWidget::addFoodStandard);
    connect(editFoodBtn, &QPushButton::clicked, this, &AdminWidget::editFoodStandard);
    connect(deleteFoodBtn, &QPushButton::clicked, this, &AdminWidget::deleteFoodStandard);

    connect(backupBtn, &QPushButton::clicked, this, &AdminWidget::backupWholeDB);
    connect(restoreBtn, &QPushButton::clicked, this, &AdminWidget::restoreDBFile);

    connect(tabWidget, &QTabWidget::currentChanged, [this](int index) {
        if (index == 0) loadAllUserList();
        else if (index == 1) loadFoodStandard();
        else if (index == 3) showAllFeedback();
    });

    loadAllUserList();
    loadFoodStandard();
}

AdminWidget::~AdminWidget()
{
}

void AdminWidget::loadAllUserList()
{
    QList<User> users = userService.queryAllUser();

    userTable->setRowCount(0);
    for (const User& user : users) {
        int row = userTable->rowCount();
        userTable->insertRow(row);
        userTable->setItem(row, 0, new QTableWidgetItem(QString::number(user.userId)));
        userTable->setItem(row, 1, new QTableWidgetItem(user.username));
        userTable->setItem(row, 2, new QTableWidgetItem(user.realName));
        userTable->setItem(row, 3, new QTableWidgetItem(QString::number(user.age)));
        userTable->setItem(row, 4, new QTableWidgetItem(user.isAdmin ? "是" : "否"));
        userTable->setItem(row, 5, new QTableWidgetItem(user.isFrozen ? "已冻结" : "正常"));
    }
}

void AdminWidget::freezeUserAccount()
{
    int row = userTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "提示", "请选择一个用户");
        return;
    }

    int userId = userTable->item(row, 0)->text().toInt();
    bool isFrozen = userTable->item(row, 5)->text() == "已冻结";

    QString msg = isFrozen ? "确定解冻该用户吗？" : "确定冻结该用户吗？";
    if (QMessageBox::question(this, "确认", msg) == QMessageBox::Yes) {
        if (userService.freezeUser(userId, !isFrozen)) {
            QMessageBox::information(this, "成功", isFrozen ? "解冻成功" : "冻结成功");
            loadAllUserList();
        } else {
            QMessageBox::warning(this, "错误", "操作失败");
        }
    }
}

void AdminWidget::resetUserPwd()
{
    int row = userTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "提示", "请选择一个用户");
        return;
    }

    int userId = userTable->item(row, 0)->text().toInt();

    if (QMessageBox::question(this, "确认", "确定重置该用户密码吗？\n重置后密码为: 123456") == QMessageBox::Yes) {
        if (userService.resetUserPassword(userId)) {
            QMessageBox::information(this, "成功", "密码重置成功\n新密码: 123456");
        } else {
            QMessageBox::warning(this, "错误", "重置失败");
        }
    }
}

void AdminWidget::loadFoodStandard()
{
    QList<FoodStandard> foods = systemService.getAllFoodStd();

    foodTable->setRowCount(0);
    for (const FoodStandard& food : foods) {
        int row = foodTable->rowCount();
        foodTable->insertRow(row);
        foodTable->setItem(row, 0, new QTableWidgetItem(QString::number(food.foodId)));
        foodTable->setItem(row, 1, new QTableWidgetItem(food.foodName));
        foodTable->setItem(row, 2, new QTableWidgetItem(QString::number(food.per100gCal)));
    }
}

void AdminWidget::addFoodStandard()
{
    QDialog* dialog = new QDialog(this);
    dialog->setWindowTitle("新增食物");

    QVBoxLayout* layout = new QVBoxLayout(dialog);

    QLineEdit* nameEdit = new QLineEdit(dialog);
    layout->addWidget(new QLabel("食物名称:"));
    layout->addWidget(nameEdit);

    QLineEdit* calEdit = new QLineEdit(dialog);
    layout->addWidget(new QLabel("每100g热量:"));
    layout->addWidget(calEdit);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    QPushButton* okBtn = new QPushButton("确定", dialog);
    QPushButton* cancelBtn = new QPushButton("取消", dialog);
    btnLayout->addWidget(okBtn);
    btnLayout->addWidget(cancelBtn);
    layout->addLayout(btnLayout);

    connect(okBtn, &QPushButton::clicked, [=]() {
        bool ok;
        int cal = calEdit->text().toInt(&ok);
        if (!ok || cal <= 0) {
            QMessageBox::warning(dialog, "提示", "热量必须为正数");
            return;
        }
        if (nameEdit->text().isEmpty()) {
            QMessageBox::warning(dialog, "提示", "食物名称不能为空");
            return;
        }

        FoodStandard food;
        food.foodName = nameEdit->text();
        food.per100gCal = cal;

        if (systemService.addFoodStd(food)) {
            QMessageBox::information(dialog, "成功", "添加成功");
            dialog->close();
            loadFoodStandard();
        } else {
            QMessageBox::warning(dialog, "错误", "添加失败");
        }
    });

    connect(cancelBtn, &QPushButton::clicked, dialog, &QDialog::close);

    dialog->exec();
}

void AdminWidget::editFoodStandard()
{
    int row = foodTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "提示", "请选择一条记录");
        return;
    }

    int foodId = foodTable->item(row, 0)->text().toInt();
    QString name = foodTable->item(row, 1)->text();
    int cal = foodTable->item(row, 2)->text().toInt();

    QDialog* dialog = new QDialog(this);
    dialog->setWindowTitle("修改食物热量");

    QVBoxLayout* layout = new QVBoxLayout(dialog);

    QLineEdit* nameEdit = new QLineEdit(name, dialog);
    layout->addWidget(new QLabel("食物名称:"));
    layout->addWidget(nameEdit);

    QLineEdit* calEdit = new QLineEdit(QString::number(cal), dialog);
    layout->addWidget(new QLabel("每100g热量:"));
    layout->addWidget(calEdit);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    QPushButton* okBtn = new QPushButton("确定", dialog);
    QPushButton* cancelBtn = new QPushButton("取消", dialog);
    btnLayout->addWidget(okBtn);
    btnLayout->addWidget(cancelBtn);
    layout->addLayout(btnLayout);

    connect(okBtn, &QPushButton::clicked, [=]() {
        bool ok;
        int newCal = calEdit->text().toInt(&ok);
        if (!ok || newCal <= 0) {
            QMessageBox::warning(dialog, "提示", "热量必须为正数");
            return;
        }
        if (nameEdit->text().isEmpty()) {
            QMessageBox::warning(dialog, "提示", "食物名称不能为空");
            return;
        }

        FoodStandard food;
        food.foodId = foodId;
        food.foodName = nameEdit->text();
        food.per100gCal = newCal;

        if (systemService.updateFoodStd(food)) {
            QMessageBox::information(dialog, "成功", "修改成功");
            dialog->close();
            loadFoodStandard();
        } else {
            QMessageBox::warning(dialog, "错误", "修改失败");
        }
    });

    connect(cancelBtn, &QPushButton::clicked, dialog, &QDialog::close);

    dialog->exec();
}

void AdminWidget::deleteFoodStandard()
{
    int row = foodTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "提示", "请选择一条记录");
        return;
    }

    int foodId = foodTable->item(row, 0)->text().toInt();

    if (QMessageBox::question(this, "确认", "确定删除该食物标准吗？") == QMessageBox::Yes) {
        if (systemService.deleteFoodStd(foodId)) {
            QMessageBox::information(this, "成功", "删除成功");
            loadFoodStandard();
        } else {
            QMessageBox::warning(this, "错误", "删除失败");
        }
    }
}

void AdminWidget::backupWholeDB()
{
    QString dirPath = QDir::currentPath() + "/backup";
    QDir dir(dirPath);
    if (!dir.exists()) {
        dir.mkpath(dirPath);
    }

    QString savePath = dirPath + "/backup_" + QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss") + ".txt";

    if (systemService.backupAllDatabase(savePath)) {
        QMessageBox::information(this, "成功", QString("备份成功！\n文件路径: %1").arg(savePath));
    } else {
        QMessageBox::warning(this, "错误", "备份失败");
    }
}

void AdminWidget::restoreDBFile()
{
    QString filePath = QFileDialog::getOpenFileName(this, "选择备份文件", "", "Text Files (*.txt)");
    if (filePath.isEmpty()) {
        return;
    }

    if (QMessageBox::question(this, "确认", "确定恢复数据库吗？\n此操作将覆盖现有数据！") == QMessageBox::Yes) {
        if (systemService.restoreDBFromFile(filePath)) {
            QMessageBox::information(this, "成功", "恢复成功");
        } else {
            QMessageBox::warning(this, "错误", "恢复失败");
        }
    }
}

void AdminWidget::showAllFeedback()
{
    QList<QMap<QString, QString>> feedbackList = systemService.getAllFeedback();

    feedbackTable->setRowCount(0);
    for (const QMap<QString, QString>& feedback : feedbackList) {
        int row = feedbackTable->rowCount();
        feedbackTable->insertRow(row);
        feedbackTable->setItem(row, 0, new QTableWidgetItem(feedback["feedbackId"]));
        feedbackTable->setItem(row, 1, new QTableWidgetItem(feedback["username"]));
        feedbackTable->setItem(row, 2, new QTableWidgetItem(feedback["content"]));
        feedbackTable->setItem(row, 3, new QTableWidgetItem(feedback["createTime"]));
    }
}