#include "InfoWidget.h"
#include <QMessageBox>
#include <QGroupBox>

InfoWidget::InfoWidget(const User& user, QWidget* parent)
    : QWidget(parent), currentUser(user)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(18);

    QLabel* titleLabel = new QLabel("👤 个人健康档案", this);
    titleLabel->setObjectName("titleLabel");
    mainLayout->addWidget(titleLabel);

    QGroupBox* basicGroup = new QGroupBox("基本信息", this);
    QFormLayout* basicForm = new QFormLayout(basicGroup);
    basicForm->setSpacing(15);
    basicForm->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);

    nameEdit = new QLineEdit(user.realName, this);
    basicForm->addRow("姓名:", nameEdit);

    QHBoxLayout* genderLayout = new QHBoxLayout();
    genderLayout->setSpacing(20);
    maleBtn = new QRadioButton("男", this);
    femaleBtn = new QRadioButton("女", this);
    if (user.gender == 1) {
        maleBtn->setChecked(true);
    } else {
        femaleBtn->setChecked(true);
    }
    genderLayout->addWidget(maleBtn);
    genderLayout->addWidget(femaleBtn);
    genderLayout->addStretch();
    basicForm->addRow("性别:", genderLayout);

    ageEdit = new QLineEdit(QString::number(user.age), this);
    ageEdit->setPlaceholderText("请输入年龄");
    basicForm->addRow("年龄:", ageEdit);

    QHBoxLayout* hwLayout = new QHBoxLayout();
    hwLayout->setSpacing(15);
    heightEdit = new QLineEdit(QString::number(user.height), this);
    heightEdit->setPlaceholderText("cm");
    weightEdit = new QLineEdit(QString::number(user.weight), this);
    weightEdit->setPlaceholderText("kg");
    QLabel* cmLabel = new QLabel("cm", this);
    QLabel* kgLabel = new QLabel("kg", this);
    hwLayout->addWidget(heightEdit);
    hwLayout->addWidget(cmLabel);
    hwLayout->addSpacing(20);
    hwLayout->addWidget(weightEdit);
    hwLayout->addWidget(kgLabel);
    hwLayout->addStretch();
    basicForm->addRow("身高/体重:", hwLayout);

    mainLayout->addWidget(basicGroup);

    QGroupBox* healthGroup = new QGroupBox("健康信息", this);
    QFormLayout* healthForm = new QFormLayout(healthGroup);
    healthForm->setSpacing(15);
    healthForm->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);

    historyEdit = new QLineEdit(user.medicalHistory, this);
    historyEdit->setPlaceholderText("如无请填“无”");
    healthForm->addRow("既往病史:", historyEdit);

    allergyEdit = new QLineEdit(user.allergies, this);
    allergyEdit->setPlaceholderText("如无请填“无”");
    healthForm->addRow("过敏史:", allergyEdit);

    // BMI显示区域
    float bmiValue = user.calculateBMI();
    QHBoxLayout* bmiInfoLayout = new QHBoxLayout();
    bmiLabel = new QLabel(this);
    bmiLabel->setObjectName("bmiLabel");
    updateBMIDisplay(bmiValue);
    bmiInfoLayout->addWidget(bmiLabel);
    bmiInfoLayout->addStretch();
    healthForm->addRow("当前BMI:", bmiInfoLayout);

    mainLayout->addWidget(healthGroup);

    mainLayout->addStretch();

    saveBtn = new QPushButton("💾 保存修改", this);
    saveBtn->setMinimumHeight(40);
    saveBtn->setObjectName("successBtn");
    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    btnLayout->addWidget(saveBtn);
    btnLayout->addStretch();
    mainLayout->addLayout(btnLayout);

    connect(saveBtn, &QPushButton::clicked, this, &InfoWidget::saveModifiedInfo);
}

void InfoWidget::updateBMIDisplay(float bmi)
{
    QString bmiText = QString::number(bmi, 'f', 2);
    QString bmiCategory;
    QString color;
    if (bmi < 18.5) {
        bmiCategory = " (偏瘦)";
        color = "#e6a23c";
    } else if (bmi < 24) {
        bmiCategory = " (正常)";
        color = "#67c23a";
    } else if (bmi < 28) {
        bmiCategory = " (偏胖)";
        color = "#e6a23c";
    } else {
        bmiCategory = " (肥胖)";
        color = "#f56c6c";
    }
    bmiLabel->setText(bmiText + bmiCategory);
    // 生成带透明度的背景色: color + "33" 表示20%不透明度
    QString bgColor = color + "33";
    bmiLabel->setStyleSheet(QString("font-size: 18px; font-weight: bold; color: %1; background-color: %2; padding: 8px 20px; border-radius: 6px;").arg(color, bgColor));
}

InfoWidget::~InfoWidget()
{
}

void InfoWidget::saveModifiedInfo()
{
    bool ok;
    int age = ageEdit->text().toInt(&ok);
    if (!ok || age < 0) {
        QMessageBox::warning(this, "提示", "年龄必须为非负整数");
        return;
    }

    float height = heightEdit->text().toFloat(&ok);
    if (!ok || height <= 0) {
        QMessageBox::warning(this, "提示", "身高必须为正数");
        return;
    }

    float weight = weightEdit->text().toFloat(&ok);
    if (!ok || weight <= 0) {
        QMessageBox::warning(this, "提示", "体重必须为正数");
        return;
    }

    User user = currentUser;
    user.realName = nameEdit->text();
    user.gender = maleBtn->isChecked() ? 1 : 0;
    user.age = age;
    user.height = height;
    user.weight = weight;
    user.medicalHistory = historyEdit->text();
    user.allergies = allergyEdit->text();

    if (userService.updateUserInfo(user)) {
        QMessageBox::information(this, "成功", "保存成功");
        currentUser = user;
        updateBMIDisplay(user.calculateBMI());
    } else {
        QMessageBox::warning(this, "错误", "保存失败");
    }
}