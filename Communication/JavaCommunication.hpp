#ifndef JAVACOMMUNICATION_H
#define JAVACOMMUNICATION_H

#include <QObject>
#include "AppDefines.hpp"
#include "QFile"

#ifdef ANDROID_KIT
#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>
#include <QtAndroid>
#endif

class JavaCommunication : public QObject
{
    Q_OBJECT

private:
    explicit JavaCommunication(QObject *parent = nullptr);

public:
    static JavaCommunication* instance();

#ifdef ANDROID_KIT

    void openFBLiteApplication(QString packageName, QString activityName) const;
    void openFBLiteWithUserID(QString packageName, QString userID) const;
    bool installFacebookLite(QString apkPath);

    bool requestPermission();
    QString getGoogleSF();
    QString getDeviceIMEI();
    QString getDeiceIMSI();
    QString getDeviceModel();
    QString getAndroidVersion();
    QString getAndroidID();
    QString getSimCardSerialNo();
    QString getSimNumber();
    QString getWifiMacAdd();
    QString getDeviceType();
#endif

private:
    static JavaCommunication* m_instance;
signals:

public slots:
};

#endif // JAVACOMMUNICATION_H
