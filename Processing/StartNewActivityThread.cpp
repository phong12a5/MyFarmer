#include "StartNewActivityThread.hpp"
#include "AppMain.hpp"

StartNewActivityThread::StartNewActivityThread(QObject *parent) : QObject(parent)
{
    // Do nothing
}

StartNewActivityThread::~StartNewActivityThread()
{
    // Do nothing
}

void StartNewActivityThread::doWork(QString packageName, QString sxtraData)
{
    JavaCommunication::instance()->openFBLiteWithUserID(packageName,sxtraData);
}

