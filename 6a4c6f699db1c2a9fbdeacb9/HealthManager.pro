QT       += core gui sql charts widgets

TARGET = HealthManager
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++17

SOURCES += \
    main.cpp \
    src/db/SqliteHelper.cpp \
    src/entity/User.cpp \
    src/entity/SportRecord.cpp \
    src/entity/DietRecord.cpp \
    src/entity/Remind.cpp \
    src/entity/FoodStandard.cpp \
    src/dao/UserDAO.cpp \
    src/dao/SportDAO.cpp \
    src/dao/DietDAO.cpp \
    src/dao/RemindDAO.cpp \
    src/dao/FoodStdDAO.cpp \
    src/service/UserService.cpp \
    src/service/SportService.cpp \
    src/service/DietService.cpp \
    src/service/RemindService.cpp \
    src/service/ReportService.cpp \
    src/service/SystemService.cpp \
    src/util/CalUtil.cpp \
    src/util/EncryptTool.cpp \
    src/util/NotifyTool.cpp \
    src/util/FileTool.cpp \
    src/ui/LoginDialog.cpp \
    src/ui/RegisterDialog.cpp \
    src/ui/MainWindow.cpp \
    src/ui/InfoWidget.cpp \
    src/ui/SportDietWidget.cpp \
    src/ui/RemindWidget.cpp \
    src/ui/ReportWidget.cpp \
    src/ui/AdminWidget.cpp

HEADERS += \
    src/db/SqliteHelper.h \
    src/entity/User.h \
    src/entity/SportRecord.h \
    src/entity/DietRecord.h \
    src/entity/Remind.h \
    src/entity/FoodStandard.h \
    src/dao/UserDAO.h \
    src/dao/SportDAO.h \
    src/dao/DietDAO.h \
    src/dao/RemindDAO.h \
    src/dao/FoodStdDAO.h \
    src/service/UserService.h \
    src/service/SportService.h \
    src/service/DietService.h \
    src/service/RemindService.h \
    src/service/ReportService.h \
    src/service/SystemService.h \
    src/util/CalUtil.h \
    src/util/EncryptTool.h \
    src/util/NotifyTool.h \
    src/util/FileTool.h \
    src/ui/LoginDialog.h \
    src/ui/RegisterDialog.h \
    src/ui/MainWindow.h \
    src/ui/InfoWidget.h \
    src/ui/SportDietWidget.h \
    src/ui/RemindWidget.h \
    src/ui/ReportWidget.h \
    src/ui/AdminWidget.h

RESOURCES += \
    resources.qrc