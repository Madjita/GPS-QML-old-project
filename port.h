#ifndef PORT_H
#define PORT_H

#include <basetsd.h>
#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/qserialport.h>
#include <QTimer>

typedef unsigned char byte;

struct Settings
{
    QString name;
    qint32 baudRate;
    QSerialPort::DataBits dataBits;
    QSerialPort::Parity parity;
    QSerialPort::StopBits stopBits;
    QSerialPort::FlowControl flowControl;
};


//Структура с данными
//#pragma pack(push, 1)
//struct frameExample
//{

//    byte NkaLit[24];  // Номер Литиры
//    byte NkaNum[24];  // Номер НКА
//    byte Ka1Mode[24]; // Режим КА1
////    byte Ka2Mode[24]; // Режим КА2
//    __int16 A1L1[24]; // Амплиуда

//};
//#pragma pack(pop)


#pragma pack(push, 1)
struct amplituda
{
    __int16 A1L1[24]; // Амплиуда //__int16
};
#pragma pack(pop)

//Структура с данными
//#pragma pack(push, 1)
struct frameExample
{

    byte NkaLit[24];  // Номер Литиры
    byte NkaNum[24];  // Номер НКА
    byte Ka1Mode[24]; // Режим КА1
    //    byte Ka2Mode[24]; // Режим КА2
    amplituda* A1L1;// Амплиуда

};
//#pragma pack(pop)

Q_DECLARE_METATYPE(frameExample);

class Port : public QObject
{
    Q_OBJECT

public:
    explicit Port(QObject *parent = 0);

    //Инициализация таймеров
    QTimer* timer_MRK_Data;

    int CountFindGPS;
    int CountFindGLONASS;
    int CountFindALL;

    bool flag_start_MRK;
    bool flag_end_MRK;

    QByteArray dataBuild;

    bool flag_GetMrk_liters = false;


    QStringList* listSP;
    QStringList* listSP_Amplitude;
    QStringList* listSP_Name;

    QByteArray data;
    QByteArray lol;

    int position_liters;
    int position_number;
    int position_Mode;
    int position_A;


public:
    ~Port();

    void RazborFrame(frameExample);

public:
    QSerialPort* thisPort;
    Settings SettingsPort;
    frameExample * container;
    bool flag;
    bool flag_work;

signals:
    void finihed_Port(); // Сигнал закртытия класса
    void error_(QString err); // Сигнал ошибок порта
    void outPort(QString data); // Сигнал вывода полученных данных
    void StopTimer();
    void outMRKdata(frameExample);

    void outPortOpen(QString);

    void startPort();
    void startWork();
    void endWork();

    void startTimerMrk(int);
    void stopTimerMrk();
    void UpdateCountFind(int,int,int);

    void signal_GoTORelizproverka(QStringList,QStringList,QStringList);

    void writeData(QByteArray);

public slots:
    void DisconnectPort(); // Слот отключения порта
    void ConnectPort(); // Слот подключения порта
    void Write_Setting_Port(QString name, int baudrate, int DataBits, int parity, int StopBits, int FlowControl); // Слот занесение настроек порта в класс
    void process_Port(); // Тело
    void WriteToPort(QByteArray data); // Слот отправки данных в порт
    void GetMrk();
    void GetMrk_OT();
    void GetMrk_liters(int);
    void GetMrk_liters_2(int);
    void END();
    void Work();

    void process_start();

private slots:
    void handleError(QSerialPort::SerialPortError error); // Слот обработки ошибок
    void ReadInProt(); //Слот чтения из порта по ReadyRead
public:
};

#endif // PORT_H
