#ifndef NOTIFYTOOL_H
#define NOTIFYTOOL_H

#include <QObject>
#include <QTimer>
#include <QDateTime>
#include <QList>
#include "../entity/Remind.h"

class NotifyTool : public QObject
{
    Q_OBJECT
public:
    static NotifyTool* getInstance();
    ~NotifyTool();

    void startTimerTask();
    void stopAllRemind();
    void loadReminds(const QList<Remind>& reminds);

private:
    NotifyTool();
    NotifyTool(const NotifyTool&) = delete;
    NotifyTool& operator=(const NotifyTool&) = delete;

    static NotifyTool* instance;
    QTimer* timer;
    QList<Remind> remindList;

private slots:
    void checkRemind();
};

#endif