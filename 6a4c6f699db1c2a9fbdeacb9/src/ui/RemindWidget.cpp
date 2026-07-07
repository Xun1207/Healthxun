#include "RemindWidget.h"
#include <QMessageBox>

RemindWidget::RemindWidget(int userId, QWidget* parent)
    : QWidget(parent), currentUserId(userId)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QLabel* titleLabel = new QLabel("健康提醒设置", this);
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold;");
    mainLayout->addWidget(titleLabel);
    mainLayout->addSpacing(20);

    QHBoxLayout* typeLayout = new QHBoxLayout();
    QLabel* typeLabel = new QLabel("提醒类型:", this);
    typeCombo = new QComboBox(this);
    typeCombo->addItems({"运动打卡", "三餐记录", "年度体检"});
    typeLayout->addWidget(typeLabel);
    typeLayout->addWidget(typeCombo);
    mainLayout->addLayout(typeLayout);

    QHBoxLayout* timeLayout = new QHBoxLayout();
    QLabel* timeLabel = new QLabel("提醒时间:", this);
    timeEdit = new QTimeEdit(QTime(8, 0), this);
    timeLayout->addWidget(timeLabel);
    timeLayout->addWidget(timeEdit);
    mainLayout->addLayout(timeLayout);

    QHBoxLayout* enableLayout = new QHBoxLayout();
    enableCheck = new QCheckBox("启用提醒", this);
    enableCheck->setChecked(true);
    enableLayout->addWidget(enableCheck);
    mainLayout->addLayout(enableLayout);

    mainLayout->addSpacing(20);

    saveBtn = new QPushButton("保存设置", this);
    mainLayout->addWidget(saveBtn);

    connect(saveBtn, &QPushButton::clicked, this, &RemindWidget::saveRemindConfig);
}

RemindWidget::~RemindWidget()
{
}

void RemindWidget::saveRemindConfig()
{
    QString remindType = typeCombo->currentText();
    QString remindTime = timeEdit->time().toString("HH:mm");
    bool isOpen = enableCheck->isChecked();

    if (remindService.saveUserRemind(currentUserId, remindType, remindTime, isOpen)) {
        QMessageBox::information(this, "成功", "提醒设置已保存");
        remindService.reloadAllTimer(currentUserId);
    } else {
        QMessageBox::warning(this, "错误", "保存失败");
    }
}