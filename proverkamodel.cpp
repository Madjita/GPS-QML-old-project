#include "proverkamodel.h"

ProverkaModel::ProverkaModel(CamberModel *camberGet, gsgModel *gsgGet, N6700Model *n6700Get, PortModel *portGet, PortModel *portGet2, PortModel *portGet3, PortModel *portGet4, PortModel *portGet5, PortModel *portGet6, PortModel *portGet7, PortModel *portGet8, screenCapture *screenClassGet, QObject *parent):
    QObject(parent),
    m_ListProverkaItem(new QList<QObject*>()),
    camber(camberGet),
    gsg(gsgGet),
    n6700(n6700Get),
    port(portGet),
    port2(portGet2),
    port3(portGet3),
    port4(portGet4),
    port5(portGet5),
    port6(portGet6),
    port7(portGet7),
    port8(portGet8),
    autoMode_NY_cold_hart(0),
    autoProverka(false), // true
    screenClass(screenClassGet),
    kol_Auto_Start(0),
    kol_cignalov(0), //переменная для 5 проверки для старта имитатора - генератора (подсчет сигналов на включение)
    kol_cignalov_Start_Imitator(0) //переменная для 6 проверки для старта имитатора (подсчет сигналов на включение)
{



    ////////////////////////////////////////////////////////////////////////////

    m_ListProverkaItem->clear();
    res.clear();



    proverka1 = new proverkaItem(1,n6700,gsg,port,false, false,"ЛЖ0001",screenClass);
    //  proverka2 = new proverkaItem(2,n6700,gsg,port2,false, true,"ЛЖ0002");
    //    proverka3 = new proverkaItem(3,n6700,gsg,port,true, false,"ЛЖ0003");
    //    proverka4 = new proverkaItem(4,n6700,gsg,port,true, false,"ЛЖ0004");
    //    proverka5 = new proverkaItem(1,n6700,gsg,port,true, false,"ЛЖ0005");
    //    proverka6 = new proverkaItem(2,n6700,gsg,port,true, false,"ЛЖ0006");
    //    proverka7 = new proverkaItem(3,n6700,gsg,port,true, false,"ЛЖ0007");
    //    proverka8 = new proverkaItem(4,n6700,gsg,port,true, false,"ЛЖ0008");

    proverka2 = nullptr;
    proverka3 = nullptr;
    proverka4 = nullptr;
    proverka5 = nullptr;
    proverka6 = nullptr;
    proverka7 = nullptr;
    proverka8 = nullptr;

    //1
    // m_ListProverkaItem->append(new proverkaItem(1,n6700,gsg,port,false, false,"ЛЖ0001")); // Дописать код для спрашивания имени у приемника
    m_ListProverkaItem->append(proverka1); // Дописать код для спрашивания имени у приемника

    res.append(m_ListProverkaItem->last());

    //2

    // m_ListProverkaItem->append(new proverkaItem(2,n6700,gsg,port2,false, true,"ЛЖ0002"));
    m_ListProverkaItem->append(new proverkaItem(2,n6700,gsg,port2,false, true,"ЛЖ0002",screenClass));
    res.append(m_ListProverkaItem->last());
    //3

    m_ListProverkaItem->append(new proverkaItem(3,n6700,gsg,port3,false, true,"ЛЖ0003",screenClass));
    res.append(m_ListProverkaItem->last());

    //4

    m_ListProverkaItem->append(new proverkaItem(4,n6700,gsg,port4,false, true,"ЛЖ0004",screenClass));
    res.append(m_ListProverkaItem->last());

    //5

    m_ListProverkaItem->append(new proverkaItem(3,n6700,gsg,port5,false, true,"ЛЖ0005",screenClass));
    res.append(m_ListProverkaItem->last());

    //6

    m_ListProverkaItem->append(new proverkaItem(4,n6700,gsg,port6,false, true,"ЛЖ0006",screenClass));
    res.append(m_ListProverkaItem->last());

    //7

    m_ListProverkaItem->append(new proverkaItem(3,n6700,gsg,port7,false, true,"ЛЖ0007",screenClass));
    res.append(m_ListProverkaItem->last());
    //8

    m_ListProverkaItem->append(new proverkaItem(4,n6700,gsg,port8,false, true,"ЛЖ0008",screenClass));
    res.append(m_ListProverkaItem->last());


    proverka = new Proverka(camber,gsg,n6700,n6700Get,port,port2,port3,port4,port5,port6,port7,port8,screenClass); // Создали модель класса port, для управления и обновления объектов в QML



    //connect(proverka,&Proverka::ListProverkaChanged,this,&ProverkaModel::GetListProverkaSignal);



    connect(this,&ProverkaModel::signal_StartProverka,proverka,&Proverka::slot_StartProverka);
    connect(this,&ProverkaModel::signal_StartProverka2,proverka,&Proverka::slot_StartProverka2);
    connect(this,&ProverkaModel::signal_StartProverka3,proverka,&Proverka::slot_StartProverka3);
}

const QList<QObject*> ProverkaModel::listProverka() const
{
    return *m_ListProverkaItem;
}

const QString &ProverkaModel::modeStart() const
{
    return m_modeStart;
}

const QString &ProverkaModel::fioStart() const
{
    return m_fioStart;
}

int ProverkaModel::seTnamberProverkaStart() const
{
    return namberProverkaStart;
}

void ProverkaModel::namberProverkaStart_write(const int &i)
{
    namberProverkaStart = i;
}

int ProverkaModel::setNamberModeCamber() const
{
    return namberModeCamber;
}

void ProverkaModel::getNamberModeCamber(const int &i)
{
    namberModeCamber = i;
}

bool ProverkaModel::setautoProverka() const
{
    return autoProverka;
}

void ProverkaModel::getautoProverka(const bool &i)
{
    autoProverka = i;
}

void ProverkaModel::GetListProverkaChanged(QList<QObject*>* list)
{

    for(int i=0;i<list->count();i++)
    {
        m_ListProverkaItem->append(list->value(i));
    }

}

void ProverkaModel::GetListProverkaSignal(QList<QObject *> list)
{


    //for(int i=0;i<list.count();i++)
   // {
     //   m_ListProverkaItem->append(list.value(i));
   // }

   // emit   ListProverkaChanged(res);
}

void ProverkaModel::SetBD(BData *bd)
{
    BD = bd;

    proverka->SetBD(bd);
}

void ProverkaModel::slot_StartProverka()
{
    emit signal_StartProverka();
}

void ProverkaModel::slot_StartProverka2()
{
     emit signal_StartProverka2();
}

void ProverkaModel::slot_StartProverka3()
{
     emit signal_StartProverka3();
}
