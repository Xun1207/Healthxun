#include "InfoWidget.h"
#include <QMessageBox>
#include <QGroupBox>

InfoWidget::InfoWidget(const User& user, QWidget* parent)
    : QWidget(parent), currentUser(user)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QLabel* titleLabel = new QLabel("个人档案", this);
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold;");
    mainLayout->addWidget(titleLabel);
    mainLayout->addSpacing(20);

    QHBoxLayout* nameLayout = new QHBoxLayout();
    QLabel* nameLabel = new QLabel("姓名:", this);
    nameEdit = new QLineEdit(user.realName, this);
    nameLayout->addWidget(nameLabel);
    nameLayout->addWidget(nameEdit);
    mainLayout->addLayout(nameLayout);

    QHBoxLayout* genderLayout = new QHBoxLayout();
    QLabel* genderLabel = new QLabel("性别:", this);
    maleBtn = new QRadioButton("男", this);
    femaleBtn = new QRadioButton("女", this);
    if (user.gender == 1) {
        maleBtn->setChecked(true);
    } else {
        femaleBtn->setChecked(true);
    }
    genderLayout->addWidget(genderLabel);
    genderLayout->addWidget(maleBtn);
    genderLayout->addWidget(femaleBtn);
    mainLayout->addLayout(genderLayout);

    QHBoxLayout* ageLayout = new QHBoxLayout();
    QLabel* ageLabel = new QLabel("年龄:", this);
    ageEdit = new QLineEdit(QString::number(user.age), this);
    ageLayout->addWidget(ageLabel);
    ageLayout->addWidget(ageEdit);
    mainLayout->addLayout(ageLayout);

    QHBoxLayout* heightLayout = new QHBoxLayout();
    QLabel* heightLabel = new QLabel("身高(cm):", this);
    heightEdit = new QLineEdit(QString::number(user.height), this);
    heightLayout->addWidget(heightLabel);
    heightLayout->addWidget(heightEdit);
    mainLayout->addLayout(heightLayout);

    QHBoxLayout* weightLayout = new QHBoxLayout();
    QLabel* weightLabel = new QLabel("体重(kg):", this);
    weightEdit = new QLineEdit(QString::number(user.weight), this);
    weightLayout->addWidget(weightLabel);
    weightLayout->addWidget(weightEdit);
    mainLayout->addLayout(weightLayout);

    QHBoxLayout* historyLayout = new QHBoxLayout();
    QLabel* historyLabel = new QLabel("既往病史:", this);
    historyEdit = new QLineEdit(user.medicalHistory, this);
    historyLayout->addWidget(historyLabel);
    historyLayout->addWidget(historyEdit);
    mainLayout->addLayout(historyLayout);

    QHBoxLayout* allergyLayout = new QHBoxLayout();
    QLabel* allergyLabel = new QLabel("过敏史:", this);
    allergyEdit = new QLineEdit(user.allergies, this);
    allergyLayout->addWidget(allergyLabel);
    allergyLayout->addWidget(allergyEdit);
    mainLayout->addLayout(allergyLayout);

    QHBoxLayout* bmiLayout = new QHBoxLayout();
    QLabel* bmiTitleLabel = new QLabel("当前BMI:", this);
    bmiLabel = new QLabel(QString::number(user.calculateBMI(), 'f', 2), this);
    bmiLayout->addWidget(bmiTitleLabel);
    bmiLayout->addWidget(bmiLabel);
    mainLayout->addLayout(bmiLayout);

    mainLayout->addSpacing(20);

    saveBtn = new QPushButton("保存", this);
    mainLayout->addWidget(saveBtn);

    connect(saveBtn, &QPushButton::clicked, this, &InfoWidget::saveModifiedInfo);
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
        bmiLabel->setText(QString::number(user.calculateBMI(), 'f', 2));
    } else {
        QMessageBox::warning(this, "错误", "保存失败");
    }
}