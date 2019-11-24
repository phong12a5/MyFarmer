#include "MainController.hpp"
#include "Communication/WebAPI.hpp"
#include "Model.hpp"

#define WEB_API         WebAPI::instance()
#define MODEL           Model::instance()
#define JAVA_COM        JavaCommunication::instance()
#define FARM_ACTIONS    FarmActions::instance()

MainController* MainController::m_instance = nullptr;

MainController::MainController(QObject *parent) : QObject(parent)
{
    m_changeScreenTimer.setSingleShot(true);
    m_changeScreenTimer.setInterval(120000);
}


MainController *MainController::instance()
{
    if(m_instance == nullptr){
        m_instance = new MainController();
    }
    return m_instance;
}

void MainController::initController()
{
    LOG;
    // Do nothing;
    connect(&m_changeScreenTimer, SIGNAL(timeout()), this, SLOT(onchangeScreenTimerTimeout()));
//    connect(MODEL,SIGNAL(currentScreenChanged()),this,SLOT(onChangeScreen()));
    connect(MODEL,SIGNAL(nextCurrentControlledObjChanged()),this,SLOT(executeRequiredActions()));
    connect(MODEL,SIGNAL(currentActionListDone()),this,SLOT(updateResult()));
    connect(MODEL,SIGNAL(finishedListObject()),this,SLOT(onFinishedListObject()));
    connect(WEB_API, SIGNAL(downloadCompleted(QStringList,QStringList)), this, SLOT(onDownloadCompleted(QStringList,QStringList)));
}

void MainController::installPackages(QStringList downloadedFile,QStringList downloadedPackage)
{
    for(int i = 0 ; i < downloadedFile.length(); i ++){
        if(!MODEL->getUserDataList()->contains(downloadedPackage.at(i))){
            if(ShellOperation::installPackage(downloadedFile.at(i))){
                USER_DATA data;
                MODEL->getUserDataList()->insert(downloadedPackage.at(i),data);
            }
        }else {
            LOG << downloadedPackage.at(i) << " package  was installed already";
        }
    }
    MODEL->saveUserDataList();
    MODEL->nextCurrentControlledObj();
}

void MainController::downloadAndInstallPackages()
{
    WEB_API->downloadlAllPackages();
}

void MainController::startCheckCurrentScreen()
{
    multiThreadController.startCheckCurrentScreen();
}

void MainController::startNewActivity(QString packageName, QString sxtraData)
{
    multiThreadController.startNewActivity(packageName,sxtraData);
}

void MainController::executeRequiredActions()
{
#ifdef ANDROID_KIT
    if(MODEL->currentControlledUser().uid == ""){
        MODEL->updateCurrentControlleredUser(WEB_API->cloneUserData());
    }else{
        LOG << "User info has storaged already";
    }

    startNewActivity(MODEL->currentControlledPkg(),"");
    MODEL->setCurrentScreen(AppEnums::HMI_UNKNOW_SCREEN);
    startCheckCurrentScreen();
#endif
}

void MainController::updateResult()
{
    WEB_API->getDoResult();
#ifdef ANDROID_KIT
    ShellOperation::killSpecificApp(MODEL->currentControlledPkg());
#endif
}

void MainController::onFinishedListObject()
{
    LOG << "The last user: " << MODEL->currentControlledUser().uid;
    if(MODEL->deviceInfo().isNox == "true"){
#ifdef ANDROID_KIT
        ShellOperation::shellCommand(QString("touch %1%2").arg(ENDSCRIPT_PATH).arg(ENDSCRIPT_FILENAME));
#endif
    }
}

void MainController::onDownloadCompleted(QStringList downloadedFile,QStringList downloadedPackage)
{
    this->installPackages(downloadedFile,downloadedPackage);
}

void MainController::onchangeScreenTimerTimeout()
{
    LOG << MODEL->screenStr(MODEL->currentScreen());
#ifdef ANDROID_KIT
    if(MODEL->currentScreen() == AppEnums::HMI_LOGIN_SCREEN){
        LOG << "Click Login again!";
        if(ShellOperation::findAndClick(LOGIN_BTN)){
            return;
        }
    }

    if(MODEL->currentScreen() != AppEnums::HMI_NEW_FEED_SCREEN){
        ShellOperation::clearPackageData(MODEL->currentControlledPkg());
        MODEL->nextCurrentControlledObj();
    }
#endif
}

void MainController::onChangeScreen(int screenID)
{

    if(screenID == MODEL->currentScreen())
        return;

    MODEL->setCurrentScreen(screenID);
    LOG << "currentScreen: " << MODEL->screenStr(MODEL->currentScreen());

#ifdef ANDROID_KIT
    m_changeScreenTimer.stop();

    switch(MODEL->currentScreen()){
    case AppEnums::HMI_UNKNOW_SCREEN:
        ShellOperation::findAndClick(ENGLISH_BTN);
        break;
    case AppEnums::HMI_SELECT_LANGUAGE_SCREEN:
        ShellOperation::findAndClick(ENGLISH_BTN);
        break;
    case AppEnums::HMI_LOGIN_SCREEN:
    {
        ShellOperation::findAndClick(ENGLISH_BTN);
        delay(500);
        ShellOperation::findAndClick(EMAIL_FIELD, true);
        delay(1000);
        ShellOperation::enterText(MODEL->currentControlledUser().uid);
        if(!MODEL->isNoxDevice()){
            ShellOperation::enterKeyBoard();
        }

        delay(1000);
        ShellOperation::findAndClick(PASSWORD_FIELD, true);
        delay(1000);
        ShellOperation::enterText(MODEL->currentControlledUser().password);
        if(!MODEL->isNoxDevice()){
            ShellOperation::enterKeyBoard();
        }

        delay(1000);
        ShellOperation::findAndClick(LOGIN_BTN);
    }
        break;
    case AppEnums::HMI_MISSING_PASSWORD_SCREEN:
    case AppEnums::HMI_ACCOUNT_NOT_FOUND_SCREEN:
        MODEL->updateCurrentControlleredUser(WEB_API->cloneUserData());
        ShellOperation::clearPackageData(MODEL->currentControlledPkg());
        MODEL->nextCurrentControlledObj();
        break;

    case AppEnums::HMI_INCORRECT_PASSWORD_SCREEN:
    case AppEnums::HMI_CONFIRM_INDENTIFY_SCREEN:
    case AppEnums::HMI_DEACTIVE_ACCOUNT_SCREEN:
        WEB_API->updateCheckPoint();
        MODEL->updateCurrentControlleredUser(WEB_API->cloneUserData());
        ShellOperation::clearPackageData(MODEL->currentControlledPkg());
        MODEL->nextCurrentControlledObj();
        break;
    case AppEnums::HMI_TURNON_FIND_FRIEND_SCREEN:
        ShellOperation::findAndClick(SKIP_FIND_FRIEND_BTN);
        break;
    case AppEnums::HMI_SAVE_LOGIN_INFO_SCREEN:
        ShellOperation::findAndClick(OK_BUTTON);
        break;
    case AppEnums::HMI_CHOOSE_AVATAR_SCREEN:
        ShellOperation::findAndClick(SKIP_AVARTAR);
        break;
    case AppEnums::HMI_ADDFRIEND_SUGGESTION_SCREEN:
        ShellOperation::findAndClick(SKIP_AVARTAR);
        delay(1000);
        JAVA_COM->openFBLiteWithUserID(MODEL->currentControlledPkg(),"");
        break;
    case AppEnums::HMI_NEW_FEED_SCREEN:
        MODEL->clearActionList();
//        FARM_ACTIONS->doActions();
        MODEL->nextCurrentControlledObj();
        break;
    case AppEnums::HMI_LOGIN_AGAIN_SCREEN:
        break;
    default:
        break;
    }

    m_changeScreenTimer.start();
#endif
}

