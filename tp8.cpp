#include "tp8.h"

#include <QThread>

/*!
    \brief Конструктор класса ТП8


   Контструктор класса ТП8 , инициализирует новый поток ( QThread ) и запускает его, выполняя функцию process_start().

    \author Sergey Smoglyuk
    \version 1.0
    \date Август 2018 года
    \warning Перемещение класса в новый поток
*/
TP8::TP8(QObject *parent) : QObject(parent),
    index(-1)
{

    this->moveToThread(new QThread()); //Переместили класс TP8 в новый поток
    //this->metaObject()->invokeMethod(this, "process_start", Qt::QueuedConnection);

    QObject::connect(this->thread(),&QThread::started,this,&TP8::process_start);
    this->thread()->start();


    connect(this, &TP8::signal_finished, this->thread(), &QThread::quit);

}

TP8::~TP8()
{
    qDebug() << "Delete TP8";
    emit signal_finished();

   // delete otvet_first;
   // delete otvet;




}

/*!
    \brief Функция для переворачивания байты порта.

    \param[in]  QByteArray datagram
    \return QByteArray datagram с перевернутыми данными внутри на 11,12,13,14 позициях

   Функция принимает прочитанную датаграмму от пульта и переворачивает баты порта для того чтобы корректно отправить ответ\n
   Возвращает QByteArray (массив байт) с перевернутым портом в послыке принятия и отправки.

    \author Sergey Smoglyuk
    \version 1.0
    \date Август 2018 года
    \warning Плохо сделанная прошивка в пульте поэтому есть такой Кастыль. Притензии к Глушкову Антону.
*/
QByteArray TP8::perevod( QByteArray datagram)
{
    char tmp = datagram[11];
    datagram[11] = datagram[12];
    datagram[12] = tmp;

    tmp = datagram[13];
    datagram[13] = datagram[14];
    datagram[14] = tmp;


    return datagram;


}


/*!
    \brief Слот для чтения посылки от пульта ТП8 и принятие решения какую посылку отправить в ответ


   Функция читает посылку от пульта ТП8 и принятие решения какую посылку отправить в ответ.\n
   Пишет в Debug принята ли команда и выполненна ли.

    \author Sergey Smoglyuk
    \version 1.0
    \date Август 2018 года
    \warning Данная функция выполняется в отдельном потоке
*/
void TP8::readPendingDatagrams()
{

//    qDebug() << "dhjasfkljdhs;flahjfjalkd;eswja;fldjsf : " << udpsocket->pendingDatagramSize() << " = " << udpsocket->hasPendingDatagrams();

    while (udpsocket->hasPendingDatagrams())
    {
        QByteArray datagram;

        datagram.resize(static_cast<int>(udpsocket->pendingDatagramSize())); //

        QHostAddress* sender = new QHostAddress();
        quint16* senderPort = new quint16();

        udpsocket->readDatagram(datagram.data(), datagram.size(),sender, senderPort);

//        if(datagram.count() == 0)
//        {
//            flagSend = true;
//            udpsocket->writeDatagram(b,b.count(),QHostAddress(ipTP),6683);
//            return;
//        }

        qDebug() << datagram.toHex(' ');
        qDebug() << datagram[0];

        if(datagram.count() > 10 && datagram[0] != static_cast<char>(0x1C))
        {
            return ;
        }

        switch (datagram[0])
        {

        case 0x1C: {
            datagram = perevod(datagram);
            otvet_first = reinterpret_cast<find*>(datagram.data());

            //запишим найденный IP в переменную (на всякий случай)
            ipTP = QString::number(otvet_first->ipTP[0]&0xFF) + "." +
                    QString::number(otvet_first->ipTP[1]&0xFF) + "." +
                    QString::number(otvet_first->ipTP[2]&0xFF) + "." +
                    QString::number(otvet_first->ipTP[3]&0xFF);


            listTP.append(*otvet_first);

            QByteArray byte_ip;

            QStringList locIP_split = locIP.split('.');


            stop_timer();

            for(int i=0;i < locIP_split.count();i++)
            {
                byte_ip.append(static_cast<char>(locIP_split[i].toInt()));

            }

            flagConnect = false;


            if(otvet_first->mod == static_cast<char>(0x09))
            {
                qDebug () << "TP free . Clear Tp8";
                break;
            }
            else
            {
                qDebug () << "TP wait . Close Tp8";


                for(int i=0;i < byte_ip.count();i++)
                {
                    if(otvet_first->ipServer[i] == byte_ip[i])
                    {
                        flagConnect = true;
                    }
                    else
                    {
                        flagConnect = false;
                    }
                }

            }


            if(flagConnect)
            {
                emit signal_Ok(true);
            }
            else
            {
                emit signal_Ok(false);
            }

           // flagSend = false;

            break;
        }

        case char(0xFB):{
            if(datagram[2] ==  static_cast<char>(0x02))
            {
                qDebug () << "Comand 0xFB on clear get";
                otvet = reinterpret_cast<Otvet*>(datagram.data());
                stop_timer();
            }
            else
            {
                if(datagram[2] == static_cast<char>(0x03))
                {
                    qDebug () << "Comand 0xFB on clear finish";

                    //if(flagSend)
                        emit signal_comandaClear_ok();

                   // flagSend = false; //проверить
                }
            }
            break;
        }
        case 0x01: {

            if(datagram[2] ==  static_cast<char>(0x01))
            {
                qDebug () << "Comand 0x01 get";
                otvet = reinterpret_cast<Otvet*>(datagram.data());
            }
            else
            {
                if(datagram[2] == static_cast<char>(0x03))
                {
                    qDebug () << "Comand 0x01 finish";

                   // if(flagSend)
                        emit signal_comanda_ok(1,index);

                   // flagSend = false;

                }
            }


            break;
        }

        case 0x02: {

            if(datagram[2] ==  static_cast<char>(0x02))
            {
                qDebug () << "Comand 0x02 get";
                otvet = reinterpret_cast<Otvet*>(datagram.data());
                stop_timer();
            }
            else
            {
                if(datagram[2] ==  char(0x03))
                {
                    qDebug () << "Comand 0x02 finish";

                   // if(flagSend)
                        emit signal_comanda_ok(2,index);

                   // flagSend = false;

                }
            }


            break;
        }
        case 0x03: {

            if(datagram[2] ==  static_cast<char>(0x02))
            {
                qDebug () << "Comand 0x03 get";
                otvet = reinterpret_cast<Otvet*>(datagram.data());
                stop_timer();
            }
            else
            {
                if(datagram[2] ==  static_cast<char>(0x03))
                {
                    qDebug () << "Comand 0x03 finish";

                   // if(flagSend)
                        emit signal_comanda_ok(3,index);

                   // flagSend = false;

                }
            }


            break;
        }
        case 0x04: {
            if(datagram[2] ==  static_cast<char>(0x02))
            {
                qDebug () << "Comand 0x04 get";
                otvet = reinterpret_cast<Otvet*>(datagram.data());
                stop_timer();
            }
            else
            {
                if(datagram[2] ==  static_cast<char>(0x03))
                {
                    qDebug () << "Comand 0x04 finish";

                  //  if(flagSend)
                        emit signal_comanda_ok(4,index);

                  //   flagSend = false;

                }
            }

            break;
        }
        case 0x05: {
            if(datagram[2] ==  static_cast<char>(0x02))
            {
                qDebug () << "Comand 0x05 get";
                otvet = reinterpret_cast<Otvet*>(datagram.data());
                stop_timer();
            }
            else
            {
                if(datagram[2] ==  static_cast<char>(0x03))
                {
                    qDebug () << "Comand 0x05 finish";

                   // if(flagSend)
                        emit signal_comanda_ok(5,index);

                  //  flagSend = false;
                }
            }

            break;
        }
        case 0x06: {
            if(datagram[2] ==  static_cast<char>(0x02))
            {
                qDebug () << "Comand 0x06 get";
                otvet = reinterpret_cast<Otvet*>(datagram.data());
                stop_timer();
            }
            else
            {
                if(datagram[2] == static_cast<char>(0x03))
                {
                    qDebug () << "Comand 0x06 finish";

                  //  if(flagSend)
                        emit signal_comanda_ok(6,index);

                  //   flagSend = false;
                }
            }

            break;
        }
        case 0x07: {
            if(datagram[2] ==  static_cast<char>(0x02))
            {
                qDebug () << "Comand 0x07 get";
                otvet = reinterpret_cast<Otvet*>(datagram.data());
                stop_timer();
            }
            else
            {
                if(datagram[2] ==  static_cast<char>(0x03))
                {
                    qDebug () << "Comand 0x07 finish";

                   // if(flagSend)
                        emit signal_comanda_ok(7,index);

                  //  flagSend = false;

                }
            }

            break;
        }
        case 0x08: {
            if(datagram[2] ==  static_cast<char>(0x02))
            {
                qDebug () << "Comand 0x08 get";
                otvet = reinterpret_cast<Otvet*>(datagram.data());
                stop_timer();
            }
            else
            {
                if(datagram[2] ==  static_cast<char>(0x03))
                {
                    qDebug () << "Comand 0x08 finish";

                   // if(flagSend)
                        emit signal_comanda_ok(8,index);

                  //  flagSend = false;

                }
            }

            break;
        }


        }


    }

}

/*!
    \brief Слот для поиска ТП8 для получения IP адреса по которому подключится ПК

   Функция отправляет широковещательный UDP запрос (Broadcast)\n
   Порт отправки: 6683
   Состав посылки:\n
   1C\n
   1C\n
   01\n
   байты IP адреса\n
   2 байта порта пульта\n
   2 байта порта ПК\n
    \author Sergey Smoglyuk
    \version 1.0
    \date Август 2018 года
    \warning Данная функция выполняется в отдельном потоке
*/
void TP8::slot_findTp()
{
    listTP.clear();

    b.clear();
    QByteArray byte_ip;

    QStringList locIP_split;


    locIP_split = locIP.split('.');


    for(int i=0;i < locIP_split.count();i++)
    {
        byte_ip.append(static_cast<char>(locIP_split[i].toInt()));
    }


    b.append(0x1C);
    b.append(0x1C);
    b.append(0x01);
    //Ip
    for(int i=0;i < locIP_split.count();i++)
    {
        b.append(byte_ip[i]);
    }

    // Первый - порт пульта Второй - порт сервера
    b.append(static_cast<char>(0xFF));
    b.append(static_cast<char>(0xDC));
    b.append(static_cast<char>(0xFF));
    b.append(static_cast<char>(0xE6));

    udpsocket->writeDatagram(b,b.count(),QHostAddress::Broadcast,6683);
}

/*!
    \brief Слот для подключения ТП8 к ПК
    \param[in] QString ip Ip адресс пульта ТП8

   Функция принимает IP адресс прибора ТП8 для его подключения к ПК\n
   Порт отправки: 6683
    \author Sergey Smoglyuk
    \version 1.0
    \date Август 2018 года
    \warning Данная функция выполняется в отдельном потоке
*/
void TP8::slot_ConnectTp(QString ip)
{
    listTP.clear();

    ipTP = ip;

    QStringList locIP_split;
    b.clear();
    QByteArray byte_ip;



    locIP_split = locIP.split('.');


    for(int i=0;i < locIP_split.count();i++)
    {
        byte_ip.append(static_cast<char>(locIP_split[i].toInt())); //проверить
    }


    b.append(static_cast<char>(0x1C));
    b.append(static_cast<char>(0x1C));
    b.append(static_cast<char>(0x03));
    //Ip
    for(int i=0;i < locIP_split.count();i++)
    {
        b.append(byte_ip[i]);
    }

    b.append(static_cast<char>(0xFF));
    b.append(static_cast<char>(0xDC));
    b.append(static_cast<char>(0xFF));
    b.append(static_cast<char>(0xE6));

    udpsocket->writeDatagram(b,b.count(),QHostAddress::Broadcast /*QHostAddress(ipTP)*/,6683); //QHostAddress(ipTP)

    index = -1;

}

/*!
    \brief Слот для выполняения 1 команды
    \param[in] char NP байт номера навигационного приемника в пульте ТП8 к которому применить команду

   Подключение к «ВЫХОД2» сигнала 10МГц с НП1-НП8\n
   Здесь младший байт — это номер НП с которого осуществляется подключение сигнала 10 МГц на «ВЫХОД2»\n
   Младший байт: 01-08.\n
   Порт отправки: 65500
    \author Sergey Smoglyuk
    \version 1.0
    \date Август 2018 года
    \warning Данная функция выполняется в отдельном потоке
*/
void TP8::slot_comand1_Connect_Vx2_10(char NP,int _index)
{
    index = _index;

    b.clear();

    b.append(0x01);
    b.append(NP);
    b.append(0x01);

    flagSend = true;
    udpsocket->writeDatagram(b,b.count(),QHostAddress::Broadcast /*QHostAddress(ipTP)*/,65500);

    countMessage =1;
    timer->start(10000);
}

/*!
    \brief Слот для выполняения 2 команды
    \param[in] char NP байт номера навигационного приемника в пульте ТП8 к которому применить команду

   Подключение к «ВЫХОД1» сигнала 1С с НП1-НП8\n
   Здесь младший байт — это номер НП с которого осуществляется подключение сигнала 1С на «ВЫХОД1»\n
   Младший байт: 01-08.
    \author Sergey Smoglyuk
    \version 1.0
    \date Август 2018 года
    \warning Данная функция выполняется в отдельном потоке
*/
void TP8::slot_comand2_Connect_Vx1_1C(char NP,int _index)
{
    qDebug() << "slot_comand2_Connect_Vx1_1C \t index: " << _index;
    index = _index;

    b.clear();

    b.append(0x02);
    b.append(NP);
    b.append(0x01);

    flagSend = true;

    udpsocket->writeDatagram(b,b.count(),QHostAddress::Broadcast /*QHostAddress(ipTP)*/,65500);

    countMessage =2;
    timer->start(10000);
}

/*!
    \brief Слот для выполняения 3 команды

   Подключение к «ВЫХОД2» сигнала 1С с внешнего источника (имитатора)\n
   Здесь младший байт не имеет значения\n
   Младший байт: 00.\n
   Порт отправки: 65500

    \author Sergey Smoglyuk
    \version 1.0
    \date Август 2018 года
    \warning Данная функция выполняется в отдельном потоке
*/
void TP8::slot_comand3_Connect_Vx2_1C(int _index)
{

    qDebug() << "slot_comand3_Connect_Vx2_1C \t index: " << _index;
    index = _index;

    b.clear();

    b.append(0x03);
    b.append('\0');
    b.append(0x01);

    flagSend = true;

    udpsocket->writeDatagram(b,b.count(),QHostAddress::Broadcast /*QHostAddress(ipTP)*/,65500);

    countMessage =3;
    timer->start(10000);
}

/*!
    \brief Слот для выполняения 4 команды
    \param[in] char NP байт номера навигационного приемника в пульте ТП8 к которому применить команду

   Подключение к «ВЫХОД1» входа НП1-НП8 для проверки уровня напряжения питания МШУ\n
   Здесь младший байт — это номер НП с которого осуществляется измерение напряжения питания МШУ на выходе «ВЫХОД1»\n
   Младший байт: 01-08.\n
   Порт отправки: 65500
    \author Sergey Smoglyuk
    \version 1.0
    \date Август 2018 года
    \warning Данная функция выполняется в отдельном потоке
*/
void TP8::slot_comand4_Connect_Vx1_MShY(char NP,int _index)
{
    index = _index;

    b.clear();

    b.append(0x04);
    b.append(NP);
    b.append(0x01);

    flagSend = true;

    udpsocket->writeDatagram(b,b.count(),QHostAddress::Broadcast /*QHostAddress(ipTP)*/,65500);

    countMessage =4;
    timer->start(10000);
}

/*!
    \brief Слот для выполняения 5 команды
    \param[in] char NP байт номера навигационного приемника в пульте ТП8 к которому применить команду

   Подключение внешнего источника 10МГц к НП1-НП8\n
   Здесь младший байт — это номер НП к которому подключается сигнал 10 МГц с внешнего источника\n
   Младший байт: 01-08.\n
   Порт отправки: 65500
    \author Sergey Smoglyuk
    \version 1.0
    \date Август 2018 года
    \warning Данная функция выполняется в отдельном потоке
*/
void TP8::slot_comand5_Connect_10MG(char NP,int _index)
{
    index = _index;

    b.clear();

    b.append(0x05);
    b.append(NP);
    b.append(0x01);

    flagSend = true;

    udpsocket->writeDatagram(b,b.count(),QHostAddress::Broadcast /*QHostAddress(ipTP)*/,65500);

    countMessage =5;
    timer->start(10000);
}

/*!
    \brief Слот для выполняения 6 команды

   Подключение к «ВЫХОД2» внешнего источника 10МГц (для контроля уровня сигнала внешнего источника 10 МГц)\n
   Здесь младший байт не имеет значения.\n
   Младший байт: 00.\n
   Порт отправки: 65500
    \author Sergey Smoglyuk
    \version 1.0
    \date Август 2018 года
    \warning Данная функция выполняется в отдельном потоке
*/
void TP8::slot_comand6_Connect_Vx2_10_ext(int _index)
{
    index = _index;
    b.clear();

    b.append(0x06);
    b.append('\0');
    b.append(0x01);

    flagSend = true;

    udpsocket->writeDatagram(b,b.count(),QHostAddress::Broadcast /*QHostAddress(ipTP)*/,65500);

    countMessage =6;
    timer->start(10000);
}

/*!
    \brief Слот для выполняения 7 команды
    \param[in] char zatyx байт затухания

   Установка уровня затухания внешнего источника 10 МГц в дБ (0-38,75 шаг 0,25).\n
   Здесь младший байт — это номер от 0 до 155 соответствующий уровню затухания от 0 до 38,75дБ с шагом 0,25. Эта величина определяется по формуле Х/0,25, где Х величина затухания в дБ с шагом 0,25дБ (На пример 01- затухание 0,25дБ, 02- затухание 0,5дБ, 3С- затухание 15 дБ, 9B затухание38,75 дБ.\n
   Младший байт: 00-9B.\n
   Порт отправки: 65500
    \author Sergey Smoglyuk
    \version 1.0
    \date Август 2018 года
    \warning Данная функция выполняется в отдельном потоке
*/
void TP8::slot_comand7_Set_zatyxanie_10(char zatyx,int _index)
{
    index = _index;
    b.clear();

    b.append(0x07);
    b.append(zatyx);
    b.append(0x01);

    flagSend = true;

    udpsocket->writeDatagram(b,b.count(),QHostAddress::Broadcast /*QHostAddress(ipTP)*/,65500);

    countMessage =7;
    timer->start(10000);
}

/*!
    \brief Слот для выполняения 8 команды
    \param[in] char NP байт номера навигационного приемника в пульте ТП8 к которому применить команду

   Подключение внутреннего источника 10МГц к НП1-НП8.\n
   Здесь младший байт — это номер НП к которому подключается сигнал 10 МГц с внешнего источника.\n
   Младший байт: 01-08\n
   Порт отправки: 65500
    \author Sergey Smoglyuk
    \version 1.0
    \date Август 2018 года
    \warning Данная функция выполняется в отдельном потоке
*/
void TP8::slot_comand8_Connect_10_in(char NP,int _index)
{
    index = _index;

    b.clear();

    b.append(0x08);
    b.append(NP);
    b.append(0x01);

    flagSend = true;

    udpsocket->writeDatagram(b,b.count(),QHostAddress::Broadcast /*QHostAddress(ipTP)*/,65500);

    countMessage =8;
    timer->start(10000);
}

void TP8::slot_clear()
{
    b.clear();

    b.append(static_cast<char>(0xfb));
    b.append('\0');
    b.append(0x01);

    flagSend = true;

    udpsocket->writeDatagram(b,b.count(),QHostAddress::Broadcast /*QHostAddress(ipTP)*/,65500);

    countMessage =9;
    timer->start(60000);
}

/*!
    \brief Функция инициализации объекта ТП8 в новом потоке

    Данная функция выполняется в новом потке и находит все доступные Ip адреса на момент запуска программы.\n
    Открывает UDP сокет для чтения на 65510 порте.\n
    Стандартный IP пульта: 10.12.0.166.\n
    Устанавливает index = 1.

    \warning Данная функция выполняется в отдельном потоке
    \author Sergey Smoglyuk
    \version 1.0
    \date Август 2018 года
*/
void TP8::process_start()
{

    QList<QHostAddress> addr = QNetworkInterface::allAddresses();

    if(addr.count() > 3)
    {
        locIP = addr[addr.count()-4].toString();
    }
    else
    {
      locIP = addr[0].toString();
    }


    udpsocket=new QUdpSocket(nullptr); //соккет для чтения IP
    udpsocket->bind(QHostAddress(locIP),65510);//65510

    connect(udpsocket, &QUdpSocket::readyRead,
            this, &TP8::readPendingDatagrams);

    //connect(this, &TP8::signal_finished, udpsocket, &QUdpSocket::close);
    connect(this, &TP8::signal_finished, udpsocket, &QUdpSocket::deleteLater);

    ipTP = "10.12.0.172";

    index = 1;
    flagSend = false;

    timer = new QTimer();

    connect(timer,&QTimer::timeout,this,&TP8::resend);
}

void TP8::resend()
{
    replay++;

    if(replay < 30)
    {
        qDebug() << "Repead TP SEND";
        flagSend = true;
        udpsocket->writeDatagram(b,b.count(),QHostAddress::Broadcast /*QHostAddress(ipTP)*/,65500);
    }
    else
    {
        qDebug() << "ERROR TP SEND";
    }

}

void TP8::stop_timer()
{
    countMessage =0;
    timer->stop();

}

