#include "gsgmodel.h"


gsgModel::gsgModel(QObject *parent) : QObject(parent),
    R(0),
    G(0)
{

    qDebug () << "GSG* gsg : create (new)" << sizeof(gsg);
    gsg = new GSG();
    qDebug () << "GSG* gsg = " << sizeof(gsg);


    //Соединение
    QObject::connect(this,&gsgModel::connectDevice,gsg,&GSG::connectDevice,Qt::QueuedConnection);
    //Разъединение
    QObject::connect(this,&gsgModel::DisConnect,gsg,&GSG::DisConnect,Qt::QueuedConnection);
    //Сигнал для подтверждения соединения
    QObject::connect(gsg,&GSG::connectOk,this,&gsgModel::slot_connectOk,Qt::QueuedConnection);


    //Сигнал для запуска таймера
    QObject::connect(this,&gsgModel::startTimer,gsg,&GSG::slot_StartTimer,Qt::QueuedConnection);
    //Сигнал для отключения таймера
    QObject::connect(this,&gsgModel::stopTimer,gsg,&GSG::endWork,Qt::QueuedConnection);


    //Обновление данных от gsg
    QObject::connect(gsg,&GSG::UpdateScenNumberIDSpytnik,this,&gsgModel::slot_ScenNumberIDSpytnik);

    //Сигнал для установки режим "START"
    QObject::connect(this,&gsgModel::signal_getCONTrol,gsg,&GSG::setCONTrol);

    //Сигнал от gsg об установки режима "START"
    QObject::connect(gsg,&GSG::signal_QMLStart,this,&gsgModel::getCONTrol,Qt::DirectConnection);


    //Сигнал для установки режима на сигнал генераторе "START"
    QObject::connect(this,&gsgModel::signal_Gen_setCONTrol,gsg,&GSG::setGenCONTrol);


    //Сигнал для установки литеры спутника на сигнал генераторе
    QObject::connect(this,&gsgModel::signal_Gen_setSATid,gsg,&GSG::setGenSATid);








    //Додумать насчет пересылки сгенерированных спутников (реализованно на подсчет)
    // Дописать сигнал для отображения мощности по каждому сгенирированному спутнику




}

const QString &gsgModel::name() const
{
    return  gsg->name;
}

void gsgModel::SetBD(BData *bd)
{

    BD = bd;

}

void gsgModel::Sed_TP_OS(OsuilografModel * getOS, TP8 * getTP)
{

    os = getOS;

    tp = getTP;

}


void gsgModel::slot_ScenNumberIDSpytnik(QString data)
{
    R = 0;
    G = 0;



    for(int i=0;i<data.split(',').count();i++)
    {

        if(data.split(',').value(i)[0] == 'R')
        {
            R++;
        }
        if(data.split(',').value(i)[0] == 'G')
        {
            G++;
        }

    }

    qDebug() <<"R = " << R <<"G = "<< G;
    emit countSpytnik_R(R);
    emit countSpytnik_G(G);
}


QString gsgModel::slot_R()
{
    return QString::number(R);
}

QString gsgModel::slot_G()
{
    return QString::number(G);
}


void gsgModel::slot_connectDevice(QString ip)
{
    emit connectDevice(ip);
}

bool gsgModel::slot_connectOk()
{
    emit signal_Ok(gsg->connected);

    return gsg->connected;
}

void gsgModel::slot_DisConnect()
{
    qDebug() << "dsafdasfasdf slot_DisConnect";
    emit endWork();
    emit DisConnect();

}

void gsgModel::slot_Work()
{
    emit Work();
}


void gsgModel::slot_StartTimer()
{
    emit startTimer();
}

void gsgModel::slot_StopTimer()
{
    emit stopTimer();
}

void gsgModel::setSIGNALtype(QString SIGNALtype)
{
    emit signal_getCONTrol(SIGNALtype);
}

void gsgModel::getCONTrol()
{
    emit signal_getCONTrolQML();
}

void gsgModel::slot_Gen_setCONTrol(QString SIGNALtype)
{
    emit signal_Gen_setCONTrol(SIGNALtype);
}

void gsgModel::slot_Gen_setSATid(QString SIGNALtype)
{
    emit signal_Gen_setSATid(SIGNALtype);
}

