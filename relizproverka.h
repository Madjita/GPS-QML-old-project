#ifndef RELIZPROVERKA_H
#define RELIZPROVERKA_H

#include <QObject>

#include <gsgmodel.h>
#include <n6700model.h>
#include <portmodel.h>

#include <tp8model.h>
#include <osuilografmodel.h>


#include <QDateTime>

#include <QSemaphore>


class RelizProverka : public QObject
{
    Q_OBJECT
public:
    explicit RelizProverka(int indexGet,int _indexList,N6700Model* n6700,
                           gsgModel* gsgModelGet,OsuilografModel* _os, tp8Model* _tp,PortModel* port,QString GetName, QObject *parent = nullptr);
    ~RelizProverka();

    //Автоматическая проверка
    bool flag_auto;

    //Флаг запуска одной првоерки
    bool flag_auto_oneProverka;

    //Режим нормально,холода,тепла
    int autoMode_NY_cold_hart;

    //Имя
    QString Name;
    QString Id;
    QString IdLink; //Id устройства из таблицы Link_Resulr для внесения в БД в таблицу Result
    QDateTime start;
    QDateTime stay;
    QDateTime stay_liter;

    //БД
   // BData* BD = new BData();


    int index;

    gsgModel* gsg;
    N6700Model* n6700;
    OsuilografModel* os;
    tp8Model* tp;

    PortModel* port;

//    QTimer* timer;
//    QTimer* timer2;

    int minut;
    int sec;

    int count;

    QSemaphore* sem;
    QMutex* mut;
    QWaitCondition lol;

    int countProverka;


    int proverka2_part;


    int proverka5_part;

    bool Good;

    bool job;

    int liter;

    int indexList;

    QVector<bool> list_flag_good_2;
    QVector<bool> list_flag_good_3;
    QVector<bool> list_flag_good_5;

    bool flag_start_mrk;
    bool flag_good_1;
    bool flag_good_2;
    bool flag_good_3;
    bool flag_good_4;
    bool flag_good_5;
    bool flag_good_6;
    bool flag_good_7;
    bool flag_good_8;
    bool flag_good_9;
    bool flag_good_10;


    bool flag_300MGH_6Proverka;


    //Функция установки литеры IdLink
    void SetIdLink(QString);

     int Count_NP;


signals:
    void Work_signal();
    void startTimer();
    void endWork_signal(int);
    void signal_GoodQML(int);
    void signal_BadQML(int);
    void signal_TimerQML(int min,int sec);



    void signal_StartGSG();
    void signal_StopGSG();

    void signal_StartN6700();
    void signal_StopN6700();

    void signal_StartPort();
    void signal_StopPort();

    void signal_EndProverka(int,int);

    void signal_slot_StartProverka_Os(int,int position);



//    //////////////////////////////////////////////
//    void startTimer1(int msec);
//    void startTimer2(int msec);

//    void stopTimer1();
//    void stopTimer2();

    void startWork();
    void startWork_liters();
    void startWork_Os();

    void signal_StartProverka_1();
    void signal_StartProverka_2();
    void signal_StartProverka_3();
    void signal_StartProverka_4();
    void signal_StartProverka_5();
    void signal_StartProverka_6();
    void signal_StartProverka_7();
    void signal_StartProverka_8();
    void signal_StartProverka_9();
    void signal_StartProverka_10();

    void setOutput(QString,bool);
    void setVolt(QString,QString);


    void signal_IndexProverka(QString);

    /////////////////////////////////////
    void signal_startGen();

    void signal_startImitator();

    void signal_StartProverkaIndex(QString namberProverkaStart);


    void signal_setTP_vneshnRR_10MGz();

    void signal_beginOSProv(int index);
    void signal_beginOSProv_7(int index);

    void signal_endOSProv(int index);

    void signal_start_10Proverka(int index);


    void signal_finished();
    ///BD
    void sendZaprozToBD(QString);

    void signal_qmlText(QString);

public slots:
    void process_start();

//    void process_start_timer();
//    void process_start_timer2();

    void waitMRKLoad();

    // Проверки
     void proverka_rabotosposobnosti_NP_ID_1();

     void proverka_rabotosposobnosti_Ponijennoe_naprRjenie_NP_ID_2();

     void proverka_rabotosposobnosti_Ponijennoe_naprRjenie_NP_ID_3();

     void proverka_rabotosposobnosti_Ponijennoe_naprRjenie_NP_ID_4();

     void proverka_rabotosposobnosti_Ponijennoe_naprRjenie_NP_ID_5();

     void proverka_rabotosposobnosti_Ponijennoe_naprRjenie_NP_ID_6();

     void proverka_rabotosposobnosti_Ponijennoe_naprRjenie_NP_ID_7();

     void proverka_rabotosposobnosti_Ponijennoe_naprRjenie_NP_ID_8();

     void proverka_rabotosposobnosti_Ponijennoe_naprRjenie_NP_ID_9();

     void proverka_rabotosposobnosti_Ponijennoe_naprRjenie_NP_ID_10();

    void Work(bool auto_test, int proverka);



};

#endif // RELIZPROVERKA_H
