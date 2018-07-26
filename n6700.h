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


    //Функции считывания параметров
    void getName();
    QVector<QString> getSetVolt(QString canal);
    QString getEror();
    QVector<QString> getMeasureVolt(QString canal);


    explicit N6700(QObject *parent = 0);

    ~N6700()
    {
        viClose(vi);
    }


    bool flag_work;


public slots:
    bool Connect(QString ip);
    void DisConnect();
    void getMeasureVoltALL();
    void getMeasureCURRentALL();
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
};

#endif // N6700_H
