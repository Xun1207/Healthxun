#include "SportDietWidget.h"
#include <QMessageBox>
#include <QDateTime>
#include <QFileDialog>
#include "../util/FileTool.h"

SportDietWidget::SportDietWidget(int userId, QWidget* parent)
    : QWidget(parent), currentUserId(userId)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    tabWidget = new QTabWidget(this);
    mainLayout->addWidget(tabWidget);

    sportTab = new QWidget(this);
    QVBoxLayout* sportLayout = new QVBoxLayout(sportTab);

    QHBoxLayout* sportTopLayout = new QHBoxLayout();
    sportCycleCombo = new QComboBox(this);
    sportCycleCombo->addItem("今日", 1);
    sportCycleCombo->addItem("本周", 2);
    sportCycleCombo->addItem("本月", 3);
    sportQueryBtn = new QPushButton("查询", this);
    sportAddBtn = new QPushButton("新增", this);
    sportAddBtn->setObjectName("successBtn");
    sportEditBtn = new QPushButton("修改", this);
    sportDelBtn = new QPushButton("删除", this);
    sportDelBtn->setObjectName("delBtn");
    sportTopLayout->addWidget(sportCycleCombo);
    sportTopLayout->addStretch();
    sportTopLayout->addWidget(sportQueryBtn);
    sportTopLayout->addWidget(sportAddBtn);
    sportTopLayout->addWidget(sportEditBtn);
    sportTopLayout->addWidget(sportDelBtn);
    sportLayout->addLayout(sportTopLayout);

    sportStatLabel = new QLabel(this);
    sportStatLabel->setObjectName("statLabel");
    sportLayout->addWidget(sportStatLabel);

    sportTable = new QTableWidget(this);
    sportTable->setColumnCount(6);
    sportTable->setHorizontalHeaderLabels({"ID", "运动类型", "时长(分钟)", "距离(km)", "消耗卡路里", "日期"});
    sportLayout->addWidget(sportTable);

    sportChartView = new QChartView(this);
    sportChartView->setFixedHeight(300);
    sportLayout->addWidget(sportChartView);

    tabWidget->addTab(sportTab, "运动记录");

    dietTab = new QWidget(this);
    QVBoxLayout* dietLayout = new QVBoxLayout(dietTab);

    QHBoxLayout* dietTopLayout = new QHBoxLayout();
    dietStartDate = new QDateEdit(QDate::currentDate(), this);
    dietEndDate = new QDateEdit(QDate::currentDate(), this);
    dietQueryBtn = new QPushButton("查询", this);
    dietAddBtn = new QPushButton("新增", this);
    dietAddBtn->setObjectName("successBtn");
    dietEditBtn = new QPushButton("修改", this);
    dietDelBtn = new QPushButton("删除", this);
    dietDelBtn->setObjectName("delBtn");
    dietExportBtn = new QPushButton("导出", this);
    dietExportBtn->setObjectName("warningBtn");
    dietTopLayout->addWidget(new QLabel("开始日期:"));
    dietTopLayout->addWidget(dietStartDate);
    dietTopLayout->addWidget(new QLabel("结束日期:"));
    dietTopLayout->addWidget(dietEndDate);
    dietTopLayout->addStretch();
    dietTopLayout->addWidget(dietQueryBtn);
    dietTopLayout->addWidget(dietAddBtn);
    dietTopLayout->addWidget(dietEditBtn);
    dietTopLayout->addWidget(dietDelBtn);
    dietTopLayout->addWidget(dietExportBtn);
    dietLayout->addLayout(dietTopLayout);

    dietStatLabel = new QLabel(this);
    dietStatLabel->setObjectName("statLabel");
    dietLayout->addWidget(dietStatLabel);

    dietTable = new QTableWidget(this);
    dietTable->setColumnCount(6);
    dietTable->setHorizontalHeaderLabels({"ID", "食物名称", "重量(g)", "摄入卡路里", "餐次", "时间"});
    dietLayout->addWidget(dietTable);

    tabWidget->addTab(dietTab, "饮食记录");

    connect(sportQueryBtn, &QPushButton::clicked, this, &SportDietWidget::querySportByCycle);
    connect(sportAddBtn, &QPushButton::clicked, this, &SportDietWidget::addSportDialog);
    connect(sportEditBtn, &QPushButton::clicked, this, &SportDietWidget::editSportDialog);
    connect(sportDelBtn, &QPushButton::clicked, this, &SportDietWidget::delSportRecord);

    connect(dietQueryBtn, &QPushButton::clicked, this, &SportDietWidget::queryDietByDate);
    connect(dietAddBtn, &QPushButton::clicked, this, &SportDietWidget::addDietDialog);
    connect(dietEditBtn, &QPushButton::clicked, this, &SportDietWidget::editDietDialog);
    connect(dietDelBtn, &QPushButton::clicked, this, &SportDietWidget::delDietRecord);
    connect(dietExportBtn, &QPushButton::clicked, this, &SportDietWidget::exportDietTxt);

    querySportByCycle();
    queryDietByDate();
}

SportDietWidget::~SportDietWidget()
{
}

void SportDietWidget::addSportDialog()
{
    QDialog* dialog = new QDialog(this);
    dialog->setWindowTitle("新增运动记录");

    QVBoxLayout* layout = new QVBoxLayout(dialog);

    QComboBox* typeCombo = new QComboBox(dialog);
    typeCombo->addItems({"跑步", "游泳", "瑜伽", "力量训练", "骑行", "跳绳", "步行", "篮球", "羽毛球"});
    layout->addWidget(new QLabel("运动类型:"));
    layout->addWidget(typeCombo);

    QLineEdit* durationEdit = new QLineEdit(dialog);
    layout->addWidget(new QLabel("运动时长(分钟):"));
    layout->addWidget(durationEdit);

    QLineEdit* distanceEdit = new QLineEdit(dialog);
    layout->addWidget(new QLabel("运动距离(km):"));
    layout->addWidget(distanceEdit);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    QPushButton* okBtn = new QPushButton("确定", dialog);
    QPushButton* cancelBtn = new QPushButton("取消", dialog);
    btnLayout->addWidget(okBtn);
    btnLayout->addWidget(cancelBtn);
    layout->addLayout(btnLayout);

    connect(okBtn, &QPushButton::clicked, [=]() {
        bool ok;
        float duration = durationEdit->text().toFloat(&ok);
        if (!ok || duration <= 0) {
            QMessageBox::warning(dialog, "提示", "时长必须为正数");
            return;
        }
        float distance = distanceEdit->text().toFloat(&ok);
        if (!ok || distance < 0) {
            QMessageBox::warning(dialog, "提示", "距离不能为负数");
            return;
        }

        if (sportService.addSport(currentUserId, typeCombo->currentText(), duration, distance)) {
            QMessageBox::information(dialog, "成功", "添加成功");
            dialog->close();
            querySportByCycle();
        } else {
            QMessageBox::warning(dialog, "错误", "添加失败");
        }
    });

    connect(cancelBtn, &QPushButton::clicked, dialog, &QDialog::close);

    dialog->exec();
}

void SportDietWidget::editSportDialog()
{
    int row = sportTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "提示", "请选择一条记录");
        return;
    }

    int recordId = sportTable->item(row, 0)->text().toInt();
    QString type = sportTable->item(row, 1)->text();
    float duration = sportTable->item(row, 2)->text().toFloat();
    float distance = sportTable->item(row, 3)->text().toFloat();

    QDialog* dialog = new QDialog(this);
    dialog->setWindowTitle("修改运动记录");

    QVBoxLayout* layout = new QVBoxLayout(dialog);

    QComboBox* typeCombo = new QComboBox(dialog);
    typeCombo->addItems({"跑步", "游泳", "瑜伽", "力量训练", "骑行", "跳绳", "步行", "篮球", "羽毛球"});
    typeCombo->setCurrentText(type);
    layout->addWidget(new QLabel("运动类型:"));
    layout->addWidget(typeCombo);

    QLineEdit* durationEdit = new QLineEdit(QString::number(duration), dialog);
    layout->addWidget(new QLabel("运动时长(分钟):"));
    layout->addWidget(durationEdit);

    QLineEdit* distanceEdit = new QLineEdit(QString::number(distance), dialog);
    layout->addWidget(new QLabel("运动距离(km):"));
    layout->addWidget(distanceEdit);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    QPushButton* okBtn = new QPushButton("确定", dialog);
    QPushButton* cancelBtn = new QPushButton("取消", dialog);
    btnLayout->addWidget(okBtn);
    btnLayout->addWidget(cancelBtn);
    layout->addLayout(btnLayout);

    connect(okBtn, &QPushButton::clicked, [=]() {
        bool ok;
        float newDuration = durationEdit->text().toFloat(&ok);
        if (!ok || newDuration <= 0) {
            QMessageBox::warning(dialog, "提示", "时长必须为正数");
            return;
        }
        float newDistance = distanceEdit->text().toFloat(&ok);
        if (!ok || newDistance < 0) {
            QMessageBox::warning(dialog, "提示", "距离不能为负数");
            return;
        }

        if (sportService.updateSport(recordId, typeCombo->currentText(), newDuration, newDistance)) {
            QMessageBox::information(dialog, "成功", "修改成功");
            dialog->close();
            querySportByCycle();
        } else {
            QMessageBox::warning(dialog, "错误", "修改失败");
        }
    });

    connect(cancelBtn, &QPushButton::clicked, dialog, &QDialog::close);

    dialog->exec();
}

void SportDietWidget::delSportRecord()
{
    int row = sportTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "提示", "请选择一条记录");
        return;
    }

    int recordId = sportTable->item(row, 0)->text().toInt();

    if (QMessageBox::question(this, "确认", "确定删除该记录吗？") == QMessageBox::Yes) {
        if (sportService.deleteSport(recordId)) {
            QMessageBox::information(this, "成功", "删除成功");
            querySportByCycle();
        } else {
            QMessageBox::warning(this, "错误", "删除失败");
        }
    }
}

void SportDietWidget::querySportByCycle()
{
    int cycleType = sportCycleCombo->currentData().toInt();
    loadSportData(cycleType);
}

void SportDietWidget::loadSportData(int cycleType)
{
    QList<SportRecord> records = sportService.querySportByCycle(currentUserId, cycleType);
    int totalCal = sportService.calcCycleSportTotal(currentUserId, cycleType);

    sportTable->setRowCount(0);
    for (const SportRecord& record : records) {
        int row = sportTable->rowCount();
        sportTable->insertRow(row);
        sportTable->setItem(row, 0, new QTableWidgetItem(QString::number(record.recordId)));
        sportTable->setItem(row, 1, new QTableWidgetItem(record.sportType));
        sportTable->setItem(row, 2, new QTableWidgetItem(QString::number(record.duration)));
        sportTable->setItem(row, 3, new QTableWidgetItem(QString::number(record.distance)));
        sportTable->setItem(row, 4, new QTableWidgetItem(QString::number(record.calorieBurn)));
        sportTable->setItem(row, 5, new QTableWidgetItem(record.recordDate));
    }

    QString cycleName = "今日";
    if (cycleType == 2) cycleName = "本周";
    else if (cycleType == 3) cycleName = "本月";
    sportStatLabel->setText(QString("%1运动消耗：%2 卡路里").arg(cycleName).arg(totalCal));

    drawSportLineChart();
}

void SportDietWidget::drawSportLineChart()
{
    int cycleType = sportCycleCombo->currentData().toInt();
    QList<SportRecord> records = sportService.querySportByCycle(currentUserId, cycleType);

    // 先删除旧chart避免内存泄漏
    QChart* oldChart = sportChartView->chart();
    if (oldChart) {
        delete oldChart;
    }

    QChart* chart = new QChart();
    chart->setTitle("运动趋势");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QLineSeries* durationSeries = new QLineSeries();
    durationSeries->setName("时长(分钟)");
    durationSeries->setPointsVisible(true);

    QLineSeries* calSeries = new QLineSeries();
    calSeries->setName("消耗卡路里");
    calSeries->setPointsVisible(true);

    QMap<QString, QPair<float, int>> dateData;
    for (const SportRecord& record : records) {
        if (dateData.contains(record.recordDate)) {
            dateData[record.recordDate].first += record.duration;
            dateData[record.recordDate].second += record.calorieBurn;
        } else {
            dateData[record.recordDate] = QPair<float, int>(record.duration, record.calorieBurn);
        }
    }

    QStringList dates = dateData.keys();
    std::sort(dates.begin(), dates.end());

    for (int i = 0; i < dates.size(); i++) {
        durationSeries->append(i, dateData[dates[i]].first);
        calSeries->append(i, dateData[dates[i]].second);
    }

    chart->addSeries(durationSeries);
    chart->addSeries(calSeries);

    // 创建X轴 - 简化日期显示避免重叠
    QCategoryAxis* axisX = new QCategoryAxis();
    axisX->setLabelsPosition(QCategoryAxis::AxisLabelsPositionOnValue);
    for (int i = 0; i < dates.size(); i++) {
        // 只显示MM-dd格式，减少标签长度
        QString shortDate = dates[i].mid(5);
        axisX->append(shortDate, i);
    }
    axisX->setRange(-0.5, qMax(dates.size() - 0.5, 0.5));
    chart->addAxis(axisX, Qt::AlignBottom);
    durationSeries->attachAxis(axisX);
    calSeries->attachAxis(axisX);

    // 设置双Y轴，分别显示时长和卡路里
    QValueAxis* axisYDuration = new QValueAxis();
    axisYDuration->setTitleText("时长(分钟)");
    axisYDuration->setLabelFormat("%.0f");
    axisYDuration->setMin(0);
    chart->addAxis(axisYDuration, Qt::AlignLeft);
    durationSeries->attachAxis(axisYDuration);

    QValueAxis* axisYCal = new QValueAxis();
    axisYCal->setTitleText("卡路里");
    axisYCal->setLabelFormat("%.0f");
    axisYCal->setMin(0);
    chart->addAxis(axisYCal, Qt::AlignRight);
    calSeries->attachAxis(axisYCal);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    if (dates.isEmpty()) {
        chart->setTitle("运动趋势 (暂无数据)");
    }

    sportChartView->setChart(chart);
    sportChartView->setRenderHint(QPainter::Antialiasing);
}

void SportDietWidget::addDietDialog()
{
    QDialog* dialog = new QDialog(this);
    dialog->setWindowTitle("新增饮食记录");

    QVBoxLayout* layout = new QVBoxLayout(dialog);

    QComboBox* mealCombo = new QComboBox(dialog);
    mealCombo->addItems({"早餐", "午餐", "晚餐", "加餐"});
    layout->addWidget(new QLabel("餐次:"));
    layout->addWidget(mealCombo);

    QLineEdit* foodEdit = new QLineEdit(dialog);
    layout->addWidget(new QLabel("食物名称:"));
    layout->addWidget(foodEdit);

    QLineEdit* weightEdit = new QLineEdit(dialog);
    layout->addWidget(new QLabel("食用重量(g):"));
    layout->addWidget(weightEdit);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    QPushButton* okBtn = new QPushButton("确定", dialog);
    QPushButton* cancelBtn = new QPushButton("取消", dialog);
    btnLayout->addWidget(okBtn);
    btnLayout->addWidget(cancelBtn);
    layout->addLayout(btnLayout);

    connect(okBtn, &QPushButton::clicked, [=]() {
        bool ok;
        float weight = weightEdit->text().toFloat(&ok);
        if (!ok || weight <= 0) {
            QMessageBox::warning(dialog, "提示", "重量必须为正数");
            return;
        }
        if (foodEdit->text().isEmpty()) {
            QMessageBox::warning(dialog, "提示", "食物名称不能为空");
            return;
        }

        if (dietService.addDiet(currentUserId, foodEdit->text(), weight, mealCombo->currentIndex() + 1)) {
            QMessageBox::information(dialog, "成功", "添加成功");
            dialog->close();
            queryDietByDate();

            User user = userService.queryUserById(currentUserId);
            int baseMetabolism = CalUtil::calcBaseMetabolism(user);
            int dailyIntake = dietService.calcDayTotalIntake(currentUserId, QDate::currentDate().toString("yyyy-MM-dd"));
            int gap = CalUtil::calcHeatSurplusGap(dailyIntake, 0, baseMetabolism);
            QString msg;
            if (gap > 0) {
                msg = QString("热量盈余：%1 卡路里").arg(gap);
            } else {
                msg = QString("热量缺口：%1 卡路里").arg(-gap);
            }
            QMessageBox::information(this, "热量提示", msg);
        } else {
            QMessageBox::warning(dialog, "错误", "添加失败");
        }
    });

    connect(cancelBtn, &QPushButton::clicked, dialog, &QDialog::close);

    dialog->exec();
}

void SportDietWidget::editDietDialog()
{
    int row = dietTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "提示", "请选择一条记录");
        return;
    }

    int dietId = dietTable->item(row, 0)->text().toInt();
    QString food = dietTable->item(row, 1)->text();
    float weight = dietTable->item(row, 2)->text().toFloat();
    int mealType = dietTable->item(row, 4)->data(Qt::UserRole).toInt();

    QDialog* dialog = new QDialog(this);
    dialog->setWindowTitle("修改饮食记录");

    QVBoxLayout* layout = new QVBoxLayout(dialog);

    QComboBox* mealCombo = new QComboBox(dialog);
    mealCombo->addItems({"早餐", "午餐", "晚餐", "加餐"});
    mealCombo->setCurrentIndex(mealType - 1);
    layout->addWidget(new QLabel("餐次:"));
    layout->addWidget(mealCombo);

    QLineEdit* foodEdit = new QLineEdit(food, dialog);
    layout->addWidget(new QLabel("食物名称:"));
    layout->addWidget(foodEdit);

    QLineEdit* weightEdit = new QLineEdit(QString::number(weight), dialog);
    layout->addWidget(new QLabel("食用重量(g):"));
    layout->addWidget(weightEdit);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    QPushButton* okBtn = new QPushButton("确定", dialog);
    QPushButton* cancelBtn = new QPushButton("取消", dialog);
    btnLayout->addWidget(okBtn);
    btnLayout->addWidget(cancelBtn);
    layout->addLayout(btnLayout);

    connect(okBtn, &QPushButton::clicked, [=]() {
        bool ok;
        float newWeight = weightEdit->text().toFloat(&ok);
        if (!ok || newWeight <= 0) {
            QMessageBox::warning(dialog, "提示", "重量必须为正数");
            return;
        }
        if (foodEdit->text().isEmpty()) {
            QMessageBox::warning(dialog, "提示", "食物名称不能为空");
            return;
        }

        if (dietService.updateDiet(dietId, foodEdit->text(), newWeight, mealCombo->currentIndex() + 1)) {
            QMessageBox::information(dialog, "成功", "修改成功");
            dialog->close();
            queryDietByDate();
        } else {
            QMessageBox::warning(dialog, "错误", "修改失败");
        }
    });

    connect(cancelBtn, &QPushButton::clicked, dialog, &QDialog::close);

    dialog->exec();
}

void SportDietWidget::delDietRecord()
{
    int row = dietTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "提示", "请选择一条记录");
        return;
    }

    int dietId = dietTable->item(row, 0)->text().toInt();

    if (QMessageBox::question(this, "确认", "确定删除该记录吗？") == QMessageBox::Yes) {
        if (dietService.deleteDiet(dietId)) {
            QMessageBox::information(this, "成功", "删除成功");
            queryDietByDate();
        } else {
            QMessageBox::warning(this, "错误", "删除失败");
        }
    }
}

void SportDietWidget::queryDietByDate()
{
    QString startDate = dietStartDate->date().toString("yyyy-MM-dd");
    QString endDate = dietEndDate->date().toString("yyyy-MM-dd");
    loadDietData(startDate, endDate);
}

void SportDietWidget::loadDietData(const QString& startDate, const QString& endDate)
{
    QList<DietRecord> records = dietService.queryDietByDate(currentUserId, startDate, endDate);

    int totalCal = 0;
    for (const DietRecord& record : records) {
        totalCal += record.calorie;
    }

    dietTable->setRowCount(0);
    QStringList mealNames = {"", "早餐", "午餐", "晚餐", "加餐"};
    for (const DietRecord& record : records) {
        int row = dietTable->rowCount();
        dietTable->insertRow(row);
        dietTable->setItem(row, 0, new QTableWidgetItem(QString::number(record.dietId)));
        dietTable->setItem(row, 1, new QTableWidgetItem(record.foodName));
        dietTable->setItem(row, 2, new QTableWidgetItem(QString::number(record.weight)));
        dietTable->setItem(row, 3, new QTableWidgetItem(QString::number(record.calorie)));
        QTableWidgetItem* mealItem = new QTableWidgetItem(mealNames[record.mealType]);
        mealItem->setData(Qt::UserRole, record.mealType); // 存储原始mealType值
        dietTable->setItem(row, 4, mealItem);
        dietTable->setItem(row, 5, new QTableWidgetItem(record.recordTime));
    }

    dietStatLabel->setText(QString("区间摄入：%1 卡路里").arg(totalCal));
}

void SportDietWidget::exportDietTxt()
{
    QString startDate = dietStartDate->date().toString("yyyy-MM-dd");
    QString endDate = dietEndDate->date().toString("yyyy-MM-dd");
    QList<DietRecord> records = dietService.queryDietByDate(currentUserId, startDate, endDate);

    QString content = QString("饮食记录报告\n查询区间：%1 至 %2\n\n").arg(startDate).arg(endDate);
    content += QString("%1\t%2\t%3\t%4\t%5\t%6\n").arg("ID").arg("食物").arg("重量(g)").arg("卡路里").arg("餐次").arg("时间");

    QStringList mealNames = {"", "早餐", "午餐", "晚餐", "加餐"};
    for (const DietRecord& record : records) {
        content += QString("%1\t%2\t%3\t%4\t%5\t%6\n")
                       .arg(record.dietId)
                       .arg(record.foodName)
                       .arg(record.weight)
                       .arg(record.calorie)
                       .arg(mealNames[record.mealType])
                       .arg(record.recordTime);
    }

    QString filePath = QFileDialog::getSaveFileName(this, "导出文件", "diet_report.txt", "Text Files (*.txt)");
    if (!filePath.isEmpty()) {
        if (FileTool::exportToTxt(filePath, content)) {
            QMessageBox::information(this, "成功", "导出成功");
        } else {
            QMessageBox::warning(this, "错误", "导出失败");
        }
    }
}