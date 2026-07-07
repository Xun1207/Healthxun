#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "../service/UserService.h"

class RegisterDialog : public QDialog
{
    Q_OBJECT
public:
    RegisterDialog(QWidget* parent = nullptr);
    ~RegisterDialog();

private slots:
    void submitReg();

private:
    QLineEdit* usernameEdit;
    QLineEdit* passwordEdit;
    QLineEdit* confirmPwdEdit;
    QPushButton* submitBtn;
    QPushButton* cancelBtn;
    UserService userService;
};

#endif