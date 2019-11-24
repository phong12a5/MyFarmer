#include "FarmActions.h"
#include "Controller/ShellOperation.hpp"

FarmActions* FarmActions::m_instance = nullptr;

FarmActions::FarmActions(QObject *parent) : QObject(parent)
{

}

FarmActions *FarmActions::instance()
{
    QMutex mutex;
    mutex.lock();
    if(m_instance == nullptr){
        m_instance = new FarmActions();
    }
    mutex.unlock();
    return m_instance;
}

void FarmActions::addFriend(int count)
{
    LOG << "count: " << count;
    delay(500);
    int limit = 0;
    while(!(ShellOperation::findAndClick(SETTING_ICON) ||
            ShellOperation::findAndClick(SETTING_ICON_2) ||
            ShellOperation::findAndClick(SETTING_ICON_3) ||
            ShellOperation::findAndClick(SETTING_ICON_4))){
        delay(1000);
        limit ++;
        if(limit >= 30)
            return;
    }

    limit = 0;
    while(!ShellOperation::findAndClick(FIND_FRIEND_ICON)){
        ShellOperation::callScrollEvent(QPoint(500,1000),QPoint(500,500));
        delay(1000);
        limit ++;
        if(limit >= 30)
            return;
    }

    limit = 0;
    while (count > 0) {
        if(limit >= 30)
            return;
        limit ++;

        int clickConfirm = ShellOperation::findAndClickList(CONFIRM_FRIEND_ICON);
        if(clickConfirm <= 0)
            clickConfirm = ShellOperation::findAndClickList(CONFIRM_FRIEND_ICON_2);
        if(clickConfirm > 0){
            count -= clickConfirm;
            ShellOperation::callScrollEvent(QPoint(300,850),QPoint(300,10));
            continue;
        }
        LOG << "clickConfirm: " << clickConfirm;

        int clickAddFriend = ShellOperation::findAndClickList(ADD_FRIEND_ICON);
        if(clickAddFriend <= 0)
            clickAddFriend = ShellOperation::findAndClickList(ADD_FRIEND_ICON_2);
        if(clickAddFriend <= 0)
            clickAddFriend = ShellOperation::findAndClickList(ADD_FRIEND_ICON_3);
        if(clickAddFriend > 0){
            count -= clickAddFriend;
            ShellOperation::callScrollEvent(QPoint(300,850),QPoint(300,10));
            continue;
        }
        LOG << "clickAddFriend: " << clickAddFriend;

        if(ShellOperation::findAndClick(MORE_ICON)){
            continue;
        }
    }
}

void FarmActions::likeNewFeeds(int count)
{
    int failTime = 0;
    while (count > 0) {
        if(ShellOperation::findAndClick(LIKE_ICON)){
            count --;
            failTime = 0;
        }else {
            failTime ++;
        }
        if(failTime >= 10){
            LOG << "Break due Like failure too many time";
            break;
        }
        ShellOperation::callScrollEvent(QPoint(500,1000),QPoint(500,200));
        delay(1000);
    }
}
