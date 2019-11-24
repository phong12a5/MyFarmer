#ifndef APPENUMS_H
#define APPENUMS_H

#include <QObject>

class AppEnums : public QObject
{
    Q_OBJECT
    Q_ENUMS( HMI_REQUESTED_ACTIONS )

public:

    enum HMI_REQUESTED_ACTIONS: int
    {
        HMI_CASE_LIKE_NEW_FEED = 0,
        HMI_CASE_ADD_CONFIRM_FRIEND,
        HMI_CASE_ADD_FRIEND_BY_UID,
        HMI_CASE_FOLLOW_PAGE,
        HMI_CASE_LIKE_PAGE,
        HMI_CASE_SHARE_PROFILE,

        HMI_CASE_SHARE_PAGE,
        HMI_CASE_GROUP_LIKE,
        HMI_CASE_GROUP_COMMENT,
        HMI_CASE_UP_PHOTO,
        HMI_CASE_SET_PHOTO_TO_AVT,
        HMI_CASE_UPDATE_INFO,
        HMI_CASE_NONE_REQUEST
    };

    enum HMI_SCREEN_ID: int
    {
        HMI_UNKNOW_SCREEN = 0,
        HMI_SELECT_LANGUAGE_SCREEN,
        HMI_LOGIN_SCREEN,
        HMI_CONFIRM_INDENTIFY_SCREEN,
        HMI_MISSING_PASSWORD_SCREEN,
        HMI_INCORRECT_PASSWORD_SCREEN,
        HMI_DEACTIVE_ACCOUNT_SCREEN,
        HMI_ACCOUNT_NOT_FOUND_SCREEN,
        HMI_TURNON_FIND_FRIEND_SCREEN,
        HMI_SAVE_LOGIN_INFO_SCREEN,
        HMI_CHOOSE_AVATAR_SCREEN,
        HMI_ADDFRIEND_SUGGESTION_SCREEN,
        HMI_NEW_FEED_SCREEN,
        HMI_LOGIN_AGAIN_SCREEN,
        HMI_NUMBER_SCREEN
    };
};

#endif // APPENUMS_H