#ifndef ADMINWIDGET_H
#define ADMINWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QTabWidget>
#include <QLineEdit>
#include <QComboBox>
#include "../service/UserService.h"
#include "../service/SystemService.h"
#include "../entity/FoodStandard.h"

class AdminWidget : public QWidget
{
    Q_OBJECT
public:
    AdminWidget(QWidget* parent = nullptr);
    ~AdminWidget();

private slots:
    void loadAllUserList();
    void freezeUserAccount();
    void resetUserPwd();

    void loadFoodStandard();
    void addFoodStandard();
    void editFoodStandard();
    void deleteFoodStandard();

    void backupWholeDB();
    void restoreDBFile();

    void showAllFeedback();

private:
    UserService userService;
    SystemService systemService;

    QTabWidget* tabWidget;

    QWidget* userTab;
    QTableWidget* userTable;
    QPushButton* freezeBtn;
    QPushButton* resetPwdBtn;

    QWidget* foodTab;
    QTableWidget* foodTable;
    QPushButton* addFoodBtn;
    QPushButton* editFoodBtn;
    QPushButton* deleteFoodBtn;

    QWidget* backupTab;
    QPushButton* backupBtn;
    QPushButton* restoreBtn;

    QWidget* feedbackTab;
    QTableWidget* feedbackTable;
};

#endif