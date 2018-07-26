#ifndef OSUILOGRAFMODEL_H
#define OSUILOGRAFMODEL_H

#include <QObject>
#include <QtCore>
#include <osuilograf.h>

class OsuilografModel : public QObject
{
    Q_OBJECT
public:
    explicit OsuilografModel(QObject *parent = 0);


     Osuilograf* os;

     int R;
     int G;

      Q_PROPERTY(QString name READ name);

      const QString & name() const;

signals:
    void connectDevice(QString);
    void DisConnect();
    void endWork();
    void Work();
    void startTimer();
    void stopTimer();

    //сигналы от модели до QML
    void countSpytnik_R(int);
    void countSpytnik_G(int);

    void signal_getCONTrol(QString);
    void signal_getCONTrolQML();

    void signal_Ok(bool good);

    ///////////////////////
    void signal_Gen_setCONTrol(QString);
    void signal_Gen_setSATid(QString);


    //Signal смнены настроек
    void signal_Change_Seitings();
    void signal_Change_Seitings3();

public slots:
    void slot_connectDevice(QString); // Слот для отправки сигнала от QML до Осцилографа
    bool slot_connectOk(); // Слот для принятия сигнала от Осцилографа, для отправки в QML
    void slot_DisConnect(); // Слот для отправки сигнала от QML до Осцилографа, разъединение
    void slot_Work(); // Слот для отправки сигнала от QML до Осцилографа, работа (старт таймера)
    void slot_StartTimer();
    void slot_StopTimer();

    //Слоты от gsg
    void slot_ScenNumberIDSpytnik(QString);  // Слот для принятия сигнала от Осцилографа, для отправки в QML, номер спутника

    QString slot_R();
    QString slot_G();

    void setSIGNALtype(QString SIGNALtype); // Слот для принятия сигнала от Осцилографа, для отправки в QML для старта имитатора

    void getCONTrol();

    ///////////////////////
    void slot_Gen_setCONTrol(QString SIGNALtype);

    void slot_Gen_setSATid(QString SIGNALtype);

    ////
    // Слот для смены настроек

    void slot_Change_Seitings();

    void slot_Change_Seitings3();


};

#endif // OSUILOGRAFMODEL_H
