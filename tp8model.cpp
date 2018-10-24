#include "tp8model.h"

tp8Model::tp8Model(QObject *parent) : QObject(parent)
{
    tp = new TP8();

    connect(this,&tp8Model::connectDevice,tp,&TP8::slot_ConnectTp); //Соединение
    connect(tp,&TP8::signal_Ok,this,&tp8Model::slot_connectOk); //Сигнал для подтверждения соединения
    connect(this,&tp8Model::signal_findTp,tp,&TP8::slot_findTp);
    connect(this,&tp8Model::signal_comand1_Connect_Vx2_10,tp,&TP8::slot_comand1_Connect_Vx2_10);
    connect(this,&tp8Model::signal_comand2_Connect_Vx1_1C,tp,&TP8::slot_comand2_Connect_Vx1_1C);
    connect(this,&tp8Model::signal_comand3_Connect_Vx2_1C,tp,&TP8::slot_comand3_Connect_Vx2_1C);
    connect(this,&tp8Model::signal_comand4_Connect_Vx1_MShY,tp,&TP8::slot_comand4_Connect_Vx1_MShY);
    connect(this,&tp8Model::signal_comand5_Connect_10MG,tp,&TP8::slot_comand5_Connect_10MG);
    connect(this,&tp8Model::signal_comand6_Connect_Vx2_10_ext,tp,&TP8::slot_comand6_Connect_Vx2_10_ext);
    connect(this,&tp8Model::signal_comand7_Set_zatyxanie_10,tp,&TP8::slot_comand7_Set_zatyxanie_10);
    connect(this,&tp8Model::signal_comand8_Connect_10_in,tp,&TP8::slot_comand8_Connect_10_in);
    connect(this,&tp8Model::signal_clear,tp,&TP8::slot_clear);

    connect(tp,&TP8::signal_comandaClear_ok,this,&tp8Model::slot_comandaClear_ok);

    //connect(tp,&TP8::signal_comanda_ok,this,&tp8Model::slot_comanda_ok);

}

tp8Model::~tp8Model()
{
    qDebug() << "Delete tp8Model";
//    qApp->processEvents();s
    delete tp;
}

void tp8Model::slot_ConnectTp(QString ip)
{
    emit connectDevice(ip);
}

void tp8Model::slot_findTp()
{
    emit signal_findTp();
}

void tp8Model::slot_comand1_Connect_Vx2_10(char NP, int index)
{
    emit signal_comand1_Connect_Vx2_10(NP, index);
}

void tp8Model::slot_comand2_Connect_Vx1_1C(char NP,int index)
{
    emit signal_comand2_Connect_Vx1_1C(NP,index);
}

void tp8Model::slot_comand3_Connect_Vx2_1C(int index)
{
    emit signal_comand3_Connect_Vx2_1C(index);
}

void tp8Model::slot_comand4_Connect_Vx1_MShY(char NP,int index)
{
    emit signal_comand4_Connect_Vx1_MShY(NP,index);
}

void tp8Model::slot_comand5_Connect_10MG(char NP,int index)
{
    emit signal_comand5_Connect_10MG(NP,index);
}

void tp8Model::slot_comand6_Connect_Vx2_10_ext(int index)
{
    emit signal_comand6_Connect_Vx2_10_ext(index);
}

void tp8Model::slot_comand7_Set_zatyxanie_10(char zatyx,int index)
{
    emit signal_comand7_Set_zatyxanie_10(zatyx,index);
}

void tp8Model::slot_comand8_Connect_10_in(char NP,int index)
{
    emit signal_comand8_Connect_10_in(NP,index);
}

void tp8Model::slot_clear()
{
    emit signal_clear();
}

void tp8Model::slot_connectOk(bool ok)
{
    emit signal_Ok(ok);
}

void tp8Model::slot_comanda_ok(int comand, int index)
{
    emit signal_comanda_ok(comand,index);
}

void tp8Model::slot_comandaClear_ok()
{
    emit signal_comandaClear_ok();
}

void tp8Model::deleteLayer()
{
    delete this;
}

bool tp8Model::get_flagConnect()
{
    return tp->flagConnect;
}
