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
    void querySportByCycle();
    void drawSportLineChart();

    void addDietDialog();
    void editDietDialog();
    void delDietRecord();
    void queryDietByDate();
    void exportDietTxt();

private:
    int currentUserId;
    SportService sportService;
    DietService dietService;
    UserService userService;

    QTabWidget* tabWidget;

    QWidget* sportTab;
    QComboBox* sportCycleCombo;
    QPushButton* sportQueryBtn;
    QPushButton* sportAddBtn;
    QPushButton* sportEditBtn;
    QPushButton* sportDelBtn;
    QTableWidget* sportTable;
    QChartView* sportChartView;
    QLabel* sportStatLabel;

    QWidget* dietTab;
    QDateEdit* dietStartDate;
    QDateEdit* dietEndDate;
    QPushButton* dietQueryBtn;
    QPushButton* dietAddBtn;
    QPushButton* dietEditBtn;
    QPushButton* dietDelBtn;
    QPushButton* dietExportBtn;
    QTableWidget* dietTable;
    QLabel* dietStatLabel;

    void loadSportData(int cycleType);
    void loadDietData(const QString& startDate, const QString& endDate);
};

#endif