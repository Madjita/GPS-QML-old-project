#ifndef GSGMODEL_H
#define GSGMODEL_H

#include <QObject>
#include <gsg.h>
#include <QtCore>

#include <bdata.h>

#include <osuilografmodel.h>
#include <tp8.h>

class gsgModel : public QObject
{
    Q_OBJECT

public:
    explicit gsgModel(QObject *parent = 0);

     GSG* gsg;

     int R;
     int G;

      Q_PROPERTY(QString name READ name);

      const QString & name() const;


      BData* BD;
      void SetBD(BData* bd);

      //////////
      //Подключаем Пульт и Осцилограф для 6 проверки.

      OsuilografModel * os;

      TP8* tp;

      void Sed_TP_OS(OsuilografModel*,TP8*);


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



public slots:
    void slot_connectDevice(QString); // Слот для отправки сигнала от QML до gsg
    bool slot_connectOk(); // Слот для принятия сигнала от gsg, для отправки в QML
    void slot_DisConnect(); // Слот для отправки сигнала от QML до gsg, разъединение
    void slot_Work(); // Слот для отправки сигнала от QML до gsg, работа (старт таймера)
    void slot_StartTimer();
    void slot_StopTimer();

    //Слоты от gsg
    void slot_ScenNumberIDSpytnik(QString);  // Слот для принятия сигнала от gsg, для отправки в QML, номер спутника

    QString slot_R();
    QString slot_G();

    void setSIGNALtype(QString SIGNALtype); // Слот для принятия сигнала от gsg, для отправки в QML для старта имитатора

    void getCONTrol();

    ///////////////////////
    void slot_Gen_setCONTrol(QString SIGNALtype);

    void slot_Gen_setSATid(QString SIGNALtype);


};

#endif // GSGMODEL_H
