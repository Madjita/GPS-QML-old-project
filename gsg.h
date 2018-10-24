#ifndef GSG_H
#define GSG_H

#include <visa.h>
#include <visatype.h>

#include <QString>

#include <QDebug>
#include <QTimer>

#include <QThread>

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
    QTimer* timer_Imitator_DataGEN;

    QString ManufacturerDefault;

    //Mode mode;

    explicit GSG(QObject *parent = nullptr);

    ~GSG();

    bool flag_work;

    QString dataPower;

    bool flagStart = false;

public slots:
    bool connectDevice(QString ip);
    void DisConnect();
    void Work();
    void WorkGen();
    void endWork();



    void getScenDataTime();

    //Функции для устанвоки параметров
    void setSIGNALtype(QString SIGNALtype);
    void setCONTrol(QString command);
    void setGenCONTrol(QString command);
    void setGenSATid(QString Np_liter);
    void setGenSIGNALtype(QString type_signal);
    void setGenREFPOWer(int power);

    void setPOWer(int id,int power,QString freqband);
    void setPOWer(QString id,int power,QString freqband);
    void setPOWerALL(int power,QString freqband);
    void setPPSOUTput(int on);


    //Функции для считывания параметров
    void getName();

    QString getCONTrol();
    QString getGenCONTrol();
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
    QString getGenSATid();

    void slot_StartTimer();


    void process_start();

signals:
    void UpdateScenDataTime(QString);
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
    void TimerStop();

    void startTimerGen(int);
    void TimerStopGen();

    void signal_QMLStart();

    void signal_finished();


};

#endif // GSG_H
