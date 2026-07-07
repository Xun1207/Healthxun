#ifndef REMINDWIDGET_H
#define REMINDWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QTimeEdit>
#include <QCheckBox>
#include <QFormLayout>
#include <QGroupBox>
#include "../service/RemindService.h"
#include "../dao/RemindDAO.h"
#include "../entity/Remind.h"

class RemindWidget : public QWidget
{
    Q_OBJECT
public:
    RemindWidget(int userId, QWidget* parent = nullptr);
    ~RemindWidget();

private slots:
    void saveRemindConfig();

private:
    int currentUserId;
    RemindService remindService;
    RemindDAO remindDAO;

    QComboBox* typeCombo;
    QTimeEdit* timeEdit;
    QCheckBox* enableCheck;
    QPushButton* saveBtn;
};

#endif