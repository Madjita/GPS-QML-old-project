#ifndef ALGORITHMMODEL_H
#define ALGORITHMMODEL_H

#include <QObject>


#include <gsgmodel.h>
#include <n6700model.h>
#include <portmodel.h>
#include <osuilografmodel.h>
#include <tp8.h>
//////библиотека Управления камерой////////////
#include <cambermodel.h>


#include <proverkaitem.h>
#include <bdata.h>

#include <algorithm.h>

#include<clockcircle.h>

class AlgorithmModel : public QObject
{
    Q_OBJECT
public:
    explicit AlgorithmModel(QObject *parent = nullptr);
    ~AlgorithmModel();

    Algorithm* alg;
    ClockCircle* clock;

    //данные для установки режима и фио регулировщика


    Q_PROPERTY(QList<QObject*> listProverka READ listProverka NOTIFY ListProverkaChanged)
    const QList<QObject*> listProverka() const;

    //Режим этапа
    Q_PROPERTY(QString modeStart READ modeStart WRITE setModeStart NOTIFY signal_modeStartChanche)
    const QString & modeStart() const;
    void setModeStart(const QString &mode);


    //ФИОы
    Q_PROPERTY(QString fioStart READ fioStart WRITE setFioStart NOTIFY signal_fioStartChanche)
    const QString & fioStart() const;
    void setFioStart(const QString &fio);


    //Номер проверки
    Q_PROPERTY(int  namberProverkaStart READ seTnamberProverkaStart WRITE namberProverkaStart_write NOTIFY signal_namberProverkaStartChanche)
    int  seTnamberProverkaStart() const;
    void namberProverkaStart_write(const int &i);

    //Номер режима камеры
    Q_PROPERTY(int  namberModeCamber READ setNamberModeCamber WRITE getNamberModeCamber NOTIFY signal_NamberModeCamberChanche)
    int  setNamberModeCamber() const;
    void getNamberModeCamber(const int &i);

    //Автоматическая проверка
    Q_PROPERTY(bool  autoProverka READ setautoProverka WRITE getautoProverka NOTIFY signal_autoProverkaChanche)
    bool  setautoProverka() const;
    void getautoProverka(const bool &i);



signals:
    //QML variable
    void signal_namberProverkaStartChanche(int);
    void signal_NamberModeCamberChanche(int);
    void signal_autoProverkaChanche(bool);

    //Сигнал для изменения фио и режима
    void signal_modeStartChanche(QString);
    void signal_fioStartChanche(QString);

    //
    void signal_StartProverka();

    void endOneProverka(int index);

    void signal_Finish();
    void signal_clearView();
    void qml_text(QString str);
    void qml_good(bool ok,QString indexProverka,QString index);

    void ListProverkaChanged(QList<QObject*>);


public slots:
     void setDevice(CamberModel* _camber,gsgModel* _gsg,N6700Model* _n6700_1,N6700Model* _n6700_2,OsuilografModel* _os,tp8Model* _tp,PortModel* _port1,PortModel* _port2,PortModel* _port3,PortModel* _port4,PortModel* _port5,PortModel* _port6,PortModel* _port7,PortModel* _port8,ClockCircle* _clock);

     void slot_StartProverka();
//     void slot_StartProverka2();
//     void slot_StartProverka3();

     void GetListProverkaChanged(QList<QObject*>*);


     void clearWait();
     void clearWait_loadMRK(bool);



public:
  QList<QObject*> m_ListProverkaItem;
  QStringList list_CountNP;

  //ФИО
  QString m_fioStart;
  //Mode
  QString m_modeStart = "Регулировка";

  int namberProverkaStart;
  int namberModeCamber;
  bool autoProverka;
};

#endif // ALGORITHMMODEL_H
