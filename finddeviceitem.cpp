#include "finddeviceitem.h"

//************************************************
//Конструктор
FindDeviceItem::FindDeviceItem(QString content,QString peerName, QObject *parent):
    QObject(parent),
    m_content(content),
    m_ip(peerName),
    m_connect(false)
{

}
//************************************************

//************************************************
const QString &FindDeviceItem::content() const{
    return m_content;
}
//************************************************

//************************************************
void FindDeviceItem::setContent(const QString &content){
    if(content != m_content){
        m_content = content;
        emit contentChanged();
    }
}
//************************************************

//************************************************
const QString &FindDeviceItem::ip() const{
    return m_ip;
}
//************************************************

//************************************************
void FindDeviceItem::setIp(const QString &ip){
    if(ip != m_ip){
        m_ip = ip;
        emit ipChanged();
    }
}
//************************************************
