#ifndef GSG_H
#define GSG_H

#include <visa.h>
#include <visatype.h>

#include <QString>

#include <QDebug>
#include <QTimer>

#include <QThread>



//struct Mode
//{
//    QString manufacturer;
//    QString model;
//    QString serialNumber;
//    QString firmwareLevel;
//    QString options;
//};



class GSG : public QObject
{
    Q_OBJECT

public:

    ViSession vi;
    int viStatus;
    ViRsrc viAddres;
    bool connected;

    ViSession defaultRM;

    QString name;
    QString log;

    QString NewDataSpytnik;
    QString NewDataSpytnikID;
    //Таймеры
    QTimer* timer_Imitator_Data;

    QString ManufacturerDefault;

    //Mode mode;

    explicit GSG(QObject *parent = 0);

    ~GSG()
    {
        viClose(vi);
    }

    bool flag_work;

    QString dataPower;

    bool flagStart = false;

public slots:
    bool connectDevice(QString ip);
    void DisConnect();
    void Work();
    void endWork();



    void getScenDataTime();

    //Функции для устанвоки параметров
    void setSIGNALtype(QString SIGNALtype);
    void setCONTrol(QString command);
    void setGenCONTrol(QString command);
    void setGenSATid(QString Np_liter);
    void setGenSIGNALtype(QString type_signal);
    void setPOWer(int id,int power,QString freqband);
    void setPOWer(QString id,int power,QString freqband);
    void setPOWerALL(int power,QString freqband);


    //Функции для считывания параметров
    void getName();

    QString getCONTrol();
    QString getSIGNALtype();
    QString getSIGNALtype(QString id);
    QString getSATid(int n);
    QString getFREQuency(int n);
    QString getFREQuency(QString id);
    QString getPOWer(int id,QString freqband);
    QString getPOWer(QString id);
    QString getSVmodel(int id);
    QString getSVmodel(QString id);

    QString getPosition();

    QString getNumberIDSpytnik();

    void slot_StartTimer();


    void process_start();

signals:
    void UpdateScenDataTime(QString);
    void TimerStop();
    void UpdateScenNumberIDSpytnik(QString);
    void UpdateScenPow(QString);
    void connectOk();

    void signal_getCONTrol(QString);
    void signal_getSIGNALtype(QString);
    void signal_getSATid(QString);
    void signal_getFREQuency(QString);
    void signal_getPOWer(QString);
    void signal_getSVmodel(QString);

    void startTimer(int);

    void signal_QMLStart();


};

#endif // GSG_H
