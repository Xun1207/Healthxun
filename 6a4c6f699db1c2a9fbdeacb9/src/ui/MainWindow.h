#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QStackedWidget>
#include "../entity/User.h"
#include "InfoWidget.h"
#include "SportDietWidget.h"
#include "RemindWidget.h"
#include "ReportWidget.h"
#include "AdminWidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(const User& user, QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onMenuClicked(QAction* action);
    void onLogout();

private:
    User currentUser;
    QStackedWidget* stackedWidget;
    InfoWidget* infoWidget = nullptr;
    SportDietWidget* sportDietWidget = nullptr;
    RemindWidget* remindWidget = nullptr;
    ReportWidget* reportWidget = nullptr;
    AdminWidget* adminWidget = nullptr;
};

#endif