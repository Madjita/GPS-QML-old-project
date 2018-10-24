#ifndef OSUILOGRAF_H
#define OSUILOGRAF_H


#include <visa.h>
#include <visatype.h>

#include <QString>

#include <QDebug>
#include <QTimer>

#include <QThread>

class Osuilograf : public QObject
{
    Q_OBJECT
public:
    explicit Osuilograf(QObject *parent = nullptr);
    ~Osuilograf();

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

     QTimer* timer_Imitator_Data_Amplituda;

    QString ManufacturerDefault;


    bool flag_work;

    QString dataPower;

    bool flag_change_seitings;
    bool flag_change_seitings_3;


public slots:
    bool connectDevice(QString ip);
    void DisConnect();
    void Work();
    void Work_Amplituda();
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

//////////////////////////////////////////////////
// Относится к Осцилографу

    void setIMPedance(QString,int);
    void setSCALe(QString,int);
    void setRANGe(QString,int);
    void setTIMebaseSCALe(QString);
    void setTIMebaseREFerence(QString);
    void setTRIGgerEDGESOURce(QString);
    void setTRIGgerEDGESLOPe(QString);
    void setTRIGgerEDGELEVel(QString);
    void setDelay(QString,QString);
    void setAmplitude(QString);
    void setVMAX(QString);
    void setTRIGgerSWEep(QString);

    void setStartSeitings();
    void setStartSeitings2();
    void setStartSeitings3();


    QString getDelay();
    QString getAmplitude();
    QString getVMAX(QString canal);
    QString getVRMS(QString canal);
//////////////////////////////////////////////////
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
    void TimerStop_Amplityda();
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

    void startTimer_Amplituda(int);

    void signal_QMLStart();

    void signal_finished();

};

#endif // OSUILOGRAF_H
