#ifndef APPDEFINES_H
#define APPDEFINES_H

#include <QDebug>
#include <QTest>
#include <QMap>
#include <AppEnums.hpp>
#include <QDir>
#include <QThread>

#define LOG qDebug() << "[" << __FUNCTION__ << "][" << __LINE__ << "][" << QThread::currentThreadId() << "] "

#define delay(milescond)        QTest::qSleep(milescond)

#define CURRENT_DIR             QDir::currentPath()+"/"
#define USER_DATA_LIST_PATH     CURRENT_DIR+"userDataList.json"
#define CONFIG_FILE_PATH        CURRENT_DIR+"config.json"

#define API_SERVER              "https://api.autofarmer.xyz/api/"
#define DCIM_FOLDER             "/sdcard/DCIM/"
#define ENDSCRIPT_PATH          "/sdcard/DCIM/"
#define ENDSCRIPT_FILENAME      "endScript.st"
#define INITSCRIPT_FILENAME     "/sdcard/DCIM/initApp.st"

#ifdef SU_USER
#define SHELL_CMD_PREFIX        "su"
#else
#define SHELL_CMD_PREFIX        "sh"
#endif


//#define LOADING_ICON            "/sdcard/DCIM/PDT17/Icons/loadingIcon.png"
//#define LOADING_SCREEN          "/sdcard/DCIM/PDT17/Icons/loadingScreen.png"
//#define LOADING_SCREEN2         "/sdcard/DCIM/PDT17/Icons/loadingScreen2.png"
//#define NEWSFEED_ICON           "/sdcard/DCIM/PDT17/Icons/NewFeedsIcon.png"
//#define NEWSFEED_ICON2          "/sdcard/DCIM/PDT17/Icons/NewFeedsIcon2.png"
//#define SKIP_FIND_FRIEND_BTN    "/sdcard/DCIM/PDT17/Icons/SkipFindFriend.png"
//#define OK_BUTTON               "/sdcard/DCIM/PDT17/Icons/OKButton.png"
//#define TURNON_FIND_FRIEND      "/sdcard/DCIM/PDT17/Icons/TurnOnFindFriendScreen.png"
//#define SAVE_LOGIN_SCREEN       "/sdcard/DCIM/PDT17/Icons/SaveLoginInfoScreen.png"
//#define LIKE_ICON               "/sdcard/DCIM/PDT17/Icons/LikeIcon_FBLite.png"
//#define LOGIN_BTN               "/sdcard/DCIM/PDT17/Icons/LoginBtn.png"
//#define LOGIN_SCREEN            "/sdcard/DCIM/PDT17/Icons/Login_screen.png"
//#define EMAIL_FIELD             "/sdcard/DCIM/PDT17/Icons/MailAndMobileNumberIcon.png"
//#define PASSWORD_FIELD          "/sdcard/DCIM/PDT17/Icons/PasswordIcon.png"
//#define ENGLISH_BTN             "/sdcard/DCIM/PDT17/Icons/EnglishBtn.png"
//#define INCORRECT_PASSWORD      "/sdcard/DCIM/PDT17/Icons/IncorrectPassowrd.png"
//#define CONFIRM_IDENTIFY        "/sdcard/DCIM/PDT17/Icons/ConfirmIdentifyScreen.png"
//#define CONFIRM_IDENTIFY2       "/sdcard/DCIM/PDT17/Icons/ConfirmIdentifyScreen2.png"
//#define CHOOSE_AVATAR           "/sdcard/DCIM/PDT17/Icons/ChooseAvt.png"
//#define SKIP_AVARTAR            "/sdcard/DCIM/PDT17/Icons/SkipAvatar.png"
//#define ADD_FRIEND_SUGGESTION   "/sdcard/DCIM/PDT17/Icons/AddFriendScreen.png"
//#define ADD_FRIEND_SUGGESTION2  "/sdcard/DCIM/PDT17/Icons/AddFriendScreen-2.png"
//#define DOWNLOAD_INFO_BTN       "/sdcard/DCIM/PDT17/Icons/DownloadYourInfomation.png"
//#define MISSING_PWD_SCREEN      "/sdcard/DCIM/PDT17/Icons/MissingPasswordIcon.png"
//#define SELECT_LANGUAGE_SCREEN  "/sdcard/DCIM/PDT17/Icons/SelectLanguage.png"
//#define LOGIN_AGAIN_ICON        "/sdcard/DCIM/PDT17/Icons/LoginAgain.png"
//#define ACC_NOT_FOUND           "/sdcard/DCIM/PDT17/Icons/AccountNotFound.png"

#define EXTERNAL_DATA_DIR       "/sdcard/MyFarmer/"
#define SETTING_ICON            "/sdcard/MyFarmer/Icons/setting_icon.png"
#define SETTING_ICON_2          "/sdcard/MyFarmer/Icons/setting_icon_2.png"
#define SETTING_ICON_3          "/sdcard/MyFarmer/Icons/setting_icon_3.png"
#define SETTING_ICON_4          "/sdcard/MyFarmer/Icons/setting_icon_4.png"
#define FIND_FRIEND_ICON        "/sdcard/MyFarmer/Icons/find_friend.png"
#define ADD_FRIEND_ICON         "/sdcard/MyFarmer/Icons/add_friend.png"
#define ADD_FRIEND_ICON_2       "/sdcard/MyFarmer/Icons/add_friend_2.png"
#define ADD_FRIEND_ICON_3       "/sdcard/MyFarmer/Icons/add_friend_3.png"
#define CONFIRM_FRIEND_ICON     "/sdcard/MyFarmer/Icons/confirm_friend.png"
#define CONFIRM_FRIEND_ICON_2   "/sdcard/MyFarmer/Icons/confirm_friend_2.png"
#define MORE_ICON               "/sdcard/MyFarmer/Icons/more_icon.png"
#define LIKE_ICON               "/sdcard/MyFarmer/Icons/like_icon.png"

#define FACEBOOK_LITE_SCREEN    "com.facebook.lite/com.facebook.lite.MainActivity"
#define FACEBOOK_LITE_PKGNAME   "com.facebook.lite"
#define FACEBOOK_LITE_ACT       "MainActivity"

#define TOKEN_PROP_KEY          "farm_token"
#define AUTO_START_KEY          "auto_start"

#define NUMBER_CLICK            5

#define EMAIL_FIELD_POS_X       540
#define EMAIL_FIELD_POS_Y       310
#define PASSWD_FIELD_POS_X      540
#define PASSWD_FIELD_POS_Y      520
#define LOGIN_AGAIN_POS_X       340
#define LOGIN_AGAIN_POS_Y       340
#define STD_SCREEN_WIDTH        1080
#define STD_SCREEN_HEIGHT       1920

typedef struct display_info
{
    int width;
    int height;
    int dpi;
    display_info(){
        width = 1920;
        height = 1080;
        dpi = 480;
    }
} DISPLAY_INFO;

typedef struct device_info_struct {
    QString googleSF;
    QString androidID;
    QString iMEI;
    QString iMSI;
    QString sIMCardSerial;
    QString wifiMacAddress;
    QString android_verion;
    QString model;
    QString isNox;
    DISPLAY_INFO disInfo;

    device_info_struct(){
        googleSF = "31b75eb8d";
        androidID = "391f3";
        iMEI = "35697865";
        iMSI = "456789987776";
        sIMCardSerial = "89876656";
        wifiMacAddress = "24:26:38";
        android_verion = "5.0.2";
        model = "Sony Z3";
        isNox = "false";
    }
} DEVICE_INFO;

typedef struct app_config_struct {
    int timeout;
    int reset_3g;
    QString debug_mode;
    int user_type;
    QString encryption;

    app_config_struct(){
        timeout = 30;
        reset_3g = 10;
        debug_mode = "test";
        user_type = 0;
        encryption = "00110001";
    }
} APP_CONFIG;

struct USER_DATA {
    QString _id;
    QString uid;
    QString password;
    QString cookie;
    QString token;
    QString birthday;
    QString name;
    QString sex;
    QString country;
    QString email;
    QString avartar;
    QString created_date;
    QString farming_status;
    QString alive_status;
    QString created_at;
    QString updated_at;
    QString user_id;

    bool operator ==(USER_DATA& input){
        if(this->_id            == input._id            &&\
           this->uid            == input.uid            &&\
           this->password       == input.password       &&\
           this->cookie         == input.cookie         &&\
           this->token          == input.token          &&\
           this->birthday       == input.birthday       &&\
           this->name           == input.name           &&\
           this->sex            == input.sex            &&\
           this->country        == input.country        &&\
           this->email          == input.email          &&\
           this->avartar        == input.avartar        &&\
           this->created_date   == input.created_date   &&\
           this->farming_status == input.farming_status &&\
           this->alive_status   == input.alive_status   &&\
           this->created_at     == input.created_at     &&\
           this->updated_at     == input.updated_at     &&\
           this->user_id        == input.user_id)
        {
            return true;
        }else{
            return false;
        }
    }

    bool operator !=(USER_DATA& input){
        if(this->_id            != input._id            ||\
           this->uid            != input.uid            ||\
           this->password       != input.password       ||\
           this->cookie         != input.cookie         ||\
           this->token          != input.token          ||\
           this->birthday       != input.birthday       ||\
           this->name           != input.name           ||\
           this->sex            != input.sex            ||\
           this->country        != input.country        ||\
           this->email          != input.email          ||\
           this->avartar        != input.avartar        ||\
           this->created_date   != input.created_date   ||\
           this->farming_status != input.farming_status ||\
           this->alive_status   != input.alive_status   ||\
           this->created_at     != input.created_at     ||\
           this->updated_at     != input.updated_at     ||\
           this->user_id        != input.user_id)
        {
            return true;
        }else{
            return false;
        }
    }
};

#endif // APPDEFINES_H
