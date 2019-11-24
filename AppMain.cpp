#include "AppMain.hpp"
#include "Controller/FarmActions.h"
#include "Communication/JavaCommunication.hpp"
#include "Controller/ShellOperation.hpp"
#include <QTime>

#define MODEL           Model::instance()
#define JAVA_COM        JavaCommunication::instance()

AppMain::AppMain(QObject *parent) : QObject(parent)
{
    LOG;
    m_startAppTimer.setInterval(2000);
    m_startAppTimer.setSingleShot(false);
}

void AppMain::initApplication(QQmlApplicationEngine &engine)
{
    LOG;

    // Coppy icon to /SDCARD/DCIM folder
    if(!QDir(QString(EXTERNAL_DATA_DIR) + "Icons").exists()){
        if(QDir("/sdcard/").mkdir("MyFarmer")){
            QDir(EXTERNAL_DATA_DIR).mkdir("Icons");
        }
    }
    this->coppyFolder("assets:/images/Icons",QString(EXTERNAL_DATA_DIR) + "Icons");

    // Get FBLite List packages
   MODEL->setListFbPackage(ShellOperation::installedFBPkg());

    // Init Main controller
//    MAIN_CONTROLLER->initController();

   // Connect signals -> slots
   connectSignalSlot();

    // Wait for start app by timer
    m_startAppTimer.start();
}

void AppMain::connectSignalSlot() const
{
    LOG;
    QObject::connect(&m_startAppTimer, &QTimer::timeout, this, &AppMain::onStartProgram);
}

void AppMain::coppyFolder(QString src, QString dst)
{
    QDir dir(src);
    if (! dir.exists()){
        LOG << "src folder not exist";
        return;
    }

    foreach (QString d, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        QString dst_path = dst + QDir::separator() + d;
        dir.mkpath(dst_path);
        coppyFolder(src+ QDir::separator() + d, dst_path);
    }

    foreach (QString f, dir.entryList(QDir::Files)) {
        QFile::copy(src + QDir::separator() + f, dst + QDir::separator() + f);
    }
}

void AppMain::onStartProgram()
{
    while (true) {
        foreach (QString package, MODEL->listFbPackage()) {
            JavaCommunication::instance()->openFBLiteWithUserID(package,"");
            delay(1000);
            FarmActions::instance()->addFriend(10);
            JavaCommunication::instance()->openFBLiteWithUserID(package,"");
            delay(1000);
            FarmActions::instance()->likeNewFeeds(QTime::currentTime().msec()%4);
            ShellOperation::stopApp(package);
        }
    }
}
