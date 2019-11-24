#include "JavaCommunication.hpp"
#include "Model.hpp"

#define MODEL           Model::instance()


JavaCommunication* JavaCommunication::m_instance = nullptr;

JavaCommunication::JavaCommunication(QObject *parent) : QObject(parent)
{
    // Do nothing
}

JavaCommunication *JavaCommunication::instance()
{
    if(!m_instance)
        m_instance = new JavaCommunication();
    return m_instance;
}
#ifdef ANDROID_KIT

void JavaCommunication::openFBLiteApplication(QString packageName, QString activityName) const
{
    LOG << "Openning " << packageName + "/" + packageName + "." + activityName;
    QAndroidJniObject activity = QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/QtNative", "activity", "()Landroid/app/Activity;");   //activity is valid
    if ( activity.isValid() )
    {
        // Equivalent to Jave code: 'Intent intent = new Intent();'
        QAndroidJniObject intent("android/content/Intent","()V");
        if ( intent.isValid() )
        {
            QAndroidJniObject param1 = QAndroidJniObject::fromString(packageName);
            QAndroidJniObject param2 = QAndroidJniObject::fromString(packageName + "." + activityName);

            if ( param1.isValid() && param2.isValid() )
            {
                // Equivalent to Jave code: 'intent.setClassName(packageName, activityName);'
                intent.callObjectMethod("setClassName","(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;",param1.object<jobject>(),param2.object<jobject>());

                // Equivalent to Jave code: 'startActivity(intent);'
                activity.callMethod<void>("startActivity","(Landroid/content/Intent;)V",intent.object<jobject>());
            }
        }
    }
}

void JavaCommunication::openFBLiteWithUserID(QString packageName, QString userID) const
{
    LOG << "packageName: " << packageName;
    LOG << "userID: " << userID;

    QAndroidJniObject activity = QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/QtNative", "activity", "()Landroid/app/Activity;");   //activity is valid
    if ( activity.isValid() )
    {
        // Intent(Intent.ACTION_VIEW,Uri.parse("fb://profile/vongocanh.vo"));

        QAndroidJniObject actionView_Field = QAndroidJniObject::getStaticObjectField("android/content/Intent", "ACTION_VIEW", "Ljava/lang/String;");

        QAndroidJniEnvironment env;
        jstring uriString = env->NewStringUTF(QString("https://www.facebook.com/%1").arg(userID).toLocal8Bit().data());
        QAndroidJniObject sUriObj;
        sUriObj = QAndroidJniObject::callStaticObjectMethod("android/net/Uri",\
                                                             "parse",\
                                                             "(Ljava/lang/String;)Landroid/net/Uri;",\
                                                             uriString);

        QAndroidJniObject intent("android/content/Intent","(Ljava/lang/String;Landroid/net/Uri;)V",actionView_Field.object<jobject>(),sUriObj.object<jobject>());
        if ( intent.isValid() )
        {
            QAndroidJniObject param1 = QAndroidJniObject::fromString(packageName);
            QAndroidJniObject param2 = QAndroidJniObject::fromString(packageName + ".MainActivity");

            if ( param1.isValid() && param2.isValid() )
            {
                // Equivalent to Jave code: 'intent.setClassName(packageName, activityName);'
                intent.callObjectMethod("setClassName","(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;",param1.object<jobject>(),param2.object<jobject>());

                // Equivalent to Jave code: 'startActivity(intent);'
                activity.callMethod<void>("startActivity","(Landroid/content/Intent;)V",intent.object<jobject>());
            }
        }
    }
}

bool JavaCommunication::installFacebookLite(QString apkPath)
{
    /*
     *Intent intent = new Intent(Intent.ACTION_VIEW);
     *intent.setDataAndType(Uri.fromFile(new File(Environment.getExternalStorageDirectory() + "/download/" + "app.apk")), "application/vnd.android.package-archive");
     *intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
     *startActivity(intent);
     */
    LOG << apkPath;
    bool retVal;
    if(!QFile(apkPath).exists()){
        LOG << apkPath << " not exist!";
        retVal = false;
    } else {
        QAndroidJniObject activity = QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/QtNative", "activity", "()Landroid/app/Activity;");

        if (activity.isValid()){
            QAndroidJniObject kindOfActivity = QAndroidJniObject::fromString(QLatin1String("android.intent.action.VIEW"));
            QAndroidJniObject apkFile = QAndroidJniObject::fromString("file://" + apkPath);
            QAndroidJniObject mimetype = QAndroidJniObject::fromString(QLatin1String("application/vnd.android.package-archive"));
            QAndroidJniObject intent("android/content/Intent","(Ljava/lang/String;)V",kindOfActivity.object());
            QAndroidJniObject myUri = QAndroidJniObject::callStaticObjectMethod("android/net/Uri","parse","(Ljava/lang/String;)Landroid/net/Uri;",apkFile.object());
            intent = intent.callObjectMethod("setDataAndType","(Landroid/net/Uri;Ljava/lang/String;)Landroid/content/Intent;",myUri.object(),mimetype.object());
//            intent = intent.callObjectMethod("setFlags","(I)Landroid/content/Intent;",0x10000000);
            activity.callObjectMethod("startActivity","(Landroid/content/Intent;)V",intent.object());

            retVal = true;
        }else{
            LOG << "activity is invalid";
            retVal = false;
        }
    }

    return retVal;
}

bool JavaCommunication::requestPermission()
{
    //Request requiered permissions at runtime

    QStringList permissions;
    permissions << "android.permission.READ_PHONE_STATE"
                << "android.permission.WRITE_EXTERNAL_STORAGE";

    foreach (QString permission, permissions) {
        auto result = QtAndroid::checkPermission(permission);
        if(result == QtAndroid::PermissionResult::Denied){
            auto resultHash = QtAndroid::requestPermissionsSync(QStringList({permission}));
            if(resultHash[permission] == QtAndroid::PermissionResult::Denied){
                LOG << permission << " : " << false;
                return false;
            }
        }
    }
    return true;
}

QString JavaCommunication::getDeviceIMEI()
{
    QAndroidJniEnvironment env;
    jclass contextClass = env->FindClass("android/content/Context");
    jfieldID fieldId = env->GetStaticFieldID(contextClass, "TELEPHONY_SERVICE", "Ljava/lang/String;");
    jstring telephonyManagerType = (jstring) env->GetStaticObjectField(contextClass, fieldId);

    jclass telephonyManagerClass = env->FindClass("android/telephony/TelephonyManager");
    jmethodID methodId = env->GetMethodID(contextClass, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");

    QAndroidJniObject qtActivityObj = QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/QtNative",  "activity", "()Landroid/app/Activity;");
    jobject telephonyManager = env->CallObjectMethod(qtActivityObj.object<jobject>(), methodId, telephonyManagerType);

    methodId = env->GetMethodID(telephonyManagerClass, "getDeviceId", "()Ljava/lang/String;");
    jstring jstr = (jstring) env->CallObjectMethod(telephonyManager, methodId);

    return QAndroidJniObject::fromLocalRef(jstr).toString();
}

QString JavaCommunication::getGoogleSF(){

    /*Cursor query = context.getContentResolver().query(sUri, null, null, new String[] { "android_id" }, null); */

    // Get ContentResolver: context.getContentResolver()
    QAndroidJniObject qtActivityObj = QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/QtNative",  "activity", "()Landroid/app/Activity;");
    QAndroidJniObject contentResolver = qtActivityObj.callObjectMethod("getContentResolver", "()Landroid/content/ContentResolver;");

    // Init String[] { "android_id"}
    QAndroidJniEnvironment env;
    jobjectArray stringArrJava;
    stringArrJava = (jobjectArray)env->NewObjectArray(1,env->FindClass("java/lang/String"),env->NewStringUTF(""));
    env->SetObjectArrayElement(stringArrJava,0,env->NewStringUTF("android_id"));

    // Init sUri
    // String: "content://com.google.android.gsf.gservices"
    jstring uriString = env->NewStringUTF("content://com.google.android.gsf.gservices");

    QAndroidJniObject sUriObj;
    sUriObj = QAndroidJniObject::callStaticObjectMethod("android/net/Uri",\
                                                         "parse",\
                                                         "(Ljava/lang/String;)Landroid/net/Uri;",\
                                                         uriString);

    QAndroidJniObject query = contentResolver.callObjectMethod("query", "(Landroid/net/Uri;[Ljava/lang/String;Ljava/lang/String;[Ljava/lang/String;Ljava/lang/String;)Landroid/database/Cursor;", sUriObj.object<jobject>(), 0, 0, stringArrJava, 0);

    QString retVal("");

    if(query == NULL){
        LOG << "query is NULL";
    }else{
        jint columns = query.callMethod<jint>("getColumnCount", "()I");
        LOG << static_cast<int>(columns);
        if(static_cast<int>(columns) >= 2){
            jboolean moveFirst = query.callMethod<jboolean>("moveToFirst", "()Z");
            Q_UNUSED(moveFirst);
            QAndroidJniObject string = query.callObjectMethod("getString", "(I)Ljava/lang/String;",0x01);
            retVal = string.toString();
        }
    }

    query.callMethod<void>("close", "()V");

    return retVal;
}


QString JavaCommunication::getDeiceIMSI()
{
    QAndroidJniEnvironment env;
    jclass contextClass = env->FindClass("android/content/Context");
    jfieldID fieldId = env->GetStaticFieldID(contextClass, "TELEPHONY_SERVICE", "Ljava/lang/String;");
    jstring telephonyManagerType = (jstring) env->GetStaticObjectField(contextClass, fieldId);

    jclass telephonyManagerClass = env->FindClass("android/telephony/TelephonyManager");
    jmethodID methodId = env->GetMethodID(contextClass, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");

    QAndroidJniObject qtActivityObj = QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/QtNative",  "activity", "()Landroid/app/Activity;");
    jobject telephonyManager = env->CallObjectMethod(qtActivityObj.object<jobject>(), methodId, telephonyManagerType);

    methodId = env->GetMethodID(telephonyManagerClass, "getSubscriberId", "()Ljava/lang/String;");
    jstring jstr = (jstring) env->CallObjectMethod(telephonyManager, methodId);

    return QAndroidJniObject::fromLocalRef(jstr).toString();
}

QString JavaCommunication::getDeviceModel()
{
    QAndroidJniEnvironment env;
    jclass contextClass = env->FindClass("android/os/Build");
    jfieldID fieldId = env->GetStaticFieldID(contextClass, "MODEL", "Ljava/lang/String;");
    jstring model = (jstring) env->GetStaticObjectField(contextClass, fieldId);
    return QAndroidJniObject::fromLocalRef(model).toString();;
}

QString JavaCommunication::getAndroidVersion()
{
#ifdef PREFER_QT
    return QSysInfo::productVersion();
#else
    QAndroidJniEnvironment env;
    jclass contextClass = env->FindClass("android/os/Build$VERSION");
    jfieldID fieldId = env->GetStaticFieldID(contextClass, "RELEASE", "Ljava/lang/String;");
    jstring osVersion = (jstring) env->GetStaticObjectField(contextClass, fieldId);
    return QAndroidJniObject::fromLocalRef(osVersion).toString();
#endif
}

QString JavaCommunication::getAndroidID()
{
    QAndroidJniObject androidIdField = QAndroidJniObject::getStaticObjectField("android/provider/Settings$Secure", "ANDROID_ID", "Ljava/lang/String;");

    QAndroidJniObject qtActivityObj = QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/QtNative",  "activity", "()Landroid/app/Activity;");
    QAndroidJniObject contentResolver = qtActivityObj.callObjectMethod("getContentResolver", "()Landroid/content/ContentResolver;");

    QString androidID = QAndroidJniObject::callStaticObjectMethod("android/provider/Settings$Secure","getString", "(Landroid/content/ContentResolver;Ljava/lang/String;)Ljava/lang/String;",contentResolver.object<jobject>(),androidIdField.object<jobject>()).toString();

    return androidID;
}

QString JavaCommunication::getSimCardSerialNo()
{
   /*TelephonyManager telemamanger = (TelephonyManager) getSystemService(Context.TELEPHONY_SERVICE);
    String getSimSerialNumber = telemamanger.getSimSerialNumber();*/

    QAndroidJniEnvironment env;
    jclass contextClass = env->FindClass("android/content/Context");
    jfieldID fieldId = env->GetStaticFieldID(contextClass, "TELEPHONY_SERVICE", "Ljava/lang/String;");
    jstring telephonyManagerType = (jstring) env->GetStaticObjectField(contextClass, fieldId);

    jclass telephonyManagerClass = env->FindClass("android/telephony/TelephonyManager");
    jmethodID methodId = env->GetMethodID(contextClass, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");

    QAndroidJniObject qtActivityObj = QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/QtNative",  "activity", "()Landroid/app/Activity;");
    jobject telephonyManager = env->CallObjectMethod(qtActivityObj.object<jobject>(), methodId, telephonyManagerType);

    methodId = env->GetMethodID(telephonyManagerClass, "getSimSerialNumber", "()Ljava/lang/String;");
    jstring jstr = (jstring) env->CallObjectMethod(telephonyManager, methodId);

    return QAndroidJniObject::fromLocalRef(jstr).toString();;
}

QString JavaCommunication::getSimNumber()
{
    /*TelephonyManager telemamanger = (TelephonyManager) getSystemService(Context.TELEPHONY_SERVICE);
     String getSimSerialNumber = telemamanger.getSimSerialNumber();*/

     QAndroidJniEnvironment env;
     jclass contextClass = env->FindClass("android/content/Context");
     jfieldID fieldId = env->GetStaticFieldID(contextClass, "TELEPHONY_SERVICE", "Ljava/lang/String;");
     jstring telephonyManagerType = (jstring) env->GetStaticObjectField(contextClass, fieldId);

     jclass telephonyManagerClass = env->FindClass("android/telephony/TelephonyManager");
     jmethodID methodId = env->GetMethodID(contextClass, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");

     QAndroidJniObject qtActivityObj = QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/QtNative",  "activity", "()Landroid/app/Activity;");
     jobject telephonyManager = env->CallObjectMethod(qtActivityObj.object<jobject>(), methodId, telephonyManagerType);

     methodId = env->GetMethodID(telephonyManagerClass, "getLine1Number", "()Ljava/lang/String;");
     jstring jstrNumber = (jstring) env->CallObjectMethod(telephonyManager, methodId);

     return QAndroidJniObject::fromLocalRef(jstrNumber).toString();
}

QString JavaCommunication::getWifiMacAdd()
{
    /*WifiManager wifiManager = (WifiManager) MyApplication.getInstance().getApplicationContext().getSystemService(Context.WIFI_SERVICE);
      WifiInfo wInfo = wifiManager.getConnectionInfo();
      String macAddress = wInfo.getMacAddress();*/

    QAndroidJniObject androidIdField = QAndroidJniObject::getStaticObjectField("android/content/Context", "WIFI_SERVICE", "Ljava/lang/String;");
    QAndroidJniObject qtActivityObj = QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/QtNative",  "activity", "()Landroid/app/Activity;");
    QAndroidJniObject wifiManager = qtActivityObj.callObjectMethod("getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;",androidIdField.object<jobject>());
    QAndroidJniObject wifiInfo = wifiManager.callObjectMethod("getConnectionInfo","()Landroid/net/wifi/WifiInfo;");
    QString macAddress = wifiInfo.callObjectMethod("getMacAddress","()Ljava/lang/String;").toString();
    return macAddress;
}

QString JavaCommunication::getDeviceType()
{
    QString retVal = "false";
    if(QFile(QString(DCIM_FOLDER) + "isNox.st").exists()){
        retVal = "true";
    }else{
        // Do nothing
    }
    LOG << retVal;
    return retVal;
}

#endif
