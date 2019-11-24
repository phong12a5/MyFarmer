#ifndef FARMACTIONS_H
#define FARMACTIONS_H

#include <QObject>
#include "AppDefines.hpp"
#include "AppEnums.hpp"

#include <QMutex>

class FarmActions : public QObject
{
    Q_OBJECT
private:
    explicit FarmActions(QObject *parent = nullptr);

public:
    static FarmActions *instance();

    void addFriend(int count = 10);

    void likeNewFeeds(int count = 5);

private:
    static FarmActions *m_instance;
};

#endif // FARMACTIONS_H
