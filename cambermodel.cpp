#include "cambermodel.h"

CamberModel::CamberModel(QObject *parent) : QObject(parent)
{
    camber = new Camber();

    camber->moveToThread(new QThread());

    camber->thread()->start();

    connect(camber->thread(),&QThread::started,camber,&Camber::prosecc);


    connect(this,&CamberModel::signal_startCamberWork,camber,&Camber::startCamberWork,Qt::QueuedConnection);
    connect(this,&CamberModel::signal_stopCamberWork,camber,&Camber::stopCamberWork,Qt::QueuedConnection);
    connect(this,&CamberModel::signal_connectCamber,camber,&Camber::connectCamber,Qt::QueuedConnection);

    connect(camber,&Camber::sendData,this,&CamberModel::sendData);

    connect(this,&CamberModel::signal_setCamberNY,camber,&Camber::setCamberNY,Qt::QueuedConnection);
    connect(this,&CamberModel::signal_setCamberHort,camber,&Camber::setCamberHort,Qt::QueuedConnection);
    connect(this,&CamberModel::signal_setCamberCold,camber,&Camber::setCamberCold,Qt::QueuedConnection);

    //Сигнал для подтверждения соединения
    connect(camber,&Camber::connectOk,this,&CamberModel::slot_connectOk,Qt::QueuedConnection);

}

CamberModel::~CamberModel()
{

}

void CamberModel::slot_startCamberWork()
{
    emit signal_startCamberWork();
}

void CamberModel::slot_stopCamberWork()
{
    emit signal_stopCamberWork();
}

void CamberModel::slot_connectCamber(QString ip)
{
    emit signal_connectCamber(ip);
}

bool CamberModel::slot_TestConnect()
{
    return true;//camber->flagConnect;
}

void CamberModel::slot_StartTimer()
{
    emit signal_startTimer();
}

void CamberModel::slot_SetNY()
{
        emit signal_setCamberNY();
}

void CamberModel::slot_SetHord()
{
    emit signal_setCamberHort();
}

void CamberModel::slot_SetCold()
{
    emit signal_setCamberCold();
}

bool CamberModel::slot_connectOk()
{
    emit signal_Ok(slot_TestConnect());

    return slot_TestConnect();
}
