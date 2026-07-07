#include "ReportWidget.h"
#include <QMessageBox>
#include <QFileDialog>

ReportWidget::ReportWidget(int userId, QWidget* parent)
    : QWidget(parent), currentUserId(userId)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(18);

    QLabel* titleLabel = new QLabel("📊 月度健康报表", this);
    titleLabel->setObjectName("titleLabel");
    mainLayout->addWidget(titleLabel);

    QGroupBox* selectGroup = new QGroupBox("报表选项", this);
    QHBoxLayout* topLayout = new QHBoxLayout(selectGroup);
    topLayout->setSpacing(15);
    QLabel* monthLabel = new QLabel("选择月份:", this);
    monthEdit = new QDateEdit(QDate::currentDate(), this);
    monthEdit->setDisplayFormat("yyyy-MM");
    generateBtn = new QPushButton("📈 生成报表", this);
    generateBtn->setObjectName("successBtn");
    exportBtn = new QPushButton("💾 导出报表", this);
    exportBtn->setObjectName("warningBtn");
    topLayout->addWidget(monthLabel);
    topLayout->addWidget(monthEdit);
    topLayout->addStretch();
    topLayout->addWidget(generateBtn);
    topLayout->addWidget(exportBtn);
    mainLayout->addWidget(selectGroup);

    reportText = new QTextEdit(this);
    reportText->setReadOnly(true);
    reportText->setStyleSheet("font-size: 14px; line-height: 1.6; padding: 10px;");
    reportText->setPlaceholderText("点击“生成报表”按钮查看本月健康统计...");
    mainLayout->addWidget(reportText);

    connect(generateBtn, &QPushButton::clicked, this, &ReportWidget::generateMonthReport);
    connect(exportBtn, &QPushButton::clicked, this, &ReportWidget::exportReportTxt);
}

ReportWidget::~ReportWidget()
{
}

void ReportWidget::generateMonthReport()
{
    QDate date = monthEdit->date();
    int year = date.year();
    int month = date.month();

    QMap<QString, QString> report = reportService.generateMonthReportData(currentUserId, year, month);
    QString suggest = reportService.buildHealthSuggest(currentUserId, year, month);

    QString content = QString("=== %1年%2月健康报表 ===\n\n")
                          .arg(year)
                          .arg(month);
    content += QString("平均BMI: %1\n").arg(report["avgBMI"]);
    content += QString("本月运动消耗: %1 卡路里\n").arg(report["totalSportCal"]);
    content += QString("本月饮食摄入: %1 卡路里\n").arg(report["totalDietCal"]);
    content += QString("运动天数: %1 天\n").arg(report["sportDays"]);
    content += QString("运动达标率: %1\n").arg(report["sportRate"]);
    content += QString("饮食均衡评分: %1\n\n").arg(report["balanceScore"]);
    content += "=== 健康建议 ===\n";
    content += suggest;

    reportText->setText(content);
}

void ReportWidget::exportReportTxt()
{
    if (reportText->toPlainText().isEmpty()) {
        QMessageBox::warning(this, "提示", "请先生成报表");
        return;
    }

    QString filePath = QFileDialog::getSaveFileName(this, "导出报表", "health_report.txt", "Text Files (*.txt)");
    if (!filePath.isEmpty()) {
        if (FileTool::exportToTxt(filePath, reportText->toPlainText())) {
            QMessageBox::information(this, "成功", "导出成功");
        } else {
            QMessageBox::warning(this, "错误", "导出失败");
        }
    }
}