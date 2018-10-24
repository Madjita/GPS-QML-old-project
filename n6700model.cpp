#include "n6700model.h"

N6700Model::N6700Model(QObject *parent) : QObject(parent),
    item1(new ItemIstochnik),
    item2(new ItemIstochnik),
    item3(new ItemIstochnik),
    item4(new ItemIstochnik)
{

    n6700 = new N6700();

    //Соединение
    connect(this,&N6700Model::connectDevice,n6700,&N6700::Connect);
    //Разъединение
    connect(this,&N6700Model::DisConnect,n6700,&N6700::DisConnect);
    //Сигнал для подтверждения соединения
    connect(n6700,&N6700::connectOk,this,&N6700Model::slot_connectOk);


   // connect(this,&N6700Model::endWork,n6700,&N6700::endWork);

   // connect(this,&N6700Model::Work,n6700,&N6700::Work);

    //Сигнал для запуска таймера
    connect(this,&N6700Model::startTimer,n6700,&N6700::slot_StartTimer);
    //Сигнал для отключения таймера
    connect(this,&N6700Model::stopTimer,n6700,&N6700::endWork);


    //Обновление данных от n6700
    //изменил
    //connect(n6700,&N6700::getMeasureCURRentSignal,this,&N6700Model::slot_getMeasureCURRentSignal);
   // connect(n6700,&N6700::getMeasureVoltSignal,this,&N6700Model::slot_getMeasureVoltSignal);


    connect(this,&N6700Model::setOut,n6700,&N6700::setOutput);
    connect(this,&N6700Model::setVolt,n6700,&N6700::setVolt);

    //Додумать насчет пересылки сгенерированных спутников (реализованно на подсчет)
    // Дописать сигнал для отображения мощности по каждому сгенирированному спутнику


   // qRegisterMetaType<QString,bool>();

}

N6700Model::~N6700Model()
{
    delete item1;
    delete item2;
    delete item3;
    delete item4;

    delete n6700;
}

const QString &N6700Model::name() const
{
    return n6700->name;
}


void N6700Model::slot_connectDevice(QString ip)
{
    emit connectDevice(ip);
}

bool N6700Model::slot_connectOk()
{
    emit signal_Ok(n6700->connected);

    return n6700->connected;
}

void N6700Model::slot_DisConnect()
{
    //qDebug() << "dsafdasfasdf slot_DisConnect";
    emit DisConnect();

}

void N6700Model::slot_Work()
{
    emit Work();
}


void N6700Model::slot_StartTimer()
{
    emit startTimer();
}

void N6700Model::slot_getMeasureCURRentSignal(QVector<QString> list)
{
     item1->tok = list[0].split("\n").first();
     item2->tok = list[1].split("\n").first();
     item3->tok = list[2].split("\n").first();
     item4->tok = list[3].split("\n").first();

   //  qDebug() <<" N6700Model tok = " <<  item1->tok;

     emit getMeasureCURRentSignal(list);
}

void N6700Model::slot_getMeasureVoltSignal(QVector<QString> list)
{
    item1->v = list[0].split("\n").first();
    item2->v = list[1].split("\n").first();
    item3->v = list[2].split("\n").first();
    item4->v = list[3].split("\n").first();

    item1->p = QString::number(item1->tok.toDouble()*item1->v.toDouble());
    item2->p = QString::number(item2->tok.toDouble()*item2->v.toDouble());
    item3->p = QString::number(item3->tok.toDouble()*item3->v.toDouble());
    item4->p = QString::number(item4->tok.toDouble()*item4->v.toDouble());


    emit getMeasureVoltSignal(list);
}

void N6700Model::slot_StopTimer()
{
    emit stopTimer();
}

bool N6700Model::getConnect()
{
    return n6700->connected;
}
