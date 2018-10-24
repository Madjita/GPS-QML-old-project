#ifndef TP8MODEL_H
#define TP8MODEL_H

#include <QObject>

#include <tp8.h>

class tp8Model : public QObject
{
    Q_OBJECT



public:
    explicit tp8Model(QObject *parent = nullptr);
    ~tp8Model();

signals:
    void connectDevice(QString ip);
    void signal_findTp();
    void signal_comand1_Connect_Vx2_10(char NP,int _index); //Подключение к «ВЫХОД2» сигнала 10МГц с НП1-НП8
    void signal_comand2_Connect_Vx1_1C(char NP,int index); //Подключение к «ВЫХОД1» сигнала 1С с НП1-НП8
    void signal_comand3_Connect_Vx2_1C(int _index); //Подключение к «ВЫХОД2» сигнала 1С с внешнего источника (имитатора)
    void signal_comand4_Connect_Vx1_MShY(char NP,int _index); //Подключение к «ВЫХОД1» входа НП1-НП8 для проверки уровня напряжения питания МШУ
    void signal_comand5_Connect_10MG(char NP,int _index); //Подключение внешнего источника 10МГц к НП1-НП8
    void signal_comand6_Connect_Vx2_10_ext(int _index); //Подключение к «ВЫХОД2» внешнего источника 10МГц (для контроля уровня сигнала внешнего источника 10 МГц)
    void signal_comand7_Set_zatyxanie_10(char zatyx,int _index); //Установка уровня затухания внешнего источника 10 МГц в дБ (0-38,75 шаг 0,25)
    void signal_comand8_Connect_10_in(char NP,int _index);
    void signal_clear();

    //resive ui
    void signal_Ok(bool ok);
    void signal_comanda_ok(int comand, int index);
    void signal_comandaClear_ok();

public slots:
    void slot_ConnectTp(QString ip);
    void slot_findTp();
    void slot_comand1_Connect_Vx2_10(char NP,int _index); //Подключение к «ВЫХОД2» сигнала 10МГц с НП1-НП8
    void slot_comand2_Connect_Vx1_1C(char NP,int index); //Подключение к «ВЫХОД1» сигнала 1С с НП1-НП8
    void slot_comand3_Connect_Vx2_1C(int _index); //Подключение к «ВЫХОД2» сигнала 1С с внешнего источника (имитатора)
    void slot_comand4_Connect_Vx1_MShY(char NP,int _index); //Подключение к «ВЫХОД1» входа НП1-НП8 для проверки уровня напряжения питания МШУ
    void slot_comand5_Connect_10MG(char NP,int _index); //Подключение внешнего источника 10МГц к НП1-НП8
    void slot_comand6_Connect_Vx2_10_ext(int _index); //Подключение к «ВЫХОД2» внешнего источника 10МГц (для контроля уровня сигнала внешнего источника 10 МГц)
    void slot_comand7_Set_zatyxanie_10(char zatyx,int _index); //Установка уровня затухания внешнего источника 10 МГц в дБ (0-38,75 шаг 0,25)
    void slot_comand8_Connect_10_in(char NP,int _index);
    void slot_clear();

    //resive
    void slot_connectOk(bool ok);
    void slot_comanda_ok(int comand,int index);

    void deleteLayer();

    bool get_flagConnect();
private slots:
    void slot_comandaClear_ok();





public:
    TP8* tp;
};

#endif // TP8MODEL_H
