#include "port.h"
#include <QDebug.h>

#include <QThread>

Port::Port(QObject *parent) :
    QObject(parent),
    flag_work(false),
    flag_start_MRK(false),
    thisPort(new QSerialPort()),
    CountFindALL(0),
    position_liters(0),
    position_number(1),
    position_Mode(2),
    position_A(18)
{

   // this->moveToThread(new QThread()); //помещаем класс в поток
    qDebug () << "Помещаем класс << Port >> в поток : " << this->thread();

    QObject::connect(this->thread(),&QThread::started,this,&Port::process_start);

  //  this->thread()->start();
    qDebug () << "Запускаем поток << Port >> : " << this->thread();

    process_start();

}

Port::~Port()
{
    qDebug("By in Thread!");
    emit finihed_Port(); // Сигнал о завершении работы
}

void Port::process_start()
{
    qDebug () << "QSerialPort* thisPort : create (new)";
    thisPort = new QSerialPort();
   qDebug () << "QSerialPort* thisPort = " << sizeof(thisPort);
//    thisPort->moveToThread(new QThread()); //помещаем сам порт в поток
//    qDebug () << "Помещаем класс << QSerialPort >> в поток : " << thisPort->thread();


    qDebug("Open the port.cpp on the new Thread");
    QObject::connect(thisPort,SIGNAL(error(QSerialPort::SerialPortError)),this,SLOT(handleError(QSerialPort::SerialPortError))); // подключаем првоерку ошибок порта
    QObject::connect(thisPort, SIGNAL(readyRead()),this,SLOT(ReadInProt()),Qt::QueuedConnection); //подключаем чтение с порта по сигналу readyRead()


    listSP = new QStringList();

    listSP_Amplitude = new QStringList();
    listSP_Name = new QStringList();

    container = new frameExample();


    QObject::connect(  thisPort->thread(),SIGNAL(started()), this,SLOT(process_Port())); // Переназначения метода run
    QObject::connect(  this, SIGNAL(finihed_Port()),  thisPort->thread(),SLOT(quit())); // Переназначение метода выход
    QObject::connect(  thisPort->thread(),SIGNAL(finished()), this,SLOT(deleteLater())); // Удалить к чертям поток
    QObject::connect(  this,SIGNAL(finihed_Port()),  thisPort->thread(),SLOT(deleteLater())); // Удалить к чертям поток

   // thisPort->thread()->start();
    qDebug () << "Запускаем поток << QSerialPort >> : " << thisPort->thread();

    //Инициализация таймеров
    timer_MRK_Data = new QTimer();

    QObject::connect(timer_MRK_Data,&QTimer::timeout,this,&Port::GetMrk);
    QObject::connect(this,SIGNAL(startTimerMrk(int)),timer_MRK_Data,SLOT(start(int)));
    QObject::connect(this,&Port::stopTimerMrk,timer_MRK_Data,&QTimer::stop);


    connect(this,SIGNAL(writeData(QByteArray)),this,SLOT(WriteToPort(QByteArray))); // отправить данные



    qRegisterMetaType<QSerialPort::SerialPortError>();
    qRegisterMetaType<QVector<int>>();
}


void Port::process_Port() //Выполняется при старте класса
{

    qDebug("Open the port.cpp on the new Thread");
    QObject::connect(thisPort,SIGNAL(error(QSerialPort::SerialPortError)),this,SLOT(handleError(QSerialPort::SerialPortError))); // подключаем првоерку ошибок порта
    QObject::connect(thisPort, SIGNAL(readyRead()),this,SLOT(ReadInProt()),Qt::DirectConnection); //подключаем чтение с порта по сигналу readyRead()


    listSP = new QStringList();

    listSP_Amplitude = new QStringList();
    listSP_Name = new QStringList();

    container = new frameExample();

}

void Port::Write_Setting_Port(QString name, int baudrate, int DataBits, int parity, int StopBits, int FlowControl) //заносим параметры порта в структуру данных
{
    SettingsPort.name = name;

    SettingsPort.baudRate = (QSerialPort::BaudRate) baudrate;
    SettingsPort.dataBits = (QSerialPort::DataBits) DataBits;
    SettingsPort.parity = (QSerialPort::Parity) parity;
    SettingsPort.stopBits = (QSerialPort::StopBits) StopBits;
    SettingsPort.flowControl = (QSerialPort::FlowControl) FlowControl;

    //        SettingsPort.baudRate = (QSerialPort::BaudRate) baudrate;
    //        SettingsPort.dataBits = QSerialPort::Data8;
    //        SettingsPort.parity = QSerialPort::NoParity;
    //        SettingsPort.stopBits = QSerialPort::OneStop;
    //        SettingsPort.flowControl = QSerialPort::NoFlowControl;
}


void Port::ConnectPort(void) //Процедура подключения
{
     qDebug() << "OPEN the PORT ";
    thisPort->setPortName(SettingsPort.name);



    if(thisPort->open(QIODevice::ReadWrite))
    {
        if(thisPort->setBaudRate(SettingsPort.baudRate) && thisPort->setDataBits(SettingsPort.dataBits)&& thisPort->setParity(SettingsPort.parity)&& thisPort->setStopBits(SettingsPort.stopBits)&& thisPort->setFlowControl(SettingsPort.flowControl))
        {
            if(thisPort->isOpen())
            {
                qDebug() << SettingsPort.name + " >> Открыт!\r";
                //error_((SettingsPort.name + " >> Открыт!\r").toLocal8Bit());

                flag_work = true;


                emit outPortOpen(SettingsPort.name);
            }
        }
        else
        {

            flag_work = false;
            flag_start_MRK = false;

            emit outPortOpen(thisPort->errorString());


            thisPort->close();
            // error_(thisPort->errorString().toLocal8Bit());
        }
    }else
    {
        thisPort->close();
        qDebug () << thisPort->errorString();
        emit outPortOpen(thisPort->errorString());
        // error_(thisPort->errorString().toLocal8Bit());
    }
}

void Port::handleError(QSerialPort::SerialPortError error) //Проверка ошибок при работе
{
    if((thisPort->isOpen()) && (error == QSerialPort::ResourceError))
    {
        error_(thisPort->errorString().toLocal8Bit());
        DisconnectPort();
    }
}

void Port::DisconnectPort() //Отключаем порт
{

    if(thisPort->isOpen())
    {
        flag_work = false;
        flag_start_MRK = false;
        emit stopTimerMrk();
        thisPort->close();
        error_(SettingsPort.name.toLocal8Bit() + " >> Закрыт!\r");
    }
}

void Port::WriteToPort(QByteArray data)//запись данных в порт
{

    if(thisPort->isOpen())
    {
        thisPort->write(data);
    }
}


//Остановка таймера без закрытия порта
void Port::END()
{
    emit stopTimerMrk();
}

void Port::Work()
{
//    if(timer_MRK_Data->isActive())
//    {
//            qDebug () << "Уже запущен";
//    }
//    else
//    {
        emit startTimerMrk(500);
 //   }
}



void Port::GetMrk()
{

    //    QByteArray b;
    //    b.append(0xfe);
    //    b.append(0xfe);
    //    b.append(0x02);
    //    b.append(0x18);
    //    b.append(0x01);
    //    b.append('\0');
    //    b.append(0x02);
    //    b.append('\0');
    //    b.append(0x03);
    //    b.append('\0');
    //    //    b.append(0x04);
    //    //    b.append('\0');
    //    b.append(0x88);
    //    b.append('\0');
    //    b.append(0x01);
    //    b.append(0xff);
    //    b.append(0xff);

    //Запрос 1 кадра
    QByteArray b;
    b.append(0xfe);
    b.append(0xfe);
    b.append(0x02);
    b.append(0x13);
    b.append(0x01);
    b.append(0x01);
    b.append(0xff);
    b.append(0xff);

    char c = NULL;

    char c1 = NULL;

    c1 = b[2]^b[3]^b[4];

    for(int i=2; i < b.count()-3;i++)
    {
        c ^=b[i];
        b[b.count()-3] = c;
    }

    this->WriteToPort(b);



}

void Port::GetMrk_OT()
{
    emit stopTimerMrk();

    flag_GetMrk_liters = true;
    qDebug () << " START 2 ZAPROS  LITERS OT ";

    QByteArray b;
    // Запрос 2 кадра
    b.append(0xfe);
    b.append(0xfe);
    b.append(0x02);
    b.append(0x13);
    b.append(0x02);
    b.append(0x01);
    b.append(0xff);
    b.append(0xff);

    char c = NULL;

    char c1 = NULL;

    c1 = b[2]^b[3]^b[4];

    for(int i=2; i < b.count()-3;i++)
    {
        c ^=b[i];
        b[b.count()-3] = c;
    }

    //  this->WriteToPort(b);

   // thisPort->clear(); //Пока что убрал проверить

    emit writeData(b);
}




void Port::GetMrk_liters(int liter)
{
    emit stopTimerMrk();

    qDebug () << "Установка литеры в 24 канала ";

    QByteArray b;

    b.append(0xfe);
    b.append(0xfe);
    b.append(0x02);
    b.append(0x15);
    b.append(0x01);
    //for(int i =22 ; i < 24;i++)
    for(int i =0 ; i < 24;i++)
    {
        //qDebug () << i;
        b.append(UINT8(i));
        b.append(UINT8(liter));          //UINT8(2) - вторая литера это 24 спутник

        b.append(0x78);
        b.append(0xEC);

        b.append(0x88);
        b.append(0x13);
    }

    b.append(0x01);
    b.append(0xff);
    b.append(0xff);


    char c = NULL;

    char c1 = NULL;

    c1 = b[2]^b[3]^b[4];

    for(int i=2; i < b.count()-3;i++)
    {
        c ^=b[i];
        b[b.count()-3] = c;
    }

    thisPort->clear();

    emit writeData(b);

    emit startTimerMrk(500);

}


void Port::GetMrk_liters_2(int liter)
{
    QByteArray b;

    b.append(0xfe);
    b.append(0xfe);
    b.append(0x02);
    b.append(0x15);
    b.append(0x01);
    //for(int i =22 ; i < 24;i++)
    for(int i =22 ; i < 24;i++)
    {
       // qDebug () << i;
        b.append(UINT8(i));
        b.append(UINT8(liter));          //UINT8(2) - вторая литера это 24 спутник

        b.append(0x78);
        b.append(0xEC);

        b.append(0x88);
        b.append(0x13);
    }

    b.append(0x01);
    b.append(0xff);
    b.append(0xff);


    char c = NULL;

    char c1 = NULL;

    c1 = b[2]^b[3]^b[4];

    for(int i=2; i < b.count()-3;i++)
    {
        c ^=b[i];
        b[b.count()-3] = c;
    }


    thisPort->clear();

    emit writeData(b);

    emit startTimerMrk(200);
}




bool Proverka(QByteArray data)
{
    QByteArray ok;
    ok.append(0xfe);
    ok.append(0xfe);
    ok.append(0x02);
    ok.append('\0');
    ok.append(0x02);
    ok.append(0xff);
    ok.append(0xff);

    if(data == ok)
    {
        return true;
    }
    else
    {
        return false;
    }
}


//void Port::ReadInProt() // чтение данных из порта
//{


//    dataBuild.append(thisPort->readAll());


//    QByteArray data;


//    for(int i=0; i < dataBuild.count();i++)
//    {

//        data.append(dataBuild[i]);

//        if(dataBuild[i] == 0xff && dataBuild[i-1] == 0xff)
//        {
//            dataBuild.clear();
//            break;
//        }
//    }


//    if(dataBuild.count() > 0)
//    {
//        return ;
//    }




//  //  // qDebug() << "ReadInProt  << " <<  data;



//    if (data != NULL)
//    {

//        if(data.count() < 10)
//        {
//            flag = false;
//            flag = Proverka(data);
//            return ;

//        }


//        if(flag_GetMrk_liters == true && data[4] == 0x02  && data.count() > 10  && ( data[data.count()-1] == 0xff && data[data.count()-2] == 0xff))
//        {

//            qDebug () << "START LITERS OT ";

//            flag_GetMrk_liters = false;

//            QByteArray b;

//            data[11] = 0x03;

//            //        auto lol = dataBuild[11]&0xf8;
//            //        dataBuild[11]=lol+2;

//            data.remove(0,4);
//            data.remove(data.count()-3,3);


//            b.append(0xfe);
//            b.append(0xfe);
//            b.append(0x02);
//            b.append(0x15);

//            for(int i = 0;i < data.count();i++)
//            {
//                b.append(data[i]);
//            }
//            b.append(0x01);
//            b.append(0xff);
//            b.append(0xff);
//            char c = NULL;

//            char c1 = NULL;

//            c1 = b[2]^b[3]^b[4];

//            for(int i=2; i < b.count()-3;i++)
//            {
//                c ^=b[i];
//                b[b.count()-3] = c;
//            }

//           // this->WriteToPort(b);
//            emit writeData(b);

//            dataBuild.clear();
//        }
//        else
//        {


//            if(data[data.count()-1] == 0xff && data[data.count()-2] == 0xff)
//            {

//             //   // qDebug() << "flag_end_MRK   << " <<  flag_end_MRK;

//                if(flag_end_MRK== true)
//                {
//                    dataBuild.clear();

//                    data = dataBuild;

//                    //                    CountFindGLONASS=0;
//                    //                    CountFindGPS=0;
//                    //                    CountFindALL=0;
//                    //                    emit UpdateCountFind(CountFindGLONASS,CountFindGPS,CountFindALL);
//                    return ;
//                }

//             //   // qDebug() << "flag_start_MRK   << " <<  flag_start_MRK;
//            //    // qDebug() << "flag   << " <<  flag;


//                if(flag)
//                {
//                    flag_start_MRK = true;

//                    //Удаление начала и конца принятого кадра
//                    data.remove(data.count()-3,3);
//                    data.remove(0,4);


//                    container = (frameExample *)data.data();

//                    // // qDebug() << container->A1L1[0];

//                    RazborFrame(*container);

//                    emit outMRKdata(*container);

//                }

//                dataBuild.clear();
//            }
//        }
//    }
//}




void Port::ReadInProt() // чтение данных из порта
{


    dataBuild.append(thisPort->readAll());

    for(int i=0; i < dataBuild.count();i++)
    {

        data.append(dataBuild[i]);

        if(i > 0 && dataBuild[i] == 0xfe && dataBuild[i-1] == 0xfe)
        {
            data.clear();
            data.append(0xfe);
            data.append(0xfe);
        }


        if(data[i] == 0xfe)
        {
            if(i > 1)
            {
                if(data[i] == 0xfe && data[i-1] == 0xff)
                {
                    data.remove(0,i);
                    dataBuild.remove(0,i);
                }
                else
                {
                    if(data[i] == 0xff && data[i-1] == 0xfe)
                    {
                        data.remove(0,i);
                        dataBuild.remove(0,i);
                    }
                }

            }

        }


        if(data[i] == 0xff)
        {
            if(i > 0 && data[i-1] == 0xff)
            {

                //data.clear();
                dataBuild.clear();

                break;
            }
        }

    }

    // qDebug() << "ReadInProt  << " <<  data;

    if(data[0] != 0xfe || data[1] != 0xfe || data[data.count()-1] != 0xff || data[data.count()-2] != 0xff )
    {


        dataBuild.clear();

        return ;
    }




    if (data != NULL)
    {

        if(data.count() < 10)
        {
            flag = false;
            flag = Proverka(data);

            data.clear();
            dataBuild.clear();

            return ;

        }
        else
        {
            flag = true;
        }



        if(flag_GetMrk_liters == true && data[4] == 0x02  && data.count() > 10  && ( data[data.count()-1] == 0xff && data[data.count()-2] == 0xff))
        {

           // qDebug () << "START LITERS OT ";

            flag_GetMrk_liters = false;

            QByteArray b;

            if(data[11] != 0x03)
            {
                data[11] = 0x03;
            }

            // data[11] = 0x02;

            //        auto lol = dataBuild[11]&0xf8;
            //        dataBuild[11]=lol+2;

            data.remove(0,4);
            data.remove(data.count()-3,3);


            b.append(0xfe);
            b.append(0xfe);
            b.append(0x02);
            b.append(0x15);

            for(int i = 0;i < data.count();i++)
            {
                b.append(data[i]);
            }
            b.append(0x01);
            b.append(0xff);
            b.append(0xff);
            char c = NULL;

            char c1 = NULL;

            c1 = b[2]^b[3]^b[4];

            for(int i=2; i < b.count()-3;i++)
            {
                c ^=b[i];
                b[b.count()-3] = c;
            }

            // this->WriteToPort(b);

            if(b[11] == 0x03)
            {
                emit writeData(b);
            }

            b.clear();

            data.clear();
            dataBuild.clear();

        }
        else
        {


            if(data[data.count()-1] == 0xff && data[data.count()-2] == 0xff)
            {

                //   // qDebug() << "flag_end_MRK   << " <<  flag_end_MRK;

                if(flag_end_MRK== true)
                {
                    dataBuild.clear();

                    data = dataBuild;

                    //                    CountFindGLONASS=0;
                    //                    CountFindGPS=0;
                    //                    CountFindALL=0;
                    //                    emit UpdateCountFind(CountFindGLONASS,CountFindGPS,CountFindALL);
                    return ;
                }

                //   // qDebug() << "flag_start_MRK   << " <<  flag_start_MRK;
                //    // qDebug() << "flag   << " <<  flag;


                if(flag)
                {
                    flag_start_MRK = true;

                    //Удаление начала и конца принятого кадра
                    data.remove(data.count()-3,3);
                    data.remove(0,4);
                    data.remove(0,2);





                    //Удаляем нули которые МРК поставил для того чтобы не перепутать с 0xfe 0xfe
                    for(int i=1;  i < data.count()-1;i++)
                    {
                        if(data[i-1] == 0xfe && data[i] == '\0')
                        {
                            data.remove(i,1); // удалит нули

                        }

                        if(data[i-1] == 0xff && data[i] == '\0')
                        {
                            data.remove(i,1); // удалит нули

                        }

                    }




                    lol.clear();

                    position_liters = 0;
                    position_number = 1;
                    position_Mode = 2;
                    position_A= 18;



                    container->NkaLit[0] = data[position_liters];
                    container->NkaNum[0] = data[position_number];
                    container->Ka1Mode[0] = data[position_Mode];


                    lol.append(data[position_A]);
                    lol.append(data[position_A+1]);


                    for(int i=1;  i < 24;i++) //73
                    {
                        position_liters += 24;
                        position_number += 24;
                        position_Mode += 24;
                        position_A += 24;

                        container->NkaLit[i] = data[position_liters];
                        container->NkaNum[i] = data[position_number];
                        container->Ka1Mode[i] = data[position_Mode];
                        // container->A1L1[i] = data[position_A];

                        lol.append(data[position_A]);
                        lol.append(data[position_A+1]);
                    }




                    container->A1L1 = (amplituda*)lol.data();



//                    for(int i=0;  i < 24;i++) //73
//                    {
//                        if(container->A1L1->A1L1[i] > 300  || container->A1L1->A1L1[i] < 0)
//                        {
//                            // qDebug() << "ОШИБКА";
//                        }

//                    }


                    RazborFrame(*container);



                 //   qDebug() << "dataBuild = " << dataBuild.count();
                 //    qDebug() << "data = " << dataBuild.count();

                  //  emit outMRKdata(*container);

                    data.clear();


                }
                dataBuild.clear();
            }
        }
    }

}



void Port::RazborFrame(frameExample container)
{

    CountFindGLONASS=0;
    CountFindGPS=0;
    CountFindALL=0;



   listSP->clear();
   listSP_Amplitude->clear();
   listSP_Name->clear();

    for(int i=0; i < 24;i++)
    {
        listSP->append(QString::number(container.Ka1Mode[i]));

        listSP_Amplitude->append(QString::number(container.A1L1->A1L1[i]));

        if(container.NkaNum[i] > 100)
        {
            auto positionGPS =  QString::number(container.NkaLit[i],2);

            bool ok;
            auto lol =  QString::number(container.NkaLit[i],2);
            lol.remove(0,1);

            listSP_Name->append("G"+QString::number(lol.toInt(&ok,2)));

        }
        else
        {
            if(container.NkaLit[i] < 32 && container.NkaLit[i] > 24)
            {
                //Преобразование отрицательной литеры

                listSP_Name->append("R"+QString::number(container.NkaNum[i])+"/"+QString::number(QString::number(container.NkaLit[i],10).toInt()-32));
            }
            else
            {
                 listSP_Name->append("R"+QString::number(container.NkaNum[i])+"/"+QString::number(container.NkaLit[i]));
            }
        }

        if(container.Ka1Mode[i] == 7)
        {
            CountFindALL++;

            if(container.NkaNum[i] > 100)
            {
                auto positionGPS =  QString::number(container.NkaLit[i],2);


                CountFindGPS++;
            }
            else
            {
                if(container.NkaLit[i] < 32 && container.NkaLit[i] > 24)
                {
                    //Преобразование отрицательной литеры

                    CountFindGLONASS++;
                }
                else
                {

                    CountFindGLONASS++;
                }
            }
        }
    }

    // // qDebug() <<"listSP = " << listSP;

    emit signal_GoTORelizproverka(*listSP,*listSP_Amplitude,*listSP_Name);


    listSP->clear();
    listSP_Amplitude->clear();
    listSP_Name->clear();


    data.clear();
    dataBuild.clear();

    lol.clear();


    emit UpdateCountFind(CountFindGLONASS,CountFindGPS,CountFindALL);
    //      // qDebug() <<"CountFindGLONASS = " << CountFindGLONASS;
    //     // qDebug() <<"CountFindGPS = " <<  CountFindGPS;
    //        // qDebug() <<"CountFindALL = " <<  CountFindALL;

   // qDebug () << "Память ! = " <<sizeof(this);



}

