#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>
#include "AppDefines.hpp"
#include "Communication/JavaCommunication.hpp"
#include "Processing/ImageProcessing.hpp"
#include "Controller/ThreadController.hpp"
#include "AppEnums.hpp"
#include "Communication/WebAPI.hpp"
#include "Controller/FarmActions.h"

class MainController : public QObject
{
    Q_OBJECT

private:
    explicit MainController(QObject *parent = nullptr);

private:
    void installPackages(QStringList,QStringList);

public:
    static MainController* instance();
    void initController();
    void downloadAndInstallPackages();

public:
    void startCheckCurrentScreen();
    void startNewActivity(QString packageName, QString sxtraData);

private:
    static MainController* m_instance;

    ThreadController multiThreadController;

    QTimer m_changeScreenTimer;

signals:

public slots:
    void onChangeScreen(int screenID);
    void executeRequiredActions();
    void updateResult();
    void onFinishedListObject();
    void onDownloadCompleted(QStringList,QStringList);
    void onchangeScreenTimerTimeout();
};

#endif // MAINCONTROLLER_H
