#ifndef PROVERKAMODEL_H
#define PROVERKAMODEL_H


#include <QObject>

#include <proverkaitem.h>

#include <gsgmodel.h>
#include <n6700model.h>
#include <portmodel.h>

#include <bdata.h>


#include <cambermodel.h>


#include <proverka.h>



class ProverkaModel : public QObject
{
    Q_OBJECT
public:
    explicit ProverkaModel(CamberModel *camberGet, gsgModel *gsgGet, N6700Model *n6700Get, PortModel *portGet, PortModel *portGet2, PortModel *portGet3, PortModel *portGet4, PortModel *portGet5, PortModel *portGet6, PortModel *portGet7, PortModel *portGet8, QObject *parent);


     Proverka* proverka;


     Q_PROPERTY(QList<QObject*> listProverka READ listProverka NOTIFY ListProverkaChanged)

     const QList<QObject*> listProverka() const;


     //данные для установки режима и фио регулировщика

     //Режим этапа
     Q_PROPERTY(QString modeStart READ modeStart NOTIFY signal_modeStartChanche)
     const QString & modeStart() const;


     //Режим этапа
     Q_PROPERTY(QString fioStart READ fioStart NOTIFY signal_fioStartChanche)
     const QString & fioStart() const;


     //Номер проверки
     Q_PROPERTY(int  namberProverkaStart READ seTnamberProverkaStart WRITE namberProverkaStart_write NOTIFY signal_namberProverkaStartChanche)
     int  seTnamberProverkaStart() const;
     void namberProverkaStart_write(const int &);

     //Номер режима камеры
     Q_PROPERTY(int  namberModeCamber READ setNamberModeCamber WRITE getNamberModeCamber NOTIFY signal_NamberModeCamberChanche)
     int  setNamberModeCamber() const;
     void getNamberModeCamber(const int &);

     //Автоматическая проверка
     Q_PROPERTY(bool  autoProverka READ setautoProverka WRITE getautoProverka NOTIFY signal_autoProverkaChanche)
     bool  setautoProverka() const;
     void getautoProverka(const bool &);


    int kol_Auto_Start;
    int kol_cignalov; //переменная для 5 проверки для старта имитатора - генератора (подсчет сигналов на включение)
    int kol_cignalov_Start_Imitator; //переменная для 6 проверки для старта имитатора (подсчет сигналов на включение)

    CamberModel* camber;



    //Режим нормально,холода,тепла
    int autoMode_NY_cold_hart;


    QList<QObject*> res;

    gsgModel* gsg;
    N6700Model* n6700;

    PortModel* port;
    PortModel* port2;
    PortModel* port3;
    PortModel* port4;
    PortModel* port5;
    PortModel* port6;
    PortModel* port7;
    PortModel* port8;

    int start;


    QList<proverkaItem*> list;

    proverkaItem* proverka1;
    proverkaItem* proverka2;
    proverkaItem* proverka3;
    proverkaItem* proverka4;
    proverkaItem* proverka5;
    proverkaItem* proverka6;
    proverkaItem* proverka7;
    proverkaItem* proverka8;
    BData* BD;

     void SetBD(BData* bd);


    QString idUser;
    QString idEtap;
    QString idData;
    QStringList idSerial;

    QStringList idLink;


    bool flagWork_Dont_PSIandRegylirovka;


  QList<QObject*>* m_ListProverkaItem;

  //ФИО
  QString m_fioStart;
  //Mode
  QString m_modeStart;

  int namberProverkaStart;
  int namberModeCamber;
  bool autoProverka;



signals:
  void signal_StartProverka();
  void signal_StartProverka2();
  void signal_StartProverka3();

  void ListProverkaChanged(QList<QObject*>);

  void signal_namberProverkaStartChanche(int);
  void signal_NamberModeCamberChanche(int);
  void signal_autoProverkaChanche(bool);

  //Сигнал для изменения фио и режима
  void signal_modeStartChanche(QString);
  void signal_fioStartChanche(QString);

  void signal_Next(); //сигнал для вызова диалогового окна для переключения проводов
  void signal_Finish(); //сигнал для вызова диалогового окна для завершения проверки

public slots:
  void slot_StartProverka();

  void slot_StartProverka2();

  void slot_StartProverka3();

  void GetListProverkaChanged(QList<QObject*>*);

  void GetListProverkaSignal(QList<QObject*>);

};

#endif // PROVERKAMODEL_H
