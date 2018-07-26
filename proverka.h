#ifndef PROVERKA_H
#define PROVERKA_H

#include <QObject>

#include <proverkaitem.h>

#include <gsgmodel.h>
#include <n6700model.h>
#include <portmodel.h>

#include <bdata.h>


#include <screencapture.h>

#include <cambermodel.h>

class Proverka : public QObject
{
   Q_OBJECT

public:
   explicit  Proverka(CamberModel* camberGet,gsgModel* gsg,N6700Model* n6700,PortModel* port,PortModel* port2,PortModel* port3,PortModel* port4,PortModel* port5,PortModel* port6,PortModel* port7,PortModel* port8,screenCapture *screenClassGet,QObject *parent = 0);


    screenCapture* screenClass;

    int kol_Auto_Start;
    int kol_cignalov; //переменная для 5 проверки для старта имитатора - генератора (подсчет сигналов на включение)
    int kol_cignalov_Start_Imitator; //переменная для 6 проверки для старта имитатора (подсчет сигналов на включение)

    CamberModel* camber;

    Q_PROPERTY(QList<QObject*> listProverka READ listProverka NOTIFY ListProverkaChanged)

    const QList<QObject*> listProverka() const;

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


    void Set_Number_Spytnik(proverkaItem*);


    BData* BD;


    void SetBD(BData* bd);


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


    QString idUser;
    QString idEtap;
    QString idData;
    QStringList idSerial;

    QStringList idLink;


    bool flagWork_Dont_PSIandRegylirovka;

    void endOneProverka();

signals:
    void ListProverkaChanged(QList<QObject*>);

    void signal_StartProverka(bool auto_test, int proverka);
    void signal_StartProverka_2();
    void signal_StartProverka_5();

    void signal_StartGSG();
    void setSIGNALtype(QString);
    void startTimer();
    void startTimerMrk();

    void signal_namberProverkaStartChanche(int);
    void signal_NamberModeCamberChanche(int);
    void signal_autoProverkaChanche(bool);


    void signal_GoodQML(QString numberOfProverka);

    void signal_BadQML(QString numberOfProverka);

    void signal_Tick(QString textTimerTick);

    void signal_Gen_setCONTrol(QString);

    void signal_Gen_setSATid(QString);

    //////////////////
    /// \brief signal_Next
    ///

    void signal_Next(); //сигнал для вызова диалогового окна для переключения проводов
    void signal_Finish(); //сигнал для вызова диалогового окна для завершения проверки

    //Сигнал для изменения фио и режима
    void signal_modeStartChanche(QString);
    void signal_fioStartChanche(QString);

    //Сигнал для запуска установки холода в камере
    void signal_startCold();

    //Сигнал для запуска установки НУ в камере
    void signal_startNY();

    //Сигнал для запуска установки повышенной темпиратуры в камере
    void signal_startHord();

    //обнуление страницы
    void signal_clearView();

    void signal_stopCamberWorkProverka();



public slots:
    void GetListProverkaChanged(QList<QObject*>*);
    void clearList();
    void addList(QString , QString , QString , QString , QString , bool , bool );
    void getR(int);
    void getList(QVector<QString>);

    void slot_StartProverka();

    void slot_StartProverka2();

    void slot_StartProverka3();



    void slot_EndProverka(int);

    void slot_StartProverka_Next();

    void slot_Tick(QString,QString);

    void slot_start_GEN();

    void slot_start_Imitator();

    //Слот для остановки переключения проверки

    void slot_next();

    void setModeStart(const QString & modeStart);
    void setfioStart(const QString & fioStart);



public:
  QList<QObject*>* m_ListProverkaItem;

  //ФИО
  QString m_fioStart;
  //Mode
  QString m_modeStart;

  int namberProverkaStart;
  int namberModeCamber;
  bool autoProverka;


};

#endif // PROVERKA_H
