#ifndef STARTNEWACTIVITYTHREAD_H
#define STARTNEWACTIVITYTHREAD_H

#include <QObject>
#include <AppDefines.hpp>

class StartNewActivityThread : public QObject
{
    Q_OBJECT
public:
    explicit StartNewActivityThread(QObject *parent = nullptr);
    ~StartNewActivityThread();


private:
    QTimer* m_updateCurrActTimer;

public slots:
    void doWork(QString packageName, QString sxtraData);
};

#endif // CHECKCURRACTIVITYTHREAD_H
