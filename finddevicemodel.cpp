#include "finddevicemodel.h"

FindDeviceModel::FindDeviceModel(QObject *parent) : QObject(parent)
{
     find = new FindDevice;

     connect(find,&FindDevice::addItem,this,&FindDeviceModel::addItemList);
     connect(find,&FindDevice::clearRes,this,&FindDeviceModel::clearRes);
     connect(this,&FindDeviceModel::signal_SendPortmapFind,find,&FindDevice::Work);
}

FindDeviceModel::~FindDeviceModel()
{
   delete find;
}

const QList<QObject *> FindDeviceModel::listDevice() const
{
    return res;
}

void FindDeviceModel::addItemList(QString name, QString peerName)
{
    if(name != "Не найдено")
        res.append(new FindDeviceItem(name,peerName));
    emit ListDeviceChanged(res);

}

void FindDeviceModel::clearRes()
{
    res.clear();
}

//************************************************
//Слот работы по поиску устрйоств
void FindDeviceModel::work()
{
    qDebug () << "Start";

    //сигнал отчисти найденных устройств и объектов
    clearRes();

    emit signal_SendPortmapFind();

}
//************************************************
