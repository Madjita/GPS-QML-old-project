#include "tp8.h"

TP8::TP8(QObject *parent) : QObject(parent)
{

    QList<QHostAddress> addr = QNetworkInterface::allAddresses();

    locIP = addr[0].toString();

    // locIP = addr.first().toString();

    udpsocket=new QUdpSocket();//соккет для чтения IP

    udpsocket->bind(QHostAddress(locIP),65510);

    connect(udpsocket, SIGNAL(readyRead()),this, SLOT(readPendingDatagrams()));

    ipTP = "10.12.0.166";
}

QByteArray perevod( QByteArray datagram)
{
    char tmp = datagram[11];
    datagram[11] = datagram[12];
    datagram[12] = tmp;

    tmp = datagram[13];
    datagram[13] = datagram[14];
    datagram[14] = tmp;


    return datagram;


}


void TP8::readPendingDatagrams()
{

    while (udpsocket->hasPendingDatagrams())
    {


        QByteArray datagram;



        datagram.resize(udpsocket->pendingDatagramSize());
        QHostAddress* sender = new QHostAddress();
        quint16* senderPort = new quint16();

        udpsocket->readDatagram(datagram.data(), datagram.size(),sender, senderPort);


        switch (datagram[0])
        {
        case 0x1C: {
            datagram = perevod(datagram);
            otvet_first = (find*)datagram.data();

            //запишим найденный IP в переменную (на всякий случай)
            ipTP = QString::number(otvet_first->ipTP[0]&0xFF) + "." +
                    QString::number(otvet_first->ipTP[1]&0xFF) + "." +
                    QString::number(otvet_first->ipTP[2]&0xFF) + "." +
                    QString::number(otvet_first->ipTP[3]&0xFF);








            listTP.append(*otvet_first);

            QByteArray byte_ip;

            QStringList locIP_split = locIP.split('.');

            for(int i=0;i < locIP_split.count();i++)
            {
                byte_ip.append(locIP_split[i].toInt());

            }

            bool flag = false;


            if(otvet_first->mod == 0x09 )
            {
                qDebug () << "Пульт свободен . Clear Tp8";
                break;
            }
            else
            {
                qDebug () << "Пульт занят . Close Tp8";


                        for(int i=0;i < byte_ip.count();i++)
                        {
                            if(otvet_first->ipServer[i] == byte_ip[i])
                            {
                                flag = true;
                            }
                            else
                            {
                                flag = false;
                            }
                        }



            }


            if(flag)
            {
                emit signal_Ok(true);
            }
            else
            {
                emit signal_Ok(false);
            }

            break;
        }
        case 0x01: {
            otvet = (Otvet*)datagram.data();

            break;
        }

        case 0x02: {

            if(datagram[2] == 0x02)
            {
                qDebug () << "Команда 0x02 принята";
                otvet = (Otvet*)datagram.data();
            }
            else
            {
                if(datagram[2] == 0x03)
                {
                    qDebug () << "Команда 0x02 выполнена";

                }
            }


            break;
        }
        case 0x03: {

            if(datagram[2] == 0x02)
            {
                qDebug () << "Команда 0x03 принята";
                otvet = (Otvet*)datagram.data();
            }
            else
            {
                if(datagram[2] == 0x03)
                {
                    qDebug () << "Команда 0x03 выполнена";

                }
            }


            break;
        }
        case 0x04: {
            if(datagram[2] == 0x02)
            {
                qDebug () << "Команда 0x04 принята";
                otvet = (Otvet*)datagram.data();
            }
            else
            {
                if(datagram[2] == 0x03)
                {
                    qDebug () << "Команда 0x04 выполнена";

                }
            }

            break;
        }
        case 0x05: {
            if(datagram[2] == 0x02)
            {
                qDebug () << "Команда 0x05 принята";
                otvet = (Otvet*)datagram.data();
            }
            else
            {
                if(datagram[2] == 0x03)
                {
                    qDebug () << "Команда 0x05 выполнена";

                }
            }

            break;
        }
        case 0x06: {
            if(datagram[2] == 0x02)
            {
                qDebug () << "Команда 0x06 принята";
                otvet = (Otvet*)datagram.data();
            }
            else
            {
                if(datagram[2] == 0x03)
                {
                    qDebug () << "Команда 0x06 выполнена";

                }
            }

            break;
        }
        case 0x07: {
            if(datagram[2] == 0x02)
            {
                qDebug () << "Команда 0x07 принята";
                otvet = (Otvet*)datagram.data();
            }
            else
            {
                if(datagram[2] == 0x03)
                {
                    qDebug () << "Команда 0x07 выполнена";

                }
            }

            break;
        }
        case 0x08: {
            if(datagram[2] == 0x02)
            {
                qDebug () << "Команда 0x08 принята";
                otvet = (Otvet*)datagram.data();
            }
            else
            {
                if(datagram[2] == 0x03)
                {
                    qDebug () << "Команда 0x08 выполнена";

                }
            }

            break;
        }


        }


    }

}

void TP8::slot_findTp()
{
    listTP.clear();

    QByteArray b;
    QByteArray byte_ip;

    QStringList locIP_split;


    locIP_split = locIP.split('.');


    for(int i=0;i < locIP_split.count();i++)
    {
       byte_ip.append(locIP_split[i].toInt());
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
    b.append(0xFF);
    b.append(0xDC);
    b.append(0xFF);
    b.append(0xE6);






    udpsocket->writeDatagram(b,b.count(),QHostAddress::Broadcast,6683);
}

void TP8::slot_ConnectTp(QString ip)
{
    listTP.clear();

    ipTP = ip;

    QStringList locIP_split;
    QByteArray b;
    QByteArray byte_ip;



    locIP_split = locIP.split('.');


    for(int i=0;i < locIP_split.count();i++)
    {
       byte_ip.append(locIP_split[i].toInt());
    }


    b.append(0x1C);
    b.append(0x1C);
    b.append(0x03);
    //Ip
    for(int i=0;i < locIP_split.count();i++)
    {
        b.append(byte_ip[i]);
    }

    b.append(0xFF);
    b.append(0xDC);
    b.append(0xFF);
    b.append(0xE6);

    udpsocket->writeDatagram(b,b.count(),QHostAddress(ipTP),6683);

}

void TP8::slot_comand1_Connect_Vx2_10(char NP)
{
    QByteArray b;

    b.append(0x01);
    b.append(NP);
    b.append(0x01);

    udpsocket->writeDatagram(b,b.count(),QHostAddress(ipTP),65500);
}

void TP8::slot_comand2_Connect_Vx1_1C(char NP)
{
    QByteArray b;

    b.append(0x02);
    b.append(NP);
    b.append(0x01);

    udpsocket->writeDatagram(b,b.count(),QHostAddress(ipTP),65500);
}

void TP8::slot_comand3_Connect_Vx2_1C()
{
    QByteArray b;

    b.append(0x03);
    b.append('\0');
    b.append(0x01);

    udpsocket->writeDatagram(b,b.count(),QHostAddress(ipTP),65500);
}

void TP8::slot_comand4_Connect_Vx1_MShY(char NP)
{
    QByteArray b;

    b.append(0x04);
    b.append(NP);
    b.append(0x01);

    udpsocket->writeDatagram(b,b.count(),QHostAddress(ipTP),65500);
}

void TP8::slot_comand5_Connect_10MG(char NP)
{
    QByteArray b;

    b.append(0x05);
    b.append(NP);
    b.append(0x01);

    udpsocket->writeDatagram(b,b.count(),QHostAddress(ipTP),65500);
}

void TP8::slot_comand6_Connect_Vx2_10_ext()
{
    QByteArray b;

    b.append(0x06);
    b.append('\0');
    b.append(0x01);

    udpsocket->writeDatagram(b,b.count(),QHostAddress(ipTP),65500);
}

void TP8::slot_comand7_Set_zatyxanie_10(char zatyx)
{
    QByteArray b;


    double z = zatyx*0.25;

    char s = (char)z;


    b.append(0x07);
   // b.append(QString::number(z));
    b.append(s);
    b.append(0x01);

    udpsocket->writeDatagram(b,b.count(),QHostAddress(ipTP),65500);
}

void TP8::slot_comand8_Connect_10_in(char NP)
{
    QByteArray b;

    b.append(0x06);
    b.append(NP);
    b.append(0x01);

    udpsocket->writeDatagram(b,b.count(),QHostAddress(ipTP),65500);
}
