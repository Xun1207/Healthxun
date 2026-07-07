#ifndef INFOWIDGET_H
#define INFOWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include "../entity/User.h"
#include "../service/UserService.h"

class InfoWidget : public QWidget
{
    Q_OBJECT
public:
    InfoWidget(const User& user, QWidget* parent = nullptr);
    ~InfoWidget();

private slots:
    void saveModifiedInfo();

private:
    User currentUser;
    UserService userService;

    QLineEdit* nameEdit;
    QRadioButton* maleBtn;
    QRadioButton* femaleBtn;
    QLineEdit* ageEdit;
    QLineEdit* heightEdit;
    QLineEdit* weightEdit;
    QLineEdit* historyEdit;
    QLineEdit* allergyEdit;
    QPushButton* saveBtn;
    QLabel* bmiLabel;
};

#endif