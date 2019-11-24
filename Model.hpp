#ifndef MODEL_HPP
#define MODEL_HPP

#include <QString>
#include "AppDefines.hpp"
#include <QStringList>

class Model : public QObject
{
    Q_OBJECT


public:
    static Model* instance();

    QStringList listFbPackage() const;
    void setListFbPackage(QStringList _listFbPackage);
    QString currentFbPackage();

private:
    explicit Model(QObject *parent = nullptr);

private:
    static Model* m_instance;

    QStringList m_listFbPackage;
    int m_curIndexFbPackage;

signals:


public slots:
};

#endif // MODEL_H
