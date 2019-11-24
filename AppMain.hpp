#ifndef APPMAIN_H
#define APPMAIN_H

#include <QObject>
#include <QTimer>
#include "AppDefines.hpp"
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include "Model.hpp"
#include "AppEnums.hpp"

class AppMain : public QObject
{
    Q_OBJECT
public:
    explicit AppMain(QObject *parent = nullptr);
    void initApplication(QQmlApplicationEngine &engine);

private:
    void connectSignalSlot() const;
    void coppyFolder(QString src, QString dst);

public slots:
    void onStartProgram();

private:
    QTimer m_startAppTimer;
};

#endif // APPMAIN_H
