#include "Model.hpp"

Model* Model::m_instance = nullptr;

Model::Model(QObject *parent) : QObject(parent)
{
    m_curIndexFbPackage = -1;
}

Model *Model::instance()
{
    if(m_instance == nullptr){
        m_instance = new Model();
    }
    return m_instance;
}

QStringList Model::listFbPackage() const
{
    return m_listFbPackage;
}

void Model::setListFbPackage(QStringList _listFbPackage)
{
    if(m_listFbPackage != _listFbPackage){
        m_listFbPackage = _listFbPackage;
        LOG << m_listFbPackage;
    }
}

QString Model::currentFbPackage()
{
    if(m_listFbPackage.isEmpty()){
        return QString();
    }else {
        if(m_curIndexFbPackage < 0 || m_curIndexFbPackage >= m_listFbPackage.length()){
            m_curIndexFbPackage = 0;
        }
        return m_listFbPackage.at(m_curIndexFbPackage);
    }
}


