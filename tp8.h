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


/*!
    \brief Класс пульта ТП8 для навигационных плат НП101
    \author Sergey Smoglyuk
    \version 1.0
    \date Август 2018 года
    \warning Данный класс создан для управления технологическим пультом ТП8

    Данный класс имеет набор свойств и методов для управления пультом
 ТП8 в котором находится 8 навигационных плат НП101, который отнаследован от QObject

*/
class TP8 : public QObject
{
    Q_OBJECT

public:
    explicit TP8(QObject *parent = nullptr);
    ~TP8();

    int index;

    //  Q_PROPERTY(QList<QObject*> listDevice READ listDevice NOTIFY ListDeviceChanged)

    bool flagClear;
    bool flagSend;
    bool flagConnect;

    QUdpSocket *udpsocket;

    find *otvet_first;

    Otvet *otvet;

    QList<find> listTP;

    QString ipTP;
    QString locIP;

    QByteArray b;
    QByteArray perevod( QByteArray datagram);

    int countMessage =0;
    int replay = 0;
    QTimer* timer;

signals:
    void signal_Ok(bool good);


    void signal_comanda_ok(int comand,int index);
    void signal_comandaClear_ok();

    void connectTp(QString ip);

    void signal_finished();

public slots:
    void readPendingDatagrams();

    void slot_findTp();

    void slot_ConnectTp(QString ip);

    //Подключение к «ВЫХОД2» сигнала 10МГц с НП1-НП8
    void slot_comand1_Connect_Vx2_10(char NP,int _index);

    //Подключение к «ВЫХОД1» сигнала 1С с НП1-НП8
    void slot_comand2_Connect_Vx1_1C(char NP,int index);

    //Подключение к «ВЫХОД2» сигнала 1С с внешнего источника (имитатора)
    void slot_comand3_Connect_Vx2_1C(int _index);

    //Подключение к «ВЫХОД1» входа НП1-НП8 для проверки уровня напряжения питания МШУ
    void slot_comand4_Connect_Vx1_MShY(char NP,int _index);

    //Подключение внешнего источника 10МГц к НП1-НП8
    void slot_comand5_Connect_10MG(char NP,int _index);

    //Подключение к «ВЫХОД2» внешнего источника 10МГц (для контроля уровня сигнала внешнего источника 10 МГц)
    void slot_comand6_Connect_Vx2_10_ext(int _index);

    //Установка уровня затухания внешнего источника 10 МГц в дБ (0-38,75 шаг 0,25)
    void slot_comand7_Set_zatyxanie_10(char zatyx,int _index);

    void slot_comand8_Connect_10_in(char NP,int _index);

    void slot_clear();


    void process_start();

    void resend();
    void stop_timer();




};

#endif // TP8_H
