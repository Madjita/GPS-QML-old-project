#ifndef N6700_H
#define N6700_H

#include <visa.h>
#include <visatype.h>

#include <QString>

#include <QDebug>
#include <QTimer>

#include <QMutex>
#include <QThread>

class N6700  : public QObject
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

    //Таймеры
    QTimer* timer_IstP1_Measure;

    QString v_1,v_2,v_3,v_4;

    QString i_1,i_2,i_3,i_4;

    QMutex mut;


    //Функции считывания параметров
    void getName();
    QVector<QString> getSetVolt(QString canal);
    QString getEror();
    QVector<QString> getMeasureVolt(QString canal);

    char buffEror[100];
    QVector<QString> VectorMeasureVoltALL;
    QVector<QString> VectorMeasureCURRentALL;

    explicit N6700(QObject *parent = nullptr);
    ~N6700();


    bool flag_work;


public slots:
    bool Connect(QString ip);
    void DisConnect();
    void getMeasureVoltALL();
    void getMeasureCURRentALL();
    QVector<QString> getMeasureCURRentALL_find();
    void Work();
    void startProverka();
    void endWork();
    void slot_StartTimer();

    void process_start();

    //Функции установки параметров
    void setOutput(QString canal, bool OnOff);
    void setVolt(QString canal, QString V);
    void setCurrent(QString canal, QString I);


    QString getOutput(QString canal);


signals:
     void getMeasureVoltSignal (QVector<QString>);
     void getMeasureCURRentSignal (QVector<QString>);
     void TimerStop();

     void connectOk();

     void startTimer(int);

     void signal_finished();

};

#endif // N6700_H
