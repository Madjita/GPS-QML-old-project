#ifndef PROVERKAITEM_H
#define PROVERKAITEM_H

#include <QObject>

#include <QVariant>

#include <gsgmodel.h>
#include <n6700model.h>
#include <portmodel.h>

#include <osuilografmodel.h>
#include <tp8model.h>

#include <relizproverka.h>


#include "relizproverkatimer.h"


class proverkaItem  : public QObject
{

    Q_OBJECT

public:
    explicit proverkaItem(int index,int indexList,N6700Model* n6700, gsgModel* gsgModelGet,OsuilografModel* _os, tp8Model* _tp, PortModel *port,bool vnytrenGenGet, bool goodGet,QString GetName, QObject *parent = nullptr);
    ~proverkaItem();


    //Ток
    Q_PROPERTY(QString indexNP READ indexNP NOTIFY indexNPChanged)
    const QString & indexNP();

    //Ток
    Q_PROPERTY(QString tok READ tok NOTIFY tokChanged)
    const QString & tok() const;


    //Напряжение
    Q_PROPERTY(QString v READ v NOTIFY vChanged)
    const QString & v() const;
    void setV(const QString & v);

    //Мощнасть
    Q_PROPERTY(QString power READ power NOTIFY powerChanged)
    const QString & power() const;
    void setPower(const QString & power);

    //Сгенирированные GPS спутники с имитатора
    Q_PROPERTY(QString gpsGen READ gpsGen NOTIFY gpsGenChanged)
    const QString & gpsGen() const;
    void setGpsGen(const QString & gpsGen);

    //Сгенирированные GLONASS спутники с имитатора
    Q_PROPERTY(QString glonassGen READ glonassGen NOTIFY glonassGenChanged)
    const QString & glonassGen() const;
    void setGlonassGen(const QString & glonassGen);

    //Пойманные GPS спутники приемником
    Q_PROPERTY(QString gpsFind READ gpsFind NOTIFY gpsFindChanged)
    const QString & gpsFind() const;
    void setGpsFind(const QString & gpsFind);

    //Пойманные GLONASS спутники приемником
    Q_PROPERTY(QString glonassFind READ glonassFind NOTIFY glonassFindChanged)
    const QString & glonassFind() const;
    void setGlonassFind(const QString & glonassFind);

    //Установка внутреннего генератора приемника
    Q_PROPERTY(bool vnytrenGen READ vnytrenGen NOTIFY vnytrenGenChanged)
    const bool & vnytrenGen() const;
    void setVnytrenGen(const bool & vnytrenGen);

    //Прошел проверку приемник или нет
    Q_PROPERTY(bool good READ good NOTIFY goodChanged)
    const bool & good() const;
    void setGood(const bool & good);

    //Какая идет проверка
    Q_PROPERTY(QString indexProverki READ indexProverki NOTIFY indexProverkiChanged)
    const QString & indexProverki() const;
    void setIndexProverki(const QString & indexProverki);


    //Таймер
    Q_PROPERTY(QString timeTimer READ timeTimer NOTIFY signal_Tick)
    const QString & timeTimer() const;
    void setTimeTimer(const QString & timeTimer);

    //Прошел проверку приемник или нет
    //Режим
    Q_PROPERTY(QVariantList  listSpytnik READ listSpytnik NOTIFY signal_listSpytnikChanged)
    const QVariantList & listSpytnik() const;
    void setListSpytnik(const QVariantList & listSpytnik);

    //Амплитуда
    Q_PROPERTY(QVariantList  listSpytnik_Amplitude READ listSpytnik_Amplitude NOTIFY signal_listSpytnik_AmplitudeChanged)
    const QVariantList & listSpytnik_Amplitude() const;
    void setlistSpytnik_Amplitude(const QVariantList & listSpytnik_Amplitude);

    //Имя спутника
    Q_PROPERTY(QVariantList  listSpytnik_Name READ listSpytnik_Name NOTIFY signal_listSpytnik_NameChanged)
    const QVariantList & listSpytnik_Name() const;
    void setlistSpytnik_Name(const QVariantList & listSpytnik_Name);


    //Литера
    Q_PROPERTY(QVariantList  listSpytnik_Liters READ listSpytnik_Liters NOTIFY signal_listSpytnik_LitersChanged)
    const QVariantList & listSpytnik_Liters() const;
    void setlistSpytnik_Liters(const QVariantList & listSpytnik_Liters);

    Q_PROPERTY(QString  proverkaName READ proverkaName NOTIFY signal_proverkaNameChanged)
    const QString & proverkaName() const;
    void setProverkaName(const QString & proverkaName);





    int index;
    int indexList;

    N6700Model* n6700model;
    OsuilografModel* os;
    tp8Model* tp;

    RelizProverka* relizProverka;


    relizproverkaTimer* relizProverkaTimer;


    QVariantList list;

    QStringList get_tok;
    QStringList get_volt;
    QStringList get_p;

    QString m_indexNP;

public slots:
     void setTok(const QString & tok);
     void getR(int);
     void getG(int);
     void slot_getMeasureCURRentSignal(QVector<QString>);
     void slot_getMeasureVoltSignal(QVector<QString>);
     void slot_getUpdateCountFindQML(int,int,int);

     void slot_GetSignalProverka();

     void slot_Tick(QString);

     void slot_Good(bool);
     void slot_IndexProverki(QString);


     int slot_IndexNp();

     void slot_setListSpytnik(QStringList,QStringList,QStringList);

signals:
    void tokChanged(QString);
    void vChanged(QString);
    void powerChanged(QString);
    void gpsGenChanged(QString);
    void glonassGenChanged(QString);
    void gpsFindChanged(QString);
    void glonassFindChanged(QString);
    void vnytrenGenChanged();
    void goodChanged(bool);
    void indexProverkiChanged(QString);
    void indexNPChanged(QString);

    void signal_StartProverka();

    void signal_Tick(QString);
    void signal_listSpytnikChanged(QVariantList);
    void signal_listSpytnik_AmplitudeChanged(QVariantList);
    void signal_listSpytnik_NameChanged(QVariantList);
    void signal_listSpytnik_LitersChanged(QVariantList);

    void signal_StartProverkaIndex(QString namberProverkaStart);

    void signal_proverkaNameChanged(QString);



private:
    QString m_tok;
    QString m_v;
    QString m_power;
    QString m_gpsGen;
    QString m_glonassGen;
    QString m_gpsFind;
    QString m_glonassFind;
    bool m_vnytrenGen;
    bool m_good;
    QString m_indexProverki;

    QString m_tick;

    QVariantList m_listSpytnik;
    QVariantList m_listSpytnik_Amplitude;
    QVariantList m_listSpytnik_Name;
    QVariantList m_listSpytnik_Liters;

    QString m_proverkaName;



};

#endif // PROVERKAITEM_H
