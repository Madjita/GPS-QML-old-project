#ifndef TP8_H
#define TP8_H

#include <QObject>

#include <QtNetwork>

#pragma pack(push,1)
struct find
{
    char bigBayt;
    char smolBayt;
    char mod;
    char ipTP[4];
    char ipServer[4];
    short int portServer;
    short int portTp;
    char nameTP[20];
    char NumberTP[6];
    char Version[6];

};
#pragma pack(pop)


#pragma pack(push, 1)
struct Otvet
{
    char KodSend;
    char Argyment;
    char mod;

};
#pragma pack(pop)

class TP8 : public QObject
{
    Q_OBJECT

public:
    explicit TP8(QObject *parent = 0);


  //  Q_PROPERTY(QList<QObject*> listDevice READ listDevice NOTIFY ListDeviceChanged)



    QUdpSocket *udpsocket;

    find *otvet_first;

    Otvet *otvet;

    QList<find> listTP;

    QString ipTP;
    QString locIP;

signals:
    void signal_Ok(bool good);

public slots:
     void readPendingDatagrams();

     void slot_findTp();

     void slot_ConnectTp(QString ip);

     //Подключение к «ВЫХОД2» сигнала 10МГц с НП1-НП8
     void slot_comand1_Connect_Vx2_10(char NP);

     //Подключение к «ВЫХОД1» сигнала 1С с НП1-НП8
     void slot_comand2_Connect_Vx1_1C(char NP);

     //Подключение к «ВЫХОД2» сигнала 1С с внешнего источника (имитатора)
     void slot_comand3_Connect_Vx2_1C();

     //Подключение к «ВЫХОД1» входа НП1-НП8 для проверки уровня напряжения питания МШУ
     void slot_comand4_Connect_Vx1_MShY(char NP);

     //Подключение внешнего источника 10МГц к НП1-НП8
     void slot_comand5_Connect_10MG(char NP);

     //Подключение к «ВЫХОД2» внешнего источника 10МГц (для контроля уровня сигнала внешнего источника 10 МГц)
     void slot_comand6_Connect_Vx2_10_ext();

     //Установка уровня затухания внешнего источника 10 МГц в дБ (0-38,75 шаг 0,25)
     void slot_comand7_Set_zatyxanie_10(char zatyx);

     void slot_comand8_Connect_10_in(char NP);

};

#endif // TP8_H
