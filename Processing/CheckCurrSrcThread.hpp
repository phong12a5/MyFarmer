#ifndef CHECKCURRSRCTHREAD_H
#define CHECKCURRSRCTHREAD_H

#include <QObject>
#include <AppDefines.hpp>
#include <QTimer>
#include <QMutex>
#include "AppEnums.hpp"
#include "ImageProcessing.hpp"

class CheckCurrSrcThread : public QObject
{
    Q_OBJECT
public:
    explicit CheckCurrSrcThread(QObject *parent = nullptr);
    ~CheckCurrSrcThread();

private:
    bool isOnScreen(QString iconPath);
    bool isCurrentScreen(int screenID);
    int findScreen() const;

private:
    cv::Mat m_screenImg;

private:
    QTimer* m_updateCurrSrcTimer;

signals:
    void screenChanged(int screenID);

public slots:
    void doWork(const QString &parameter);
    void onUpdateCurrentScreen();
};

#endif // CHECKCURRSRCTHREAD_H
