#include "SportDietWidget.h"
#include <QMessageBox>
#include <QDateTime>
#include <QFileDialog>
#include <QHeaderView>
#include "../util/FileTool.h"

SportDietWidget::SportDietWidget(int userId, QWidget* parent)
    : QWidget(parent), currentUserId(userId), sportDateMode(1), dietDateMode(1)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    tabWidget = new QTabWidget(this);
    mainLayout->addWidget(tabWidget);

    // ========== 运动记录Tab ==========
    sportTab = new QWidget(this);
    QVBoxLayout* sportLayout = new QVBoxLayout(sportTab);
    sportLayout->setContentsMargins(5, 5, 5, 5);
    sportLayout->setSpacing(12);

    // 顶部筛选栏
    QHBoxLayout* sportTopLayout = new QHBoxLayout();
    sportTopLayout->setSpacing(10);
    sportCycleCombo = new QComboBox(this);
    sportCycleCombo->addItem("今日", 1);
    sportCycleCombo->addItem("本周", 2);
    sportCycleCombo->addItem("本月", 3);
    sportCycleCombo->addItem("自定义", 4);
    sportCycleCombo->setMinimumWidth(100);

    sportCustomDateWidget = new QWidget(this);
    QHBoxLayout* sportCustomLayout = new QHBoxLayout(sportCustomDateWidget);
    sportCustomLayout->setContentsMargins(0, 0, 0, 0);
    sportCustomLayout->setSpacing(5);
    sportCustomStartDate = new QDateEdit(QDate::currentDate(), this);
    sportCustomStartDate->setDisplayFormat("yyyy-MM-dd");
    sportCustomEndDate = new QDateEdit(QDate::currentDate(), this);
    sportCustomEndDate->setDisplayFormat("yyyy-MM-dd");
    sportCustomLayout->addWidget(new QLabel("从:"));
    sportCustomLayout->addWidget(sportCustomStartDate);
    sportCustomLayout->addWidget(new QLabel("到:"));
    sportCustomLayout->addWidget(sportCustomEndDate);
    sportCustomDateWidget->hide();

    sportAddBtn = new QPushButton("➕ 新增记录", this);
    sportAddBtn->setObjectName("successBtn");
    sportEditBtn = new QPushButton("✏️ 修改", this);
    sportDelBtn = new QPushButton("🗑️ 删除", this);
    sportDelBtn->setObjectName("delBtn");

    sportTopLayout->addWidget(new QLabel("时间筛选:"));
    sportTopLayout->addWidget(sportCycleCombo);
    sportTopLayout->addWidget(sportCustomDateWidget);
    sportTopLayout->addStretch();
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
    sportTable->setAlternatingRowColors(true);
    sportTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    sportTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    sportTable->horizontalHeader()->setStretchLastSection(true);
    sportLayout->addWidget(sportTable, 1);

    sportChartView = new QChartView(this);
    sportChartView->setFixedHeight(250);
    sportLayout->addWidget(sportChartView);

    tabWidget->addTab(sportTab, "🏃 运动记录");

    // ========== 饮食记录Tab ==========
    dietTab = new QWidget(this);
    QVBoxLayout* dietLayout = new QVBoxLayout(dietTab);
    dietLayout->setContentsMargins(5, 5, 5, 5);
    dietLayout->setSpacing(12);

    // 顶部筛选栏
    QHBoxLayout* dietTopLayout = new QHBoxLayout();
    dietTopLayout->setSpacing(10);
    dietCycleCombo = new QComboBox(this);
    dietCycleCombo->addItem("今日", 1);
    dietCycleCombo->addItem("本周", 2);
    dietCycleCombo->addItem("本月", 3);
    dietCycleCombo->addItem("自定义", 4);
    dietCycleCombo->setMinimumWidth(100);

    dietCustomDateWidget = new QWidget(this);
    QHBoxLayout* dietCustomLayout = new QHBoxLayout(dietCustomDateWidget);
    dietCustomLayout->setContentsMargins(0, 0, 0, 0);
    dietCustomLayout->setSpacing(5);
    dietCustomStartDate = new QDateEdit(QDate::currentDate(), this);
    dietCustomStartDate->setDisplayFormat("yyyy-MM-dd");
    dietCustomEndDate = new QDateEdit(QDate::currentDate(), this);
    dietCustomEndDate->setDisplayFormat("yyyy-MM-dd");
    dietCustomLayout->addWidget(new QLabel("从:"));
    dietCustomLayout->addWidget(dietCustomStartDate);
    dietCustomLayout->addWidget(new QLabel("到:"));
    dietCustomLayout->addWidget(dietCustomEndDate);
    dietCustomDateWidget->hide();

    dietAddBtn = new QPushButton("➕ 新增记录", this);
    dietAddBtn->setObjectName("successBtn");
    dietEditBtn = new QPushButton("✏️ 修改", this);
    dietDelBtn = new QPushButton("🗑️ 删除", this);
    dietDelBtn->setObjectName("delBtn");
    dietExportBtn = new QPushButton("📤 导出", this);
    dietExportBtn->setObjectName("warningBtn");

    dietTopLayout->addWidget(new QLabel("时间筛选:"));
    dietTopLayout->addWidget(dietCycleCombo);
    dietTopLayout->addWidget(dietCustomDateWidget);
    dietTopLayout->addStretch();
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
    dietTable->setAlternatingRowColors(true);
    dietTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    dietTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    dietTable->horizontalHeader()->setStretchLastSection(true);
    dietLayout->addWidget(dietTable, 1);

    tabWidget->addTab(dietTab, "🍎 饮食记录");

    // ========== 信号连接 ==========
    connect(sportCycleCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SportDietWidget::onSportDateModeChanged);
    connect(sportCustomStartDate, &QDateEdit::dateChanged, this, &SportDietWidget::querySport);
    connect(sportCustomEndDate, &QDateEdit::dateChanged, this, &SportDietWidget::querySport);
    connect(sportAddBtn, &QPushButton::clicked, this, &SportDietWidget::addSportDialog);
    connect(sportEditBtn, &QPushButton::clicked, this, &SportDietWidget::editSportDialog);
    connect(sportDelBtn, &QPushButton::clicked, this, &SportDietWidget::delSportRecord);

    connect(dietCycleCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SportDietWidget::onDietDateModeChanged);
    connect(dietCustomStartDate, &QDateEdit::dateChanged, this, &SportDietWidget::queryDiet);
    connect(dietCustomEndDate, &QDateEdit::dateChanged, this, &SportDietWidget::queryDiet);
    connect(dietAddBtn, &QPushButton::clicked, this, &SportDietWidget::addDietDialog);
    connect(dietEditBtn, &QPushButton::clicked, this, &SportDietWidget::editDietDialog);
    connect(dietDelBtn, &QPushButton::clicked, this, &SportDietWidget::delDietRecord);
    connect(dietExportBtn, &QPushButton::clicked, this, &SportDietWidget::exportDietTxt);

    // 默认加载今日数据
    querySport();
    queryDiet();
}

SportDietWidget::~SportDietWidget()
{
}

void SportDietWidget::onSportDateModeChanged(int index)
{
    sportDateMode = sportCycleCombo->itemData(index).toInt();
    sportCustomDateWidget->setVisible(sportDateMode == 4);
    querySport();
}

void SportDietWidget::onDietDateModeChanged(int index)
{
    dietDateMode = dietCycleCombo->itemData(index).toInt();
    dietCustomDateWidget->setVisible(dietDateMode == 4);
    queryDiet();
}

void SportDietWidget::getSportDateRange(QString& startDate, QString& endDate)
{
    QDate today = QDate::currentDate();
    if (sportDateMode == 1) { // 今日
        startDate = today.toString("yyyy-MM-dd");
        endDate = today.toString("yyyy-MM-dd");
    } else if (sportDateMode == 2) { // 本周
        int dayOfWeek = today.dayOfWeek();
        startDate = today.addDays(1 - dayOfWeek).toString("yyyy-MM-dd");
        endDate = today.addDays(7 - dayOfWeek).toString("yyyy-MM-dd");
    } else if (sportDateMode == 3) { // 本月
        startDate = today.toString("yyyy-MM-01");
        endDate = today.toString("yyyy-MM-dd");
    } else { // 自定义
        startDate = sportCustomStartDate->date().toString("yyyy-MM-dd");
        endDate = sportCustomEndDate->date().toString("yyyy-MM-dd");
    }
}

void SportDietWidget::getDietDateRange(QString& startDate, QString& endDate)
{
    QDate today = QDate::currentDate();
    if (dietDateMode == 1) { // 今日
        startDate = today.toString("yyyy-MM-dd");
        endDate = today.toString("yyyy-MM-dd");
    } else if (dietDateMode == 2) { // 本周
        int dayOfWeek = today.dayOfWeek();
        startDate = today.addDays(1 - dayOfWeek).toString("yyyy-MM-dd");
        endDate = today.addDays(7 - dayOfWeek).toString("yyyy-MM-dd");
    } else if (dietDateMode == 3) { // 本月
        startDate = today.toString("yyyy-MM-01");
        endDate = today.toString("yyyy-MM-dd");
    } else { // 自定义
        startDate = dietCustomStartDate->date().toString("yyyy-MM-dd");
        endDate = dietCustomEndDate->date().toString("yyyy-MM-dd");
    }
}

void SportDietWidget::addSportDialog()
{
    QDialog dialog(this);
    dialog.setWindowTitle("新增运动记录");
    dialog.setFixedSize(350, 380);
    dialog.setStyleSheet("QDialog { background-color: white; border-radius: 8px; }");

    QVBoxLayout* layout = new QVBoxLayout(&dialog);
    layout->setContentsMargins(25, 25, 25, 25);
    layout->setSpacing(15);

    QLabel* titleLabel = new QLabel("🏃 新增运动记录", &dialog);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #303133;");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);
    layout->addSpacing(5);

    QComboBox* typeCombo = new QComboBox(&dialog);
    typeCombo->addItems({"跑步", "游泳", "瑜伽", "力量训练", "骑行", "跳绳", "步行", "篮球", "羽毛球"});
    layout->addWidget(new QLabel("运动类型:"));
    layout->addWidget(typeCombo);

    QLineEdit* durationEdit = new QLineEdit(&dialog);
    durationEdit->setPlaceholderText("请输入运动时长");
    layout->addWidget(new QLabel("运动时长(分钟):"));
    layout->addWidget(durationEdit);

    QLineEdit* distanceEdit = new QLineEdit(&dialog);
    distanceEdit->setPlaceholderText("请输入运动距离，无距离填0");
    distanceEdit->setText("0");
    layout->addWidget(new QLabel("运动距离(km):"));
    layout->addWidget(distanceEdit);

    QDateEdit* dateEdit = new QDateEdit(QDate::currentDate(), &dialog);
    dateEdit->setDisplayFormat("yyyy-MM-dd");
    dateEdit->setCalendarPopup(true);
    layout->addWidget(new QLabel("记录日期:"));
    layout->addWidget(dateEdit);

    layout->addStretch();

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(10);
    QPushButton* okBtn = new QPushButton("确定", &dialog);
    okBtn->setMinimumHeight(38);
    QPushButton* cancelBtn = new QPushButton("取消", &dialog);
    cancelBtn->setMinimumHeight(38);
    cancelBtn->setStyleSheet("QPushButton { background-color: #f5f7fa; color: #606266; border: 1px solid #dcdfe6; border-radius: 6px; } QPushButton:hover { background-color: #ecf5ff; color: #409eff; border-color: #c6e2ff; }");
    btnLayout->addWidget(okBtn);
    btnLayout->addWidget(cancelBtn);
    layout->addLayout(btnLayout);

    connect(okBtn, &QPushButton::clicked, [&]() {
        bool ok;
        float duration = durationEdit->text().toFloat(&ok);
        if (!ok || duration <= 0) {
            QMessageBox::warning(&dialog, "提示", "时长必须为正数");
            return;
        }
        float distance = distanceEdit->text().toFloat(&ok);
        if (!ok || distance < 0) {
            QMessageBox::warning(&dialog, "提示", "距离不能为负数");
            return;
        }

        QString recordDate = dateEdit->date().toString("yyyy-MM-dd");
        if (sportService.addSport(currentUserId, typeCombo->currentText(), duration, distance, recordDate)) {
            QMessageBox::information(&dialog, "成功", "添加成功");
            dialog.accept();
            querySport();
        } else {
            QMessageBox::warning(&dialog, "错误", "添加失败");
        }
    });

    connect(cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);

    dialog.exec();
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
    QString recordDate = sportTable->item(row, 5)->text();

    QDialog dialog(this);
    dialog.setWindowTitle("修改运动记录");
    dialog.setFixedSize(350, 380);
    dialog.setStyleSheet("QDialog { background-color: white; border-radius: 8px; }");

    QVBoxLayout* layout = new QVBoxLayout(&dialog);
    layout->setContentsMargins(25, 25, 25, 25);
    layout->setSpacing(15);

    QLabel* titleLabel = new QLabel("✏️ 修改运动记录", &dialog);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #303133;");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);
    layout->addSpacing(5);

    QComboBox* typeCombo = new QComboBox(&dialog);
    typeCombo->addItems({"跑步", "游泳", "瑜伽", "力量训练", "骑行", "跳绳", "步行", "篮球", "羽毛球"});
    typeCombo->setCurrentText(type);
    layout->addWidget(new QLabel("运动类型:"));
    layout->addWidget(typeCombo);

    QLineEdit* durationEdit = new QLineEdit(QString::number(duration), &dialog);
    layout->addWidget(new QLabel("运动时长(分钟):"));
    layout->addWidget(durationEdit);

    QLineEdit* distanceEdit = new QLineEdit(QString::number(distance), &dialog);
    layout->addWidget(new QLabel("运动距离(km):"));
    layout->addWidget(distanceEdit);

    QDateEdit* dateEdit = new QDateEdit(QDate::fromString(recordDate, "yyyy-MM-dd"), &dialog);
    dateEdit->setDisplayFormat("yyyy-MM-dd");
    dateEdit->setCalendarPopup(true);
    layout->addWidget(new QLabel("记录日期:"));
    layout->addWidget(dateEdit);

    layout->addStretch();

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(10);
    QPushButton* okBtn = new QPushButton("确定", &dialog);
    okBtn->setMinimumHeight(38);
    QPushButton* cancelBtn = new QPushButton("取消", &dialog);
    cancelBtn->setMinimumHeight(38);
    cancelBtn->setStyleSheet("QPushButton { background-color: #f5f7fa; color: #606266; border: 1px solid #dcdfe6; border-radius: 6px; } QPushButton:hover { background-color: #ecf5ff; color: #409eff; border-color: #c6e2ff; }");
    btnLayout->addWidget(okBtn);
    btnLayout->addWidget(cancelBtn);
    layout->addLayout(btnLayout);

    connect(okBtn, &QPushButton::clicked, [&]() {
        bool ok;
        float newDuration = durationEdit->text().toFloat(&ok);
        if (!ok || newDuration <= 0) {
            QMessageBox::warning(&dialog, "提示", "时长必须为正数");
            return;
        }
        float newDistance = distanceEdit->text().toFloat(&ok);
        if (!ok || newDistance < 0) {
            QMessageBox::warning(&dialog, "提示", "距离不能为负数");
            return;
        }

        QString newDate = dateEdit->date().toString("yyyy-MM-dd");
        if (sportService.updateSport(recordId, typeCombo->currentText(), newDuration, newDistance, newDate)) {
            QMessageBox::information(&dialog, "成功", "修改成功");
            dialog.accept();
            querySport();
        } else {
            QMessageBox::warning(&dialog, "错误", "修改失败");
        }
    });

    connect(cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);

    dialog.exec();
}

void SportDietWidget::delSportRecord()
{
    int row = sportTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "提示", "请选择一条记录");
        return;
    }

    int recordId = sportTable->item(row, 0)->text().toInt();

    if (QMessageBox::question(this, "确认删除", "确定删除该运动记录吗？", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        if (sportService.deleteSport(recordId)) {
            QMessageBox::information(this, "成功", "删除成功");
            querySport();
        } else {
            QMessageBox::warning(this, "错误", "删除失败");
        }
    }
}

void SportDietWidget::querySport()
{
    loadSportData();
}

void SportDietWidget::loadSportData()
{
    QString startDate, endDate;
    getSportDateRange(startDate, endDate);

    QList<SportRecord> records = sportService.querySportByDateRange(currentUserId, startDate, endDate);
    int totalCal = sportService.calcDateRangeSportTotal(currentUserId, startDate, endDate);

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

    QString cycleName;
    if (sportDateMode == 1) cycleName = "今日";
    else if (sportDateMode == 2) cycleName = "本周";
    else if (sportDateMode == 3) cycleName = "本月";
    else cycleName = QString("%1 至 %2").arg(startDate, endDate);
    sportStatLabel->setText(QString("%1运动消耗：%2 卡路里").arg(cycleName).arg(totalCal));

    drawSportLineChart();
}

void SportDietWidget::drawSportLineChart()
{
    QString startDate, endDate;
    getSportDateRange(startDate, endDate);
    QList<SportRecord> records = sportService.querySportByDateRange(currentUserId, startDate, endDate);

    // 先删除旧chart避免内存泄漏
    QChart* oldChart = sportChartView->chart();
    if (oldChart) {
        delete oldChart;
    }

    QChart* chart = new QChart();
    chart->setTitle("运动趋势");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

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
        QString shortDate = dates[i].mid(5);
        axisX->append(shortDate, i);
    }
    axisX->setRange(-0.5, qMax(dates.size() - 0.5, 0.5));
    chart->addAxis(axisX, Qt::AlignBottom);
    durationSeries->attachAxis(axisX);
    calSeries->attachAxis(axisX);

    // 设置双Y轴
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

    if (dates.isEmpty()) {
        chart->setTitle("运动趋势 (暂无数据)");
    }

    sportChartView->setChart(chart);
    sportChartView->setRenderHint(QPainter::Antialiasing);
}

void SportDietWidget::addDietDialog()
{
    QDialog dialog(this);
    dialog.setWindowTitle("新增饮食记录");
    dialog.setFixedSize(350, 420);
    dialog.setStyleSheet("QDialog { background-color: white; border-radius: 8px; }");

    QVBoxLayout* layout = new QVBoxLayout(&dialog);
    layout->setContentsMargins(25, 25, 25, 25);
    layout->setSpacing(15);

    QLabel* titleLabel = new QLabel("🍎 新增饮食记录", &dialog);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #303133;");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);
    layout->addSpacing(5);

    QComboBox* mealCombo = new QComboBox(&dialog);
    mealCombo->addItems({"早餐", "午餐", "晚餐", "加餐"});
    layout->addWidget(new QLabel("餐次:"));
    layout->addWidget(mealCombo);

    QLineEdit* foodEdit = new QLineEdit(&dialog);
    foodEdit->setPlaceholderText("请输入食物名称");
    layout->addWidget(new QLabel("食物名称:"));
    layout->addWidget(foodEdit);

    QLineEdit* weightEdit = new QLineEdit(&dialog);
    weightEdit->setPlaceholderText("请输入食用重量");
    layout->addWidget(new QLabel("食用重量(g):"));
    layout->addWidget(weightEdit);

    QDateTimeEdit* timeEdit = new QDateTimeEdit(QDateTime::currentDateTime(), &dialog);
    timeEdit->setDisplayFormat("yyyy-MM-dd HH:mm");
    timeEdit->setCalendarPopup(true);
    layout->addWidget(new QLabel("记录时间:"));
    layout->addWidget(timeEdit);

    layout->addStretch();

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(10);
    QPushButton* okBtn = new QPushButton("确定", &dialog);
    okBtn->setMinimumHeight(38);
    QPushButton* cancelBtn = new QPushButton("取消", &dialog);
    cancelBtn->setMinimumHeight(38);
    cancelBtn->setStyleSheet("QPushButton { background-color: #f5f7fa; color: #606266; border: 1px solid #dcdfe6; border-radius: 6px; } QPushButton:hover { background-color: #ecf5ff; color: #409eff; border-color: #c6e2ff; }");
    btnLayout->addWidget(okBtn);
    btnLayout->addWidget(cancelBtn);
    layout->addLayout(btnLayout);

    connect(okBtn, &QPushButton::clicked, [&]() {
        bool ok;
        float weight = weightEdit->text().toFloat(&ok);
        if (!ok || weight <= 0) {
            QMessageBox::warning(&dialog, "提示", "重量必须为正数");
            return;
        }
        if (foodEdit->text().trimmed().isEmpty()) {
            QMessageBox::warning(&dialog, "提示", "食物名称不能为空");
            return;
        }

        QString recordTime = timeEdit->dateTime().toString("yyyy-MM-dd HH:mm:ss");
        if (dietService.addDiet(currentUserId, foodEdit->text().trimmed(), weight, mealCombo->currentIndex() + 1, recordTime)) {
            QMessageBox::information(&dialog, "成功", "添加成功");
            dialog.accept();
            queryDiet();

            User user = userService.queryUserById(currentUserId);
            int baseMetabolism = CalUtil::calcBaseMetabolism(user);
            int todayIntake = dietService.calcDayTotalIntake(currentUserId, QDate::currentDate().toString("yyyy-MM-dd"));
            int todaySport = sportService.calcDateRangeSportTotal(currentUserId, QDate::currentDate().toString("yyyy-MM-dd"), QDate::currentDate().toString("yyyy-MM-dd"));
            int gap = CalUtil::calcHeatSurplusGap(todayIntake, todaySport, baseMetabolism);
            QString msg;
            if (gap > 0) {
                msg = QString("今日热量盈余：%1 卡路里").arg(gap);
            } else {
                msg = QString("今日热量缺口：%1 卡路里").arg(-gap);
            }
            QMessageBox::information(this, "热量提示", msg);
        } else {
            QMessageBox::warning(&dialog, "错误", "添加失败");
        }
    });

    connect(cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);

    dialog.exec();
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
    QString recordTime = dietTable->item(row, 5)->text();

    QDialog dialog(this);
    dialog.setWindowTitle("修改饮食记录");
    dialog.setFixedSize(350, 420);
    dialog.setStyleSheet("QDialog { background-color: white; border-radius: 8px; }");

    QVBoxLayout* layout = new QVBoxLayout(&dialog);
    layout->setContentsMargins(25, 25, 25, 25);
    layout->setSpacing(15);

    QLabel* titleLabel = new QLabel("✏️ 修改饮食记录", &dialog);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #303133;");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);
    layout->addSpacing(5);

    QComboBox* mealCombo = new QComboBox(&dialog);
    mealCombo->addItems({"早餐", "午餐", "晚餐", "加餐"});
    mealCombo->setCurrentIndex(mealType - 1);
    layout->addWidget(new QLabel("餐次:"));
    layout->addWidget(mealCombo);

    QLineEdit* foodEdit = new QLineEdit(food, &dialog);
    layout->addWidget(new QLabel("食物名称:"));
    layout->addWidget(foodEdit);

    QLineEdit* weightEdit = new QLineEdit(QString::number(weight), &dialog);
    layout->addWidget(new QLabel("食用重量(g):"));
    layout->addWidget(weightEdit);

    QDateTimeEdit* timeEdit = new QDateTimeEdit(QDateTime::fromString(recordTime, "yyyy-MM-dd HH:mm:ss"), &dialog);
    timeEdit->setDisplayFormat("yyyy-MM-dd HH:mm");
    timeEdit->setCalendarPopup(true);
    layout->addWidget(new QLabel("记录时间:"));
    layout->addWidget(timeEdit);

    layout->addStretch();

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(10);
    QPushButton* okBtn = new QPushButton("确定", &dialog);
    okBtn->setMinimumHeight(38);
    QPushButton* cancelBtn = new QPushButton("取消", &dialog);
    cancelBtn->setMinimumHeight(38);
    cancelBtn->setStyleSheet("QPushButton { background-color: #f5f7fa; color: #606266; border: 1px solid #dcdfe6; border-radius: 6px; } QPushButton:hover { background-color: #ecf5ff; color: #409eff; border-color: #c6e2ff; }");
    btnLayout->addWidget(okBtn);
    btnLayout->addWidget(cancelBtn);
    layout->addLayout(btnLayout);

    connect(okBtn, &QPushButton::clicked, [&]() {
        bool ok;
        float newWeight = weightEdit->text().toFloat(&ok);
        if (!ok || newWeight <= 0) {
            QMessageBox::warning(&dialog, "提示", "重量必须为正数");
            return;
        }
        if (foodEdit->text().trimmed().isEmpty()) {
            QMessageBox::warning(&dialog, "提示", "食物名称不能为空");
            return;
        }

        QString newTime = timeEdit->dateTime().toString("yyyy-MM-dd HH:mm:ss");
        if (dietService.updateDiet(dietId, foodEdit->text().trimmed(), newWeight, mealCombo->currentIndex() + 1, newTime)) {
            QMessageBox::information(&dialog, "成功", "修改成功");
            dialog.accept();
            queryDiet();
        } else {
            QMessageBox::warning(&dialog, "错误", "修改失败");
        }
    });

    connect(cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);

    dialog.exec();
}

void SportDietWidget::delDietRecord()
{
    int row = dietTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "提示", "请选择一条记录");
        return;
    }

    int dietId = dietTable->item(row, 0)->text().toInt();

    if (QMessageBox::question(this, "确认删除", "确定删除该饮食记录吗？", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        if (dietService.deleteDiet(dietId)) {
            QMessageBox::information(this, "成功", "删除成功");
            queryDiet();
        } else {
            QMessageBox::warning(this, "错误", "删除失败");
        }
    }
}

void SportDietWidget::queryDiet()
{
    loadDietData();
}

void SportDietWidget::loadDietData()
{
    QString startDate, endDate;
    getDietDateRange(startDate, endDate);

    QList<DietRecord> records = dietService.queryDietByDate(currentUserId, startDate, endDate);
    int totalCal = dietService.calcDateRangeTotalIntake(currentUserId, startDate, endDate);

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
        mealItem->setData(Qt::UserRole, record.mealType);
        dietTable->setItem(row, 4, mealItem);
        dietTable->setItem(row, 5, new QTableWidgetItem(record.recordTime));
    }

    QString cycleName;
    if (dietDateMode == 1) cycleName = "今日";
    else if (dietDateMode == 2) cycleName = "本周";
    else if (dietDateMode == 3) cycleName = "本月";
    else cycleName = QString("%1 至 %2").arg(startDate, endDate);
    dietStatLabel->setText(QString("%1摄入：%2 卡路里").arg(cycleName).arg(totalCal));
}

void SportDietWidget::exportDietTxt()
{
    QString startDate, endDate;
    getDietDateRange(startDate, endDate);
    QList<DietRecord> records = dietService.queryDietByDate(currentUserId, startDate, endDate);

    QString cycleName;
    if (dietDateMode == 1) cycleName = "今日";
    else if (dietDateMode == 2) cycleName = "本周";
    else if (dietDateMode == 3) cycleName = "本月";
    else cycleName = QString("%1-%2").arg(startDate, endDate);

    QString content = QString("饮食记录报告\n查询区间：%1\n\n").arg(cycleName);
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

    QString defaultName = QString("diet_report_%1.txt").arg(QDate::currentDate().toString("yyyyMMdd"));
    QString filePath = QFileDialog::getSaveFileName(this, "导出文件", defaultName, "Text Files (*.txt)");
    if (!filePath.isEmpty()) {
        if (FileTool::exportToTxt(filePath, content)) {
            QMessageBox::information(this, "成功", "导出成功");
        } else {
            QMessageBox::warning(this, "错误", "导出失败");
        }
    }
}
