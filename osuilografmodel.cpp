#include "osuilografmodel.h"

OsuilografModel::OsuilografModel(QObject *parent) : QObject(parent),
  R(0),
  G(0)
{

  qDebug () << "OsuilografModel* os : create (new)";
  os = new Osuilograf();
  qDebug () << "OsuilografModel* os = " << sizeof(os);


  //Соединение
  QObject::connect(this,&OsuilografModel::connectDevice,os,&Osuilograf::connectDevice,Qt::QueuedConnection);
  //Разъединение
  QObject::connect(this,&OsuilografModel::DisConnect,os,&Osuilograf::DisConnect,Qt::QueuedConnection);
  //Сигнал для подтверждения соединения
  QObject::connect(os,&Osuilograf::connectOk,this,&OsuilografModel::slot_connectOk,Qt::QueuedConnection);





  //Сигнал для запуска таймера
  QObject::connect(this,&OsuilografModel::startTimer,os,&Osuilograf::slot_StartTimer,Qt::QueuedConnection);
  //Сигнал для отключения таймера
  QObject::connect(this,&OsuilografModel::stopTimer,os,&Osuilograf::endWork,Qt::QueuedConnection);


  //Обновление данных от os
 QObject::connect(os,&Osuilograf::UpdateScenNumberIDSpytnik,this,&OsuilografModel::slot_ScenNumberIDSpytnik);

 //Сигнал для установки режим "START"
 QObject::connect(this,&OsuilografModel::signal_getCONTrol,os,&Osuilograf::setCONTrol);

 //Сигнал от os об установки режима "START"
 QObject::connect(os,&Osuilograf::signal_QMLStart,this,&OsuilografModel::getCONTrol);


 //Сигнал для установки режима на сигнал генераторе "START"
 QObject::connect(this,&OsuilografModel::signal_Gen_setCONTrol,os,&Osuilograf::setGenCONTrol);


 //Сигнал для установки литеры спутника на сигнал генераторе
 QObject::connect(this,&OsuilografModel::signal_Gen_setSATid,os,&Osuilograf::setGenSATid);



 //Сигнал для смены настроек
 QObject::connect(this,&OsuilografModel::signal_Change_Seitings,os,&Osuilograf::setStartSeitings2);

 //Сигнал для смены настроек на каналы
 QObject::connect(this,&OsuilografModel::signal_Change_Seitings3,os,&Osuilograf::setStartSeitings3);



  //Додумать насчет пересылки сгенерированных спутников (реализованно на подсчет)
  // Дописать сигнал для отображения мощности по каждому сгенирированному спутнику




}

const QString &OsuilografModel::name() const
{
  return os->name;
}


void OsuilografModel::slot_ScenNumberIDSpytnik(QString data)
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


QString OsuilografModel::slot_R()
{
  return QString::number(R);
}

QString OsuilografModel::slot_G()
{
  return QString::number(G);
}


void OsuilografModel::slot_connectDevice(QString ip)
{
  emit connectDevice(ip);
}

bool OsuilografModel::slot_connectOk()
{
  emit signal_Ok(os->connected);

  return os->connected;
}

void OsuilografModel::slot_DisConnect()
{
  qDebug() << "dsafdasfasdf slot_DisConnect";
  emit endWork();
  emit DisConnect();

}

void OsuilografModel::slot_Work()
{
  emit Work();
}


void OsuilografModel::slot_StartTimer()
{
  emit startTimer();
}

void OsuilografModel::slot_StopTimer()
{
  emit stopTimer();
}

void OsuilografModel::setSIGNALtype(QString SIGNALtype)
{
  emit signal_getCONTrol(SIGNALtype);
}

void OsuilografModel::getCONTrol()
{
  emit signal_getCONTrolQML();
}

void OsuilografModel::slot_Gen_setCONTrol(QString SIGNALtype)
{
  emit signal_Gen_setCONTrol(SIGNALtype);
}

void OsuilografModel::slot_Gen_setSATid(QString SIGNALtype)
{
    emit signal_Gen_setSATid(SIGNALtype);
}

void OsuilografModel::slot_Change_Seitings()
{
    emit signal_Change_Seitings();
}

void OsuilografModel::slot_Change_Seitings3()
{
    emit signal_Change_Seitings3();
}

