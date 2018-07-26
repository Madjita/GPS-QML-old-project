#ifndef FINDDEVICE_H
#define FINDDEVICE_H

#include <QObject>
#include <QtNetwork>
#include <finddeviceitem.h>

#include <visa.h>
#include <visatype.h>


// Класс поиска устройств в сети ( через Portmap протокол - протокол поиска переадрессованных портов)
class FindDevice: public QObject
{
    Q_OBJECT

    Q_PROPERTY(QList<QObject*> listDevice READ listDevice NOTIFY ListDeviceChanged)
    const QList<QObject*> listDevice() const;


    //Сокет для UDP запроса
    QUdpSocket *udpSocket;

    //Сокет для UDP запроса №2
    QUdpSocket *udpSocket1;

    QString  locIP ;




    //Лист найденных объектов
    public: QList<QObject *> res;

public:
    //Конструктор
   explicit FindDevice(QObject *parent = 0);

    //Инициализация сокетов UDP
    void initSocket();

    //ФУнкция поиска устройств по Portmap протоколу (UDP)
    void SendPortmapFind();


signals:
    void ListDeviceChanged(QList<QObject*>);

    //Отчистить Найденный список
    void clearList();
    //Добавить найденое устрйоство
    void addItem(QString,QString);

public slots:
    void readPendingDatagrams();
    void readPendingDatagrams1();
    void Work();
    void setListDevice(QByteArray data, QString peerName);

    void setListDeviceTP(QByteArray data, QString peerName);

public:
 // QList<FindDeviceItem*> m_FindDeviceItem;

};

#endif // FINDDEVICE_H
