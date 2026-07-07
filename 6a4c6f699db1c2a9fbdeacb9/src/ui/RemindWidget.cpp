#include "RemindWidget.h"
#include <QMessageBox>

RemindWidget::RemindWidget(int userId, QWidget* parent)
    : QWidget(parent), currentUserId(userId)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(18);

    QLabel* titleLabel = new QLabel("⏰ 健康提醒设置", this);
    titleLabel->setObjectName("titleLabel");
    mainLayout->addWidget(titleLabel);

    QGroupBox* remindGroup = new QGroupBox("提醒配置", this);
    QFormLayout* formLayout = new QFormLayout(remindGroup);
    formLayout->setSpacing(20);
    formLayout->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);

    typeCombo = new QComboBox(this);
    typeCombo->addItems({"运动打卡", "三餐记录", "年度体检"});
    formLayout->addRow("提醒类型:", typeCombo);

    timeEdit = new QTimeEdit(QTime(8, 0), this);
    timeEdit->setDisplayFormat("HH:mm");
    formLayout->addRow("提醒时间:", timeEdit);

    enableCheck = new QCheckBox("启用该提醒", this);
    enableCheck->setChecked(true);
    enableCheck->setStyleSheet("font-size: 14px;");
    formLayout->addRow("", enableCheck);

    mainLayout->addWidget(remindGroup);

    QLabel* tipLabel = new QLabel("💡 提示：程序运行时，到达提醒时间会自动弹出消息提示。", this);
    tipLabel->setStyleSheet("color: #909399; padding: 10px; background-color: #f4f4f5; border-radius: 6px;");
    tipLabel->setWordWrap(true);
    mainLayout->addWidget(tipLabel);

    mainLayout->addStretch();

    saveBtn = new QPushButton("💾 保存设置", this);
    saveBtn->setMinimumHeight(40);
    saveBtn->setObjectName("successBtn");
    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    btnLayout->addWidget(saveBtn);
    btnLayout->addStretch();
    mainLayout->addLayout(btnLayout);

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