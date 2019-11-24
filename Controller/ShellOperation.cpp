#include "ShellOperation.hpp"
#include "Model.hpp"

#define MODEL           Model::instance()

ShellOperation::ShellOperation(QObject *parent) : QObject(parent)
{

}

#ifdef ANDROID_KIT
bool ShellOperation::shellCommand(QString cmd)
{
       int retVal = QProcess::execute("su", QStringList() << "-c" << cmd);
       return retVal >= 0;
}

bool ShellOperation::shellCommand(QString cmd, QString &output)
{
    QProcess process;
    process.start("su", QStringList() << "-c" << cmd);
    process.waitForFinished(-1);
    output = process.readAllStandardOutput();
    return process.exitCode() == 0;
}



bool ShellOperation::installPackage(QString packagePath)
{
    LOG << packagePath;
    bool retVal = false;
    if(!QFile(packagePath).exists()){
        LOG << "packagePath not existed";
        retVal = false;
    }else{
        retVal = ShellOperation::shellCommand(QString("pm install %1").arg(packagePath));
    }
    LOG << packagePath << " --- retVal: " << retVal;
    return retVal;
}

void ShellOperation::callScrollEvent(QPoint point1, QPoint point2)
{
    LOG << QString("Scrolling from [%1,%2] to [%3,%4]")\
           .arg(point1.x()).arg(point1.y())\
           .arg(point2.x()).arg(point2.y());

    ShellOperation::shellCommand(QString("input swipe %1 %2 %3 %4 1000").arg(QString::number(point1.x())).\
                                 arg(QString::number(point1.y())).\
                                 arg(QString::number(point2.x())).\
                                 arg(QString::number(point2.y())));
}

QString ShellOperation::getCurrentActivity()
{
    QString retVal = "";
    QString output;
    ShellOperation::shellCommand(QString("dumpsys window windows | grep -E 'mCurrentFocus'"),output);
    QStringList outputList = output.split(' ');
    if(outputList.length() > 4){
        retVal = QString(outputList.at(4)).simplified().remove('}');
    }
    return retVal;
}

bool ShellOperation::findAndClick(QString iconPath, bool repeat)
{
    LOG << iconPath;
    QString screenImgPath = ShellOperation::screenShot();
    QPoint point = ImageProcessing::findImageOnImage(iconPath,screenImgPath);
    if(point.isNull()){
        screenImgPath = ShellOperation::screenShot();
        point = ImageProcessing::findImageOnImage(iconPath,screenImgPath);
        if(point.isNull()){
            return false;
        }
    }
    return ShellOperation::tapScreen(point, repeat);
}

int ShellOperation::findAndClickList(QString iconPath)
{
    QString screenImgPath = ShellOperation::screenShot();
    QList<QPoint> pointList = ImageProcessing::findImageListOnImage(iconPath,screenImgPath);
    if(!pointList.isEmpty()){
        foreach (QPoint point, pointList) {
            ShellOperation::tapScreen(point);
            delay(1000);
        }
        return pointList.length();
    }else{
        LOG << "There is no the matching image";
        return 0;
    }
}

bool ShellOperation::tapScreen(QPoint point, bool repeat)
{
    LOG << "Tapping at [" << point.x() << "," << point.y() << "]";
    bool retVal = false;
    retVal = ShellOperation::shellCommand(QString("input tap %1 %2").arg(point.x()).arg(point.y()));
    if(repeat){
        delay(200);
        retVal &= ShellOperation::shellCommand(QString("input tap %1 %2").arg(point.x()).arg(point.y()));
    }
    return retVal;
}

bool ShellOperation::enterText(QString text)
{
    LOG << "Entering text: " << text;
    return ShellOperation::shellCommand(QString("input text %1").arg(text));
}

void ShellOperation::killSpecificApp(QString packageName)
{
    LOG << "Killing " << packageName;
    ShellOperation::shellCommand(QString("am force-stop %1").arg(packageName));
}

QPoint ShellOperation::findAnImageOnScreen(QString iconPath)
{
    QString screenImgPath = ShellOperation::screenShot();
    QPoint point = ImageProcessing::findImageOnImage(iconPath,screenImgPath);
    return  point;
}

void ShellOperation::clearPackageData(QString packageName)
{
    LOG << packageName;
    ShellOperation::shellCommand(QString("pm clear %1").arg(packageName));
//    ShellOperation::stopApp(packageName);
}

void ShellOperation::stopApp(QString packageName)
{
    ShellOperation::shellCommand(QString("am force-stop %1").arg(packageName));
}

bool ShellOperation::pressTap()
{
    LOG << "Pressing Tap Key ...";
    return ShellOperation::shellCommand("input keyevent KEYCODE_TAB");
}

DISPLAY_INFO ShellOperation::getDisplayInfo()
{
    DISPLAY_INFO info;

    QString sizeInfo;
    ShellOperation::shellCommand("wm size",sizeInfo);

    QString dpiInfo;
    ShellOperation::shellCommand("wm density",dpiInfo);

    if(sizeInfo.contains("Physical size")){
        info.width = (sizeInfo.split(":").length() < 2? info.width : sizeInfo.split(":").at(1).simplified().split("x").at(0).simplified().toInt());
        info.height = (sizeInfo.split(":").length() < 2? info.height : sizeInfo.split(":").at(1).simplified().split("x").at(1).simplified().toInt());
    }

    if(dpiInfo.contains("Physical density")){
        info.dpi = (dpiInfo.split(":").length() < 2? info.dpi : dpiInfo.split(":").at(1).simplified().toInt());
    }
    return info;
}

void ShellOperation::removeFile(QString path)
{
    ShellOperation::shellCommand(QString("rm %1").arg(path));
}

void ShellOperation::enterKeyBoard()
{
    LOG << "Pressing Tap Key ...";
    ShellOperation::shellCommand("input keyevent KEYCODE_ENTER");
}

bool ShellOperation::isPackageExisted(QString packageName)
{
    QString output;
    if(ShellOperation::shellCommand(QString("pm list packages | grep %1").arg(packageName),output)){
        LOG << output;
        if(output.contains(packageName)){
            return true;
        }else{
            return false;
        }
    }else{
        return false;
    }
}

QStringList ShellOperation::installedFBPkg()
{
    QStringList retVal;
    QString output;
    if(ShellOperation::shellCommand("pm list packages | grep com.facebook", output)){
        retVal = output.remove("package:").split("\n");
    }
    if(retVal.contains(""))
        retVal.removeOne("");
    return retVal;
}

QString ShellOperation::screenShot(QString fileName)
{
    QString path = QString("/sdcard/Pictures/%1").arg(fileName);
    ShellOperation::shellCommand(QString("screencap -p %1").arg(path));
    return path;
}
#endif
