#ifndef REPORTWIDGET_H
#define REPORTWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QDateEdit>
#include <QTextEdit>
#include <QGroupBox>
#include "../service/ReportService.h"
#include "../util/FileTool.h"

class ReportWidget : public QWidget
{
    Q_OBJECT
public:
    ReportWidget(int userId, QWidget* parent = nullptr);
    ~ReportWidget();

private slots:
    void generateMonthReport();
    void exportReportTxt();

private:
    int currentUserId;
    ReportService reportService;

    QDateEdit* monthEdit;
    QPushButton* generateBtn;
    QPushButton* exportBtn;
    QTextEdit* reportText;
};

#endif