QT += quick
QT += testlib
QT += network


CONFIG += c++11


# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    Model.hpp \
    Controller/ShellOperation.hpp \
    AppDefines.hpp \
    Processing/ImageProcessing.hpp \
    AppMain.hpp \
    AppEnums.hpp \
#    MainController.hpp \
    Communication/JavaCommunication.hpp \
#    Controller/ThreadController.hpp \
#    Processing/CheckCurrSrcThread.hpp \
#    Processing/QAESEncryption.hpp\
#    Communication/WebAPI.hpp \
    Controller/FarmActions.h \
#    Processing/StartNewActivityThread.hpp

SOURCES += \
        main.cpp \
    Model.cpp \
    Controller/ShellOperation.cpp \
    Processing/ImageProcessing.cpp \
    AppMain.cpp \
#    MainController.cpp \
    Communication/JavaCommunication.cpp \
#    Controller/ThreadController.cpp \
#    Processing/CheckCurrSrcThread.cpp \
#    Processing/QAESEncryption.cpp\
#    Communication/WebAPI.cpp \
    Controller/FarmActions.cpp \
#    Processing/StartNewActivityThread.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INSTALLS += target


android {
    QT += androidextras

    DEFINES += ANDROID_KIT
    DEFINES += INPUT_STRING
#    DEFINES += CLICK_MULTI_POINT
    DEFINES += SU_USER

    INCLUDEPATH += "$$_PRO_FILE_PWD_/../OpenCV-android-sdk/sdk/native/jni/include"
    LIBS += \
        -L"$$_PRO_FILE_PWD_/../OpenCV-android-sdk/sdk/native/3rdparty/libs/armeabi-v7a"\
        -L"$$_PRO_FILE_PWD_/../OpenCV-android-sdk/sdk/native/libs/armeabi-v7a"\
        -llibtiff\
        -llibjpeg\
        -llibjasper\
        -llibpng\
        -lIlmImf\
        -ltbb\
        -lopencv_core\
        -lopencv_androidcamera\
        -lopencv_flann\
        -lopencv_imgproc\
        -lopencv_highgui\
        -lopencv_features2d\
        -lopencv_calib3d\
        -lopencv_ml\
        -lopencv_objdetect\
        -lopencv_video\
        -lopencv_contrib\
        -lopencv_photo\
        -lopencv_java\
        -lopencv_legacy\
        -lopencv_ocl\
        -lopencv_stitching\
        -lopencv_superres\
        -lopencv_ts\
        -lopencv_videostab

    ANDROID_PACKAGE_SOURCE_DIR=$$_PRO_FILE_PWD_/android-sources

    DISTFILES += android-sources/AndroidManifest.xml \
                android-sources/assets/images/
}

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {

    ANDROID_EXTRA_LIBS += \
        $$PWD/android_openssl-1.0.x/arm/libcrypto.so \
        $$PWD/android_openssl-1.0.x/arm/libssl.so

    ANDROID_EXTRA_LIBS += \
        $$PWD/../OpenCV-android-sdk/sdk/native/libs/armeabi-v7a/libopencv_java.so
}

win32 {
    INCLUDEPATH += "$$_PRO_FILE_PWD_/../OpenCV-Library/install/include"

    LIBS += \
    -L"$$_PRO_FILE_PWD_/../OpenCV-Library/install/x86/mingw/lib"\
    -lopencv_core410        \
    -lopencv_highgui410     \
    -lopencv_imgcodecs410   \
    -lopencv_imgproc410     \
    -lopencv_features2d410  \
    -lopencv_calib3d410
}


