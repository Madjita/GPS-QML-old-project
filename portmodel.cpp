#include "portmodel.h"

PortModel::PortModel(QObject *parent) : QObject(parent)
{

    qDebug () << "PortModel* PortNew : create (new)";
    PortNew = new Port();
    qDebug () << "PortModel* PortNew = " << sizeof(PortNew);

  //  QObject::connect(PortNew,SIGNAL(outMRKdata(frameExample)), this,SLOT(Update_MRK_Ttable(frameExample))); //По нажатию кнопки подключить порт


    qRegisterMetaType<frameExample>();

    //Сигнал на подключение порта
    QObject::connect( this,&PortModel::open, PortNew,&Port::ConnectPort,Qt::QueuedConnection);
    //Сигнал от порта к QML , что он открыт или ошибка
    QObject::connect( PortNew,&Port::outPortOpen, this,PortModel::openOK);


    m_listCOM.clear();

    foreach (const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        m_listCOM << info.portName(); // Находит доступные порты

    }

    qDebug () <<"Доступные порты: " <<  m_listCOM.toVector();

    emit listCOMChanged(m_listCOM);

    QObject::connect(this,&PortModel::Work,PortNew,&Port::Work);
    QObject::connect(this,&PortModel::endWork,PortNew,&Port::END);

    QObject::connect(this,&PortModel::Write_Setting_Port,PortNew,&Port::Write_Setting_Port);

    QObject::connect(this,&PortModel::DisconnectPort,PortNew,&Port::DisconnectPort,Qt::QueuedConnection);

    QObject::connect(PortNew,&Port::UpdateCountFind,this,&PortModel::UpdateCountFind,Qt::QueuedConnection);


    QObject::connect(PortNew,&Port::signal_GoTORelizproverka,this,&PortModel::setListSpytnik,Qt::QueuedConnection);


}

const QStringList PortModel::listCOM() const
{
    return   m_listCOM;
}

const QString PortModel::nameOpenPort() const
{
    return name;
}

void PortModel::openPort(QString name)
{
   emit Write_Setting_Port(name,115200,8,0,1,0);
   // PortNew->Write_Setting_Port(name, 115200,8,0,1,0);

    qDebug() << " PortNew->Write_Setting_Port = " << name;

    emit open();
}

void PortModel::openOK(QString ok)
{

    if("Device is not open" == ok || "Неизвестная ошибка" == ok)
    {
        emit signal_Ok(false);
    }
    else
    {
        emit signal_Ok(true);
    }


    name = ok;

    qDebug() << "Open port name = " << name;
}

void PortModel::closePort()
{
    emit  DisconnectPort();
}

void PortModel::slot_Work()
{
   qDebug() << "LOL ";
   emit Work();
}

void PortModel::slot_endWork()
{
    emit endWork();
}


void PortModel::setListSpytnik(QStringList listSP,QStringList listSP_Amplitude,QStringList listSP_Name)
{



    GetlistSP = listSP;

    GetlistSP_Amplitude = listSP_Amplitude;

    GetlistSP_Name = listSP_Name;

    emit signal_setListSpytnik(GetlistSP,GetlistSP_Amplitude,GetlistSP_Name);

    GetlistSP.clear();
    GetlistSP_Amplitude.clear();
    GetlistSP_Name.clear();


}

void PortModel::UpdateCountFind(int CountFindGLONASSGet,int CountFindGPSGet,  int CountFindALLGet)
{

    CountFindGPS = CountFindGPSGet;
    CountFindGLONASS = CountFindGLONASSGet;

    CountFindALL = CountFindALLGet;

    emit UpdateCountFindQML(CountFindGLONASS,CountFindGPS,CountFindALL);
//    qDebug() <<"CountFindGLONASS = " << CountFindGLONASS;
//  qDebug() <<"CountFindGPS = " <<  CountFindGPS;
//     qDebug() <<"CountFindALL = " <<  CountFindALL;

}
