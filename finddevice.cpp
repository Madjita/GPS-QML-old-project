#include "finddevice.h"

//const QList<QObject*> FindDevice::listDevice() const
//{

//    //    if(res.count() > 0)
//    //    {
//    //        qDebug() << "1 " <<res.value(0)->property("ip");
//    //    }
//    qDebug () << res;

//    return res;
//}


//************************************************
//Конструктор
const QList<QObject *> FindDevice::listDevice() const
{
    return res;
}

FindDevice::FindDevice(QObject *parent):
    QObject(parent)
{

    this->moveToThread(new QThread()); //Переместили класс GSG в новый поток
    connect(this->thread(),&QThread::started,this,&FindDevice::initSocket);
    this->thread()->start();

    connect(this, &FindDevice::signal_finished, this->thread(), &QThread::quit);

    //Инициализация Соектов UDP
    // initSocket();


}

FindDevice::~FindDevice()
{
//    udpSocket->close();
//    udpSocket1->close();

//    delete udpSocket;
//    delete udpSocket1;

    emit signal_finished();
}
//************************************************

//************************************************
//Инициализация Соектов UDP
void FindDevice::initSocket()
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

    qDebug () << "Init UDP";
    udpSocket = new QUdpSocket(nullptr);
    //udpSocket->bind(52824,QAbstractSocket::DefaultForPlatform);
    udpSocket->bind(QHostAddress(locIP));

    // Соединяем пришеднише в Порт (QHostAddress::LocalHost) пакеты с функцией readPendingDatagrams()
    connect(udpSocket, SIGNAL(readyRead()),
            this, SLOT(readPendingDatagrams()));


    udpSocket1 = new QUdpSocket(nullptr);
    udpSocket1->bind(QHostAddress(locIP),65511);


    // Соединяем пришеднише в Порт (8890) пакеты с функцией readPendingDatagrams1()
    connect(udpSocket1, &QUdpSocket::readyRead,
            this, &FindDevice::readPendingDatagrams1);

    connect(this,&FindDevice::signal_SendPortmapFind,this,&FindDevice::SendPortmapFind);

    connect(this, &FindDevice::signal_finished, udpSocket, &QUdpSocket::deleteLater);
    connect(this, &FindDevice::signal_finished, udpSocket1, &QUdpSocket::deleteLater);


}
//************************************************

//************************************************
//Слот работы по поиску устрйоств
void FindDevice::Work()
{
    qDebug () << "Start";

    //сигнал отчисти найденных устройств и объектов
    emit clearList();
    //сигнал добавления объекта
    emit addItem("Поиск","Найти устройства?");

    //Функция отправки запроса на поиск устройств в сети по UDP
    // SendPortmapFind();

    emit signal_SendPortmapFind();

}
//************************************************

//************************************************
// Проверка найденных Ip адресов на наличие устрйоств
void FindDevice::setListDevice(QByteArray data, QString peerName)
{

    //Инициализация переменных для подклчюения к устройству
    ViSession vi;
    int viStatus;
    ViSession defaultRM;

    char nameChar[100]; //переменная имени
    memset(nameChar,0,sizeof (nameChar));


    viStatus = viOpenDefaultRM(&defaultRM);


    //peerName.remove(peerName.indexOf("%"),peerName.count()-peerName.indexOf("%"));

    QString str  = "TCPIP0::"+peerName+"::inst0::INSTR";

    QString str2  = "TCPIP0::"+peerName+"::8888::SOCKET::GEN";


    qDebug() << peerName;


    //Подключеие к уйстройству
    viStatus = viOpen(defaultRM, const_cast<ViRsrc>(qPrintable("TCPIP0::"+peerName+"::inst0::INSTR")), VI_NULL, VI_NULL,&vi); // проверено - работает через IP


    if(viStatus<VI_SUCCESS)
    {
        qDebug() << "[Соединение c "+peerName+" не установленно(ERROR)]";

        viStatus = viOpen(defaultRM, const_cast<ViRsrc>(qPrintable("TCPIP0::"+peerName+"::8888::SOCKET::GEN")), VI_NULL, VI_NULL,&vi); // проверено - работает через IP

        if(viStatus<VI_SUCCESS)
        {
            qDebug() << "[Соединение c "+peerName+" не установленно(ERROR)]";

            emit addItem("Не найдено",peerName);

        }
        else
        {

            qDebug() << "[Соединение c "<< peerName<<" установленно(ОК)]";

            viQueryf(vi,const_cast<ViString>("*IDN?\t\n"),const_cast<ViString>("%T"),nameChar);

            qDebug() << qPrintable(nameChar);


            auto list = QString(qPrintable(nameChar)).split(",");

            QString name = list[0] + ": " + list[1] + ": " + list[2];


            emit addItem(name,peerName);

            res.append(new FindDeviceItem(name,peerName));

            emit ListDeviceChanged(res);

        }

    }
    else
    {

        qDebug() << "[Соединение c "<< peerName<<" установленно(ОК)]";

        viQueryf(vi,const_cast<ViString>("*IDN?\t\n"),const_cast<ViString>("%T"),nameChar);

        qDebug() << qPrintable(nameChar);


        auto list = QString(qPrintable(nameChar)).split(",");

        QString name = list[0] + ": " + list[1] + ": " + list[2];

        emit addItem(name,peerName);

        res.append(new FindDeviceItem(name,peerName));

        emit ListDeviceChanged(res);
    }



    viClose( vi );
}

//************************************************
void FindDevice::setListDeviceTP(QByteArray data, QString peerName)
{

    qDebug() << "[Найден пульт с  IP: "<< peerName<<" ]";

    QString name = "Пульт ТП8 IP: " + peerName;
    emit addItem(name,peerName);
    res.append(new FindDeviceItem(name,peerName));
    emit ListDeviceChanged(res);

}
//************************************************

//************************************************
//Чтение данных пришедших после запроса на поиск. от порта 111
void FindDevice::readPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(static_cast<int>(udpSocket->pendingDatagramSize()));
        QHostAddress *sender = new QHostAddress();;
        quint16 senderPort;

        udpSocket->readDatagram(datagram.data(), datagram.size(),
                                sender, &senderPort);



        qDebug() << sender->toString();

        auto ip = sender->toString().split(":").last();


        setListDevice(datagram,ip);

    }
}
//************************************************

//************************************************
//Чтение данных пришедших после запроса на поиск. от порта 8890
void FindDevice::readPendingDatagrams1()
{

    while (udpSocket1->hasPendingDatagrams())
    {


        QByteArray datagram;
        quint16* senderPort = new quint16();
        QHostAddress* sender = new QHostAddress();
        datagram.resize(static_cast<char>(udpSocket1->pendingDatagramSize()));
        udpSocket1->readDatagram(datagram.data(), datagram.size(),sender, senderPort);

        qDebug () <<  udpSocket1->localAddress();

        //   auto ip = sender->toString().split(":").last();

        datagram.remove(0,3);
        datagram.remove(4,datagram.count()-4);

        QString ip;

        for(int i=0; i < datagram.count();i++)
        {


            ip.append(QString::number(QChar(datagram[i]).unicode()));

            if(i < datagram.count()-1)
            {
                ip.append(".");
            }

        }



        setListDeviceTP(qPrintable(datagram.split(0x09).first()),ip);


        qDebug () << "Get message TP";
        qDebug () <<ip;


    }

}
//************************************************

//************************************************
//Функция отправки запроса на поиск устройств в сети по UDP
void FindDevice::SendPortmapFind()
{

    res.clear();

    emit clearRes();


    QStringList locIP_split;
    QByteArray byte_ip;


    locIP_split = locIP.split('.');


    for(int i=0;i < locIP_split.count();i++)
    {
        byte_ip.append(static_cast<char>(locIP_split[i].toInt()));
    }

    QByteArray b;
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
    b.append(static_cast<char>(0xE7));



    udpSocket1->writeDatagram(b,b.count(),QHostAddress::Broadcast,6683);


    //Инициализация массива посылки
    char ad2f[56];

    //Обнуление всей посылки
    for(int i=0; i <56;i++)
    {
        ad2f[i] = '\0';
    }
    //Cоздание посылки
    ad2f[2]=0x03;
    ad2f[3]=static_cast<char>(0xe8);
    ad2f[11]=0x02;
    ad2f[13]=0x01;
    ad2f[14]=static_cast<char>(0x86);
    ad2f[15]=static_cast<char>(0xa0);
    ad2f[19]=0x02;
    ad2f[23]=0x03;
    ad2f[41]=0x06;
    ad2f[42]=0x07;
    ad2f[43]=static_cast<char>(0xaf);
    ad2f[47]=0x01;
    ad2f[51]=0x06;

    //Отправка в порт 111 посылки для поиска устрйоств
    udpSocket->writeDatagram(ad2f,56,QHostAddress::Broadcast,111);




}

//************************************************
