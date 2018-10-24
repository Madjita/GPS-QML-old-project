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




    //Лист найденных объектов
    public:
    QList<QObject *> res;
    //Сокет для UDP запроса
    QUdpSocket *udpSocket;

    //Сокет для UDP запроса №2
    QUdpSocket *udpSocket1;

    QString  locIP ;


public:
    //Конструктор
   explicit FindDevice(QObject *parent = nullptr);
    ~FindDevice();





signals:
    void ListDeviceChanged(QList<QObject*>);

    //Отчистить Найденный список
    void clearList();
    //Добавить найденое устрйоство
    void addItem(QString,QString);

    void signal_SendPortmapFind();
    void clearRes();

    void signal_finished();

public slots:
    void readPendingDatagrams();
    void readPendingDatagrams1();
    void Work();
    void setListDevice(QByteArray data, QString peerName);

    void setListDeviceTP(QByteArray data, QString peerName);

    //ФУнкция поиска устройств по Portmap протоколу (UDP)
    void SendPortmapFind();

    //Инициализация сокетов UDP
    void initSocket();


public:
 // QList<FindDeviceItem*> m_FindDeviceItem;

};

#endif // FINDDEVICE_H
