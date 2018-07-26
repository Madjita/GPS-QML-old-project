#include "predstavleniefinddevice.h"

PredstavlenieFindDevice::PredstavlenieFindDevice():
    m_FindDeviceItem(QList<QObject*>())
{

    FindDevice* mFindDevice = new FindDevice();

    mFindDevice->moveToThread(new QThread());


    QObject::connect(mFindDevice,SIGNAL(clearList()),this,SLOT(clearList()));
    QObject::connect(mFindDevice,SIGNAL(addItem(QString,QString)),this,SLOT(addList(QString,QString)));


    QObject::connect(mFindDevice->thread(),SIGNAL(started()),mFindDevice,SLOT(Work()));
    QObject::connect(this,SIGNAL(findDevice()),mFindDevice,SLOT(Work()));



    mFindDevice->thread()->start();
}


const QList<QObject*> PredstavlenieFindDevice::listDevice() const
{

    return m_FindDeviceItem;
}



void PredstavlenieFindDevice::GetListDeviceChanged(QList<QObject*>* list)
{

    for(int i=0;i<list->count();i++)
    {
        m_FindDeviceItem.append(list->value(i));
    }

}

void PredstavlenieFindDevice::clearList()
{
    res.clear();
    m_FindDeviceItem.clear();
}

void PredstavlenieFindDevice::addList(QString name,QString ip)
{
    m_FindDeviceItem.append(new FindDeviceItem(name,ip));

    res.append(m_FindDeviceItem.last());

    ListDeviceChanged(res);

}

void PredstavlenieFindDevice::Work()
{
    emit findDevice();
}
