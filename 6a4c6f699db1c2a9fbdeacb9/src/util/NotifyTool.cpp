#include "NotifyTool.h"
#include <QMessageBox>
#include <QApplication>

NotifyTool* NotifyTool::instance = nullptr;

NotifyTool::NotifyTool()
    : QObject(nullptr)
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &NotifyTool::checkRemind);
}

NotifyTool::~NotifyTool()
{
    stopAllRemind();
    delete timer;
}

NotifyTool* NotifyTool::getInstance()
{
    if (instance == nullptr) {
        instance = new NotifyTool();
    }
    return instance;
}

void NotifyTool::startTimerTask()
{
    timer->start(60000);
    checkRemind();
}

void NotifyTool::stopAllRemind()
{
    timer->stop();
}

void NotifyTool::loadReminds(const QList<Remind>& reminds)
{
    remindList = reminds;
}

void NotifyTool::checkRemind()
{
    QDateTime now = QDateTime::currentDateTime();
    QString currentTime = now.toString("HH:mm");

    for (const Remind& remind : remindList) {
        if (remind.isOpen && remind.remindTime == currentTime) {
            QString title = "健康提醒";
            QString msg = QString("该%1了！").arg(remind.remindType);
            QMessageBox::information(nullptr, title, msg);
        }
    }
}