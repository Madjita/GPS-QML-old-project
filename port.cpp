#include "port.h"


#include <QThread>

Port::Port(QObject *parent) :
    QObject(parent),
    CountFindALL(0),
    flag_start_MRK(false),
    position_liters(0),
    position_number(1),
    position_Mode(2),
    position_A(18),
    // thisPort(new QSerialPort()),
    flag_work(false),
    flag_getName(false),
    flag_waitloadingMRK(false),
    flag_waitloadingMRK_proverka(false)
{
    //помещаем класс в поток
    this->moveToThread(new QThread());
    connect(this->thread(),&QThread::started,this,&Port::process_start);
    this->thread()->start();

}

Port::~Port()
{
    qDebug("By in Thread!");

    emit stopTimerMrk();

    emit finihed_Port(); // Сигнал о завершении работы

}

void Port::process_start()
{
    thisPort = new QSerialPort();

    qDebug("Open the port.cpp on the new Thread");

    connect(thisPort, &QSerialPort::readyRead,this,&Port::ReadInProt); //подключаем чтение с порта по сигналу readyRead()

    //connect(thisPort, SIGNAL(readyRead()),this,SLOT(getComData())); //подключаем чтение с порта по сигналу readyRead()

    listSP = new QStringList();

    listSP_Amplitude = new QStringList();
    listSP_Name = new QStringList();

    container = new frameExample();

    for(int i=0; i < 24;i++)
    {
        frame[i] = new frame1();
    }


    connect(  thisPort->thread(),&QThread::started, this,&Port::process_Port); // Переназначения метода run
    connect(  this, &Port::finihed_Port,  thisPort->thread(),&QThread::quit); // Переназначение метода выход


    //Инициализация таймеров
    timer_MRK_Data = new QTimer();
    timer_errorStartMRk = new QTimer();

    connect(timer_MRK_Data,&QTimer::timeout,this,&Port::GetMrk);
    connect(this,SIGNAL(startTimerMrk(int)),timer_MRK_Data,SLOT(start(int)));
    connect(this,&Port::stopTimerMrk,timer_MRK_Data,&QTimer::stop);


    connect(timer_errorStartMRk,&QTimer::timeout,this,&Port::timerStartMrk);
    connect(this,SIGNAL(startTimerMrkError(int)),timer_errorStartMRk,SLOT(start(int)));
    connect(this,&Port::stopTimerMrk,timer_errorStartMRk,&QTimer::stop);


    connect(this, &Port::start_UdpZapros,this,&Port::GetMrk);


    //connect(this,SIGNAL(writeData(QByteArray)),this,SLOT(WriteToPort(QByteArray))); // отправить данные
    connect(this,&Port::writeData,this,&Port::WriteToPort);


    //qRegisterMetaType<QSerialPort::SerialPortError>();
    //qRegisterMetaType<QVector<int>>();
}


void Port::process_Port() //Выполняется при старте класса
{

    //    qDebug("Open the port.cpp on the new Thread NEW");
    //    connect(thisPort, SIGNAL(readyRead()),this,SLOT(ReadInProt())); //подключаем чтение с порта по сигналу readyRead()


    //    listSP = new QStringList();

    //    listSP_Amplitude = new QStringList();
    //    listSP_Name = new QStringList();

    //    container = new frameExample();

    //    for(int i=0; i < 24;i++)
    //    {
    //        frame[i] = new frame1();
    //    }


}

void Port::Write_Setting_Port(QString name, int baudrate, int DataBits, int parity, int StopBits, int FlowControl) //заносим параметры порта в структуру данных
{
    SettingsPort.name = name;

    SettingsPort.baudRate = static_cast<QSerialPort::BaudRate>(baudrate);
    SettingsPort.dataBits = static_cast<QSerialPort::DataBits>(DataBits);
    SettingsPort.parity = static_cast<QSerialPort::Parity>(parity);
    SettingsPort.stopBits = static_cast<QSerialPort::StopBits>(StopBits);
    SettingsPort.flowControl = static_cast<QSerialPort::FlowControl>(FlowControl);

    //        SettingsPort.baudRate = (QSerialPort::BaudRate) baudrate;
    //        SettingsPort.dataBits = QSerialPort::Data8;
    //        SettingsPort.parity = QSerialPort::NoParity;
    //        SettingsPort.stopBits = QSerialPort::OneStop;
    //        SettingsPort.flowControl = QSerialPort::NoFlowControl;
}


void Port::ConnectPort(void) //Процедура подключения
{
    thisPort->setPortName(SettingsPort.name);


    if(thisPort->open(QIODevice::ReadWrite))
    {
        if(thisPort->setBaudRate(SettingsPort.baudRate) && thisPort->setDataBits(SettingsPort.dataBits)&& thisPort->setParity(SettingsPort.parity)&& thisPort->setStopBits(SettingsPort.stopBits)&& thisPort->setFlowControl(SettingsPort.flowControl))
        {
            if(thisPort->isOpen())
            {
                //Открыт порт
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
        }
    }else
    {
        thisPort->close();
        emit outPortOpen(thisPort->errorString());
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
        thisPort->flush();
    }
}

void Port::timerStartMrk()
{
    countWaitMRK++;

    if(countWaitMRK > 20)
    {
        qDebug() << "timerStartMrk ERRR " << flag_waitloadingMRK << flag_waitloadingMRK_proverka;

        timer_errorStartMRk->stop();
        emit stopTimerMrk();

        if(flag_waitloadingMRK && flag_waitloadingMRK_proverka)
        {
            emit signal_MRkLoadProverka(false);
        }
        else
        {
            emit signal_MRkLoad(false);
        }

        flag_waitloadingMRK = false;
        flag_waitloadingMRK_proverka = false;

        countWaitMRK =0;
    }
}


//Остановка таймера без закрытия порта
void Port::END()
{
    emit stopTimerMrk();
}

void Port::Work()
{
    emit startTimerMrk(750);
    emit startTimerMrkError(1000);
}

void Port::GetMrk()
{
    dataBuild.clear();
    data.clear();

    //Запрос 1 кадра
    QByteArray b;
    b.append(static_cast<char>(0xfe));
    b.append(static_cast<char>(0xfe));
    b.append('\0');
    b.append(0x13);
    b.append(0x01);
    b.append(0x01);
    b.append(static_cast<char>(0xff));
    b.append(static_cast<char>(0xff));

    char c = '\0';

    for(int i=2; i < b.count()-3;i++)
    {
        c ^=b[i];
        b[b.count()-3] = c;
    }

    CountFindGLONASS =0;
    CountFindGPS = 0;
    CountFindALL = 0;

    emit writeData(b);

}

void Port::GetMrk_OT()
{
    emit stopTimerMrk();
    dataBuild.clear();
    data.clear();

    flag_GetMrk_liters = true;
    qDebug() << " START 2 ZAPROS  LITERS OT ";

    QByteArray b;
    // Запрос 2 кадра
    b.append(static_cast<char>(0xfe));
    b.append(static_cast<char>(0xfe));
    b.append('\0');
    b.append(0x13);
    b.append(0x02);
    b.append(0x01);
    b.append(static_cast<char>(0xff));
    b.append(static_cast<char>(0xff));

    char c = '\0';

    for(int i=2; i < b.count()-3;i++)
    {
        c ^=b[i];
        b[b.count()-3] = c;
    }

    emit writeData(b);
}





void Port::GetMrk_liters(int liter)
{
    emit stopTimerMrk();
    dataBuild.clear();
    data.clear();

    //qDebug() << "Установка литеры в 24 канала ";

    QByteArray b;

    b.append(static_cast<char>(0xfe));
    b.append(static_cast<char>(0xfe));
    b.append('\0');
    b.append(0x15);
    b.append(0x01);
    for(int i =0 ; i < 24;i++)
    {
        b.append(static_cast<char>(UINT8(i)));
        b.append(static_cast<char>(UINT8(liter)));          //UINT8(2) - вторая литера это 24 спутник

        b.append(0x78);
        b.append(static_cast<char>(0xEC));

        b.append(static_cast<char>(0x88));
        b.append(0x13);
    }

    b.append(0x01);
    b.append(static_cast<char>(0xff));
    b.append(static_cast<char>(0xff));


    char c = '\0';

    for(int i=2; i < b.count()-3;i++)
    {
        c ^=b[i];
        b[b.count()-3] = c;
    }

    emit writeData(b);


    count_setLiters++;
    if(count_setLiters >= 10)
    {
        emit startTimerMrk(750);
        emit startTimerMrkError(1000);
        count_setLiters=0;
    }

}


void Port::GetMrk_Name()
{
    count_setLiters = 0;
    dataBuild.clear();
    data.clear();
    nameMRK.clear();

    //Запрос Идентификационный номер платы навига-ционного приемника в составе МРК

    flag_getName = true;

    QByteArray b;
    b.append(static_cast<char>(0xfe));
    b.append(static_cast<char>(0xfe));
    b.append('\0');
    b.append(0x18);
    b.append(static_cast<char>(0xA9));
    b.append('\0');
    b.append(0x01); //контрольная сумма
    b.append(static_cast<char>(0xff));
    b.append(static_cast<char>(0xff));

    char c = '\0';

    for(int i=2; i < b.count()-3;i++)
    {
        c ^=b[i];
        b[b.count()-3] = c;
    }

    emit writeData(b);
}




//bool Proverka(QByteArray data)
//{
//    QByteArray ok;
//    ok.append(static_cast<char>(0xfe));
//    ok.append(static_cast<char>(0xfe));
//    ok.append(0x02);
//    ok.append('\0');
//    ok.append(0x02);
//    ok.append(static_cast<char>(0xff));
//    ok.append(static_cast<char>(0xff));

//    if(data == ok)
//    {
//        return true;
//    }
//    else
//    {
//        return false;
//    }
//}


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




//  //  //qDebug() << "ReadInProt  << " <<  data;



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

//            //qDebug() << "START LITERS OT ";

//            flag_GetMrk_liters = false;

//            QByteArray b;

//            data[11] = 0x03;

//            //        auto lol = dataBuild[11]&0xf8;
//            //        dataBuild[11]=lol+2;

//            data.remove(0,4);
//            data.remove(data.count()-3,3);


//           b.append(static_cast<char>(0xfe));
//           b.append(static_cast<char>(0xfe));
//            b.append(0x02);
//            b.append(0x15);

//            for(int i = 0;i < data.count();i++)
//            {
//                b.append(data[i]);
//            }
//            b.append(0x01);
//           b.append(static_cast<char>(0xff));
//           b.append(static_cast<char>(0xff));
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

//             //   //qDebug() << "flag_end_MRK   << " <<  flag_end_MRK;

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

//             //   //qDebug() << "flag_start_MRK   << " <<  flag_start_MRK;
//            //    //qDebug() << "flag   << " <<  flag;


//                if(flag)
//                {
//                    flag_start_MRK = true;

//                    //Удаление начала и конца принятого кадра
//                    data.remove(data.count()-3,3);
//                    data.remove(0,4);


//                    container = (frameExample *)data.data();

//                    //qDebug() << container->A1L1[0];

//                    RazborFrame(*container);

//                    emit outMRKdata(*container);

//                }

//                dataBuild.clear();
//            }
//        }
//    }
//}


int errorCount =0;

void Port::ReadInProt() // чтение данных из порта
{

    if(timer_errorStartMRk->isActive())
    {
        timer_errorStartMRk->stop();
        countWaitMRK =0;
    }

    dataBuild.append(thisPort->readAll()); // чтение данных с COM

//    qDebug() << "Count : " <<  dataBuild.count();
//    qDebug() << "====================================";
//    qDebug() << dataBuild.toHex(' ');
//    qDebug() << "====================================";

    if(dataBuild.count() > 0)
    {
        if(getMessage(dataBuild))
        {

            if(data.count() < 10)
            {
                flag = false;
                flag = Proverka(data);

               // qDebug() << data.toHex(' ');

                if(flag)
                {
                    //qDebug() << nameMRK << "Message Good : " << data.toHex(' ').split(' ').value(3);
                }
                else
                {
                    //qDebug() << nameMRK << "ERROR : " << data.toHex(' ').split(' ').value(3);
                }

                data.clear();
                dataBuild.clear();
                zahvat_fefe = false;
                zahvat_ffff = false;
                return ;

            }
            else
            {
                if(zahvat_fefe && zahvat_ffff)
                {
                    flag = true;
                }
                else
                {
                    dataBuild.clear();
                    return ;
                }
            }
        }
        else
        {
            dataBuild.clear();
            return;
        }

//        qDebug() <<  nameMRK <<  "=================DATA==================";
//        qDebug() << data.toHex(' ');
//        qDebug() <<  nameMRK <<  "====================================";

        char KS = data[data.count()-3];
        char _KS = '\0';

        for(int i=2; i < data.count()-3;i++)
        {
            _KS ^=data[i];
        }

        if(_KS == KS)
        {
           // qDebug() << "KS GOOD";
            zahvat_fefe = false;
            zahvat_ffff = false;
        }
        else
        {
          //  qDebug() << "KS Bead : " << QString::number(_KS,16).toUpper() << " ; " << QString::number(KS,16).toUpper();

            data.clear();
            dataBuild.clear();
            zahvat_fefe = false;
            zahvat_ffff = false;
            return ;
        }


        ////////////////////////////////////


        //qDebug() << "==============GOOD===============";
        //qDebug() << dataBuild;
        //qDebug() << "=======================================";


        //КОД для того чтоб узнать название платы
        if(nameMRK.count() == 0 && flag_getName)
        {
            ReadInProt_nameNP101(data);
            dataBuild.clear();
            return;
        }



        if(flag_GetMrk_liters == true && static_cast<char>(data[4]) == 0x02  && data.count() > 10  && ( data[data.count()-1] == static_cast<char>(0xff) && data[data.count()-2] == static_cast<char>(0xff)))
        {

            qDebug() << "START LITERS OT ";

            flag_GetMrk_liters = false;

            QByteArray b;

            if(data[11] != static_cast<char>(0x03))
            {
                data[11] = 0x03;
            }

            // data[11] = 0x02;

            //        auto lol = dataBuild[11]&0xf8;
            //        dataBuild[11]=lol+2;

            data.remove(0,4);
            data.remove(data.count()-3,3);


            b.append(static_cast<char>(0xfe));
            b.append(static_cast<char>(0xfe));
            b.append(0x02);
            b.append(0x15);

            for(int i = 0;i < data.count();i++)
            {
                b.append(data[i]);
            }
            b.append(0x01);
            b.append(static_cast<char>(0xff));
            b.append(static_cast<char>(0xff));

            char c = '\0';

            for(int i=2; i < b.count()-3;i++)
            {
                c ^=b[i];
                b[b.count()-3] = c;
            }

            if(b[11] == static_cast<char>(0x03))
            {
                emit writeData(b);
            }

            b.clear();

            data.clear();
            dataBuild.clear();

        }
        else
        {

            if(data[data.count()-1] == static_cast<char>(0xff) && data[data.count()-2] == static_cast<char>(0xff))
            {

                if(flag_end_MRK == true)
                {
                    dataBuild.clear();
                    data = dataBuild;
                    return ;
                }


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
                        if(data[i-1] == static_cast<char>(0xfe) && data[i] == '\0')
                        {
                            data.remove(i,1); // удалит нули
                        }

                        if(data[i-1] == static_cast<char>(0xff) && data[i] == '\0')
                        {
                            data.remove(i,1); // удалит нули
                        }
                    }

                    lol.clear();

                    position_liters = 0;
                    position_number = 1;
                    position_Mode = 2;
                    position_A= 18;



                    container->NkaLit[0] = static_cast<unsigned char>(data[position_liters]);
                    container->NkaNum[0] = static_cast<unsigned char>(data[position_number]);
                    container->Ka1Mode[0] = static_cast<unsigned char>(data[position_Mode]);


                    lol.append(data[position_A]);
                    lol.append(data[position_A+1]);


                    for(int i=1;  i < 24;i++) //73
                    {
                        position_liters += 24;
                        position_number += 24;
                        position_Mode += 24;
                        position_A += 24;

                        container->NkaLit[i] = static_cast<unsigned char>(data[position_liters]);
                        container->NkaNum[i] = static_cast<unsigned char>(data[position_number]);
                        container->Ka1Mode[i] = static_cast<unsigned char>(data[position_Mode]);

                        lol.append(data[position_A]);
                        lol.append(data[position_A+1]);
                    }

                    container->A1L1 =  reinterpret_cast<amplituda*>(lol.data());

                    RazborFrame(*container);

                    data.clear();

                }
                dataBuild.clear();
            }
        }


        ///////////////////////////

    }
    else
    {
        dataBuild.clear();
    }





}

void Port::ReadInProt_nameNP101(QByteArray _nameMRK)
{
    nameMRK.clear();

    for(int i=0;i < _nameMRK.count();i++)
    {
        nameMRK.append(_nameMRK[i]);
    }

    nameMRK.remove(0,4);
    nameMRK.remove(nameMRK.count()-3,3);
    qDebug() << "nameMRK_String = " << nameMRK.toHex() << "nameMRK = " << nameMRK;

    emit signal_MRkgetName(nameMRK);
    flag_getName = false;


    data.clear();
    dataBuild.clear();
    zahvat_fefe = false;
    zahvat_ffff = false;
}


bool Port::getMessage(QByteArray _data)
{
    ///////////////////////////////////

    int N = _data.count();


    if(N != 1)
    {
        for(int i=0; i < N-1;i++)
        {
            // если конец и начало посылки еще не найдены
            if(zahvat_ffff == false && zahvat_fefe == false)
            {
                // Поиск начала посылки
                if(_data[i] == static_cast<char>(0xfe) && _data[i+1] == static_cast<char>(0xfe))
                {
                    // если начало посылки не совпадает с началом данных, то очищаем массив результатов
                    if(i > 0 )
                    {
                        data.clear();
                        dataBuild.clear();
                        return false;
                    }
                    else
                    {
                        zahvat_fefe = true; //поймали начало посылки fefe
                        // data.append(_data[i]); //записали fe
                    }
                }
            }
            else
            {

                // Поиск начала посылки ошибочного сообзения
                if(_data[i] == static_cast<char>(0xfe) && _data[i+1] == static_cast<char>(0xfe))
                {
                    data.clear();
                    dataBuild.clear();
                    return false;
                }
                else
                {
                    // Поиск конца посылки
                    if(_data[i] == static_cast<char>(0xff) && _data[i+1] == static_cast<char>(0xff))
                    {

                        if(i+1 == N-1)
                        {
                            zahvat_ffff = true; //поймали конец посылки ffff
                        }
                        else
                        {
                            data.clear();
                            dataBuild.clear();
                            return false;
                        }
                    }
                }

            }
        }
    }
    else
    {
        data.append(_data); //записали данные
        // Поиск конца посылки
        if(data[data.count()-1] == static_cast<char>(0xff) && data[data.count()-2] == static_cast<char>(0xff))
        {
            zahvat_ffff = true; //поймали конец посылки ffff
        }

        data.remove(data.count()-1,1);
    }



    if(zahvat_fefe && zahvat_ffff)
    {
        data.append(_data); //записали данные
        return true;
    }
    else
    {
        data.append(_data); //записали данные
        return false;
    }
}

bool Port::Proverka(QByteArray _data)
{
    QByteArray ok;
    ok.append(static_cast<char>(0xfe));
    ok.append(static_cast<char>(0xfe));
    ok.append(0x02);
    ok.append('\0');
    ok.append(0x02);
    ok.append(static_cast<char>(0xff));
    ok.append(static_cast<char>(0xff));

    if(_data[3] == '\0')
    {
        return true; // Приемник принял данные верно
    }
    else
    {

        switch (_data[3]) {
        case 0x01: emit signalErrorMessage("01h – неверный (несуществующий, неподдерживаемый) код операции.",0x01); return  false;
        case 0x02: emit signalErrorMessage("02h – несовпадение контрольной суммы при приеме сообщения.",0x02); return false;
        case 0x03: emit signalErrorMessage("03h – неправильная длина команды (число принятых байт не соответствует коду операции), блок <данные> содержит принятую длину.",0x03); return false;
        case 0x04: emit signalErrorMessage("04h – запрашиваемый кадр не существует (не назначен).",0x04); return false;
        case 0x05: emit signalErrorMessage("05h – в принятой команде (кадре) обнаружены некорректные параметры.",0x05); return false;
        case 0x06: emit signalErrorMessage("06h – номер запрашиваемого параметра превышает допустимый для данного прибора.",0x06); return false;
        case 0x07: emit signalErrorMessage("07h – превышение размера ответного сообщения.",0x07); return false;
        case 0x09: emit signalErrorMessage("09h – канал занят для передачи (команды, принятые во время передачи, не обрабатываются).",0x09); return false;
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

        if(QString::number(container.Ka1Mode[i]) == "7")
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

    qDebug() << CountFindALL <<  " ) listSP = " << listSP->toVector();

    if(flag_waitloadingMRK)
    {
        if(flag_waitloadingMRK && flag_waitloadingMRK_proverka)
        {
            emit signal_MRkLoadProverka(true);
            flag_waitloadingMRK = false;
            flag_waitloadingMRK_proverka = false;
        }
        else
        {
            emit stopTimerMrk();
            emit signal_MRkLoad(true);
            flag_waitloadingMRK = false;
            flag_waitloadingMRK_proverka = false;
        }

    }
    else
    {
        emit signal_GoTORelizproverka(*listSP,*listSP_Amplitude,*listSP_Name);
        emit UpdateCountFind(CountFindGLONASS,CountFindGPS,CountFindALL);

    }


    data.clear();
    dataBuild.clear();
    lol.clear();

}

