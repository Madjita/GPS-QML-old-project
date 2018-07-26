#ifndef RELIZPROVERKATIMER_H
#define RELIZPROVERKATIMER_H

#include <QObject>

#include <gsgmodel.h>
#include <n6700model.h>
#include <portmodel.h>
#include <QSemaphore>

#include <QDateTime>

#include <QSemaphore>

#include <relizproverka.h>

class relizproverkaTimer : public QObject
{
    Q_OBJECT
public:
    explicit relizproverkaTimer(int indexGet,N6700Model* n6700,
                                gsgModel* gsgModelGet,PortModel* port,RelizProverka*,QObject *parent = 0);


     int index;

    gsgModel* gsg;
    N6700Model* n6700;

    PortModel* port;

    QString power;

    QTimer* timer;


    QTimer* timer_seitings;

    QTimer* timer_liters;

    QSemaphore* sem;
    QMutex* mut;
    QWaitCondition lol;
    RelizProverka* rez1;

    int sec =0;
    int minut=0;


    bool flag_seitings;

signals:
    void startTimer1(int);
    void stopTimer1();

    void startTimer_liters(int);
    void stopTimer_liters();
    void startTimer_Os(int);

    void startTimer_seitings(int);
    void stopTimer_seitings();

    void signal_Tick(QString);

    void signal_GoodQML(bool ok);

    /////////////////////////////////////////////////////
    void signal_GetMrk_OT();
    void signal_GetMrk_liters(int);
    void signal_GetMrk_liters_2(int);

public slots:
    void process_start_timer();
    void process_start_timer_liters();
    void process_start_timer_seitings();
    void time();
    void time_liters();
    void time_seitings();
    void Work();
    void Work_liters();
    void Work_Os();

    void slot_Power(QString);

    void slot_EndBlock();
    void slot_EndBlock_5_proverka();
};

#endif // RELIZPROVERKATIMER_H
