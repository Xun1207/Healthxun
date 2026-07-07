#ifndef SPORTDIETWIDGET_H
#define SPORTDIETWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QTabWidget>
#include <QDateEdit>
#include <QChart>
#include <QChartView>
#include <QLineSeries>
#include <QBarSeries>
#include <QBarSet>
#include <QValueAxis>
#include <QCategoryAxis>
#include <QRadioButton>
#include <QButtonGroup>
#include "../service/SportService.h"
#include "../service/DietService.h"
#include "../service/UserService.h"
#include "../util/CalUtil.h"
#include "../entity/User.h"

class SportDietWidget : public QWidget
{
    Q_OBJECT
public:
    SportDietWidget(int userId, QWidget* parent = nullptr);
    ~SportDietWidget();

private slots:
    void addSportDialog();
    void editSportDialog();
    void delSportRecord();
    void querySport();
    void drawSportLineChart();

    void addDietDialog();
    void editDietDialog();
    void delDietRecord();
    void queryDiet();
    void exportDietTxt();

    void onSportDateModeChanged(int mode);
    void onDietDateModeChanged(int mode);

private:
    int currentUserId;
    SportService sportService;
    DietService dietService;
    UserService userService;

    QTabWidget* tabWidget;

    // 运动页面
    QWidget* sportTab;
    QComboBox* sportCycleCombo;
    QDateEdit* sportCustomStartDate;
    QDateEdit* sportCustomEndDate;
    QWidget* sportCustomDateWidget;
    int sportDateMode; // 1=今日 2=本周 3=本月 4=自定义
    QPushButton* sportAddBtn;
    QPushButton* sportEditBtn;
    QPushButton* sportDelBtn;
    QTableWidget* sportTable;
    QChartView* sportChartView;
    QLabel* sportStatLabel;

    // 饮食页面
    QWidget* dietTab;
    QComboBox* dietCycleCombo;
    QDateEdit* dietCustomStartDate;
    QDateEdit* dietCustomEndDate;
    QWidget* dietCustomDateWidget;
    int dietDateMode; // 1=今日 2=本周 3=本月 4=自定义
    QPushButton* dietAddBtn;
    QPushButton* dietEditBtn;
    QPushButton* dietDelBtn;
    QPushButton* dietExportBtn;
    QTableWidget* dietTable;
    QLabel* dietStatLabel;

    void loadSportData();
    void loadDietData();
    void getSportDateRange(QString& startDate, QString& endDate);
    void getDietDateRange(QString& startDate, QString& endDate);
};

#endif
