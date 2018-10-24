#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QList>

#include <gsgmodel.h>
#include <n6700model.h>
#include <portmodel.h>
#include <osuilografmodel.h>
#include <tp8model.h>

#include <QMessageBox>

//////библиотека Управления камерой////////////
#include <cambermodel.h>


#include <proverkaitem.h>
#include <bdata.h>

class Algorithm : public QObject
{
    Q_OBJECT
public:
    explicit Algorithm(QObject *parent = nullptr);
    ~Algorithm();

    QSemaphore sem;
    int namberProverkaStart;
    int namberModeCamber;
    bool autoProverka;

    QString idUser;
    QString idEtap;
    QString idData;
    QStringList idSerial;

    QStringList idLink;


    QThread* mainThread = nullptr;


    int kol_Auto_Start;
    int kol_cignalov; //переменная для 5 проверки для старта имитатора - генератора (подсчет сигналов на включение)
    int kol_cignalov_Start_Imitator; //переменная для 6 проверки для старта имитатора (подсчет сигналов на включение)

    //Режим нормально,холода,тепла
    int autoMode_NY_cold_hart;

    void Set_Number_Spytnik(proverkaItem*);
    int kolStart;
    int start = 0;

    bool flagWork_Dont_PSIandRegylirovka;
    QString m_modeStart;
    QString m_fio;

    BData *BD;

signals:
    void sendZapros(QString data);
    QSqlQueryModel* getZapros(QString data);

    void listProverkaItemChanged(QList<QObject *> *);

    void setSIGNALtype(QString);
    void setPPS(int);
    void signal_StartGSG();
    //Сигнал в MainWindow Для открытие окна проверок
    void signalOnMainWindow_OpenWindowWork();
    //обнуление страницы
    void signal_clearView();

    void signal_StartProverka(bool auto_test, int proverka);
    void startTimer();
    void startTimerMrk();

    void signal_Next();

    void signal_Gen_setSATid(QString);
    void signal_Gen_setCONTrol(QString);
    void signal_StartProverka_5();

    //camber
    void signal_NextCamberTemprature();

    void signal_startNY();
    void signal_startCold();
    void signal_startHord();

    void signal_stopCamberWorkProverka();

    //qml
    void signal_Finish();
    void signal_finished();
    void endOneProverka(int);

    void qml_text(QString);
    void qml_good(bool ok,QString indexProverka,QString index);
public slots:
    void setDevice(CamberModel* _camber,gsgModel* _gsg,N6700Model* _n6700_1,N6700Model* _n6700_2,OsuilografModel* _os,tp8Model* _tp,PortModel* _port1,PortModel* _port2,PortModel* _port3,PortModel* _port4,PortModel* _port5,PortModel* _port6,PortModel* _port7,PortModel* _port8);



    ///
    void slot_StartProverka();
    void slot_StartOneProverka(int indexProverka, int modeCamber);
    void get_beginOSProv(int index);
    void get_beginOSProv_7(int index);

    void slot_comand_ok(int comand,int index);
    void signal_slot_StartProverka_Os(int index,int position);


    void slot_start_GSG();



private slots:
    void process_start();

    void initializate_proverka();
    void find_NameMRK_On_BD();

    void get_zaprozBD(QString);

    void slot_endProverka(int,int);
    void slot_start_GEN();

    void waitTpQML();


private:
    //variable
    N6700Model* n6700_1;
    N6700Model* n6700_2;
    CamberModel* camber;
    gsgModel* gsg;
    OsuilografModel* os;
    tp8Model* tp;


    QList<QObject*>* m_ListProverkaItem;

    QList<proverkaItem*> listProverkaItem;

    QStringList list_CountNP;
    QList<QByteArray> listNameMRK;

    QList<PortModel*> portList;

    //////////
    int Count_NP;

};

#endif // ALGORITHM_H
