#ifndef SHELLOPERATION_H
#define SHELLOPERATION_H

#include <QObject>
#include <QDir>
#include <QFile>
#include <QProcess>
#include <QImage>
#include "AppDefines.hpp"
#include <QStandardPaths>
#include "Processing/ImageProcessing.hpp"

class ShellOperation : public QObject
{
    Q_OBJECT
private:
    explicit ShellOperation(QObject *parent = nullptr);

public:
#ifdef ANDROID_KIT
    static bool shellCommand(QString cmd);
    static bool shellCommand(QString cmd, QString& output);
    static bool installPackage(QString packagePath);
    static QString screenShot(QString fileName = "screen.png");
    static void callScrollEvent(QPoint point1, QPoint point2);
    static QString getCurrentActivity();
    static bool findAndClick(QString iconPath, bool repeat = false);
    static int findAndClickList(QString iconPath);
    static bool tapScreen(QPoint point, bool repeat = false);
    static bool enterText(QString text);
    static void killSpecificApp(QString packageName);
    static QPoint findAnImageOnScreen(QString iconPath);
    static void clearPackageData(QString packageName);
    static void stopApp(QString packageName);
    static bool pressTap();
    static DISPLAY_INFO getDisplayInfo();
    static void removeFile(QString path);
    static void enterKeyBoard();
    static bool isPackageExisted(QString packageName);
    static QStringList installedFBPkg();
#endif
private:
};

#endif // SHELLOPERATION_H
