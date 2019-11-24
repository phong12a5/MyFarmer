#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <AppDefines.hpp>
#include "Model.hpp"
#include "Controller/ShellOperation.hpp"
#include "Processing/ImageProcessing.hpp"
#include "AppMain.hpp"

int main(int argc, char *argv[])
{
    LOG << "STARTING .... ";

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    AppMain appMain;
    appMain.initApplication(engine);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty()){
        LOG << "rootObject is NULL";
    }

    return app.exec();
}

