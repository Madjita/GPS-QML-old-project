#include "proverka.h"
#include <QWidget>

int Count_NP = 1;

Proverka::Proverka(CamberModel* camberGet,gsgModel* gsgGet, N6700Model* n6700Get,PortModel* portGet,PortModel* portGet2,PortModel* portGet3,PortModel* portGet4,PortModel* portGet5,PortModel* portGet6,PortModel* portGet7,PortModel* portGet8,screenCapture *screenClassGet,QObject *parent):
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



    QObject::connect(gsg,&gsgModel::countSpytnik_R,this,&Proverka::getR);

    QObject::connect(n6700,&N6700Model::getMeasureCURRentSignal,this,&Proverka::getList);

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

    proverka2 = NULL;
    proverka3 = NULL;
    proverka4 = NULL;
    proverka5 = NULL;
    proverka6 = NULL;
    proverka7 = NULL;
    proverka8 = NULL;

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




    for(int i=0;i <Count_NP;i++) //for(int i=0;i <2;i++)
    {

        list.append((proverkaItem*)m_ListProverkaItem->value(i));
    }


    qDebug () << "Количество НП = " << list.count();

    for(int i=0;i < list.count();i++)
    {
        //Сигнал от relizproverka о завершении проверки с index

        QObject::connect(list[i]->relizProverka,&RelizProverka::signal_EndProverka,this,&Proverka::slot_EndProverka,Qt::DirectConnection); //

        //Сигнал на запуск 2 проверки
        QObject::connect(this,&Proverka::signal_StartProverka_2,list[i]->relizProverkaTimer,&relizproverkaTimer::slot_EndBlock,Qt::DirectConnection);

        QObject::connect(this,&Proverka::signal_StartProverka_5,list[i]->relizProverkaTimer,&relizproverkaTimer::slot_EndBlock_5_proverka,Qt::QueuedConnection);

        QObject::connect(this,&Proverka::signal_StartProverka,list[i]->relizProverka,&RelizProverka::Work,Qt::QueuedConnection);

        QObject::connect(list[i]->relizProverka,&RelizProverka::signal_startGen,this,&Proverka::slot_start_GEN);

        QObject::connect(list[i]->relizProverka,&RelizProverka::signal_startImitator,this,&Proverka::slot_start_Imitator);

    }


    QObject::connect(this,&Proverka::startTimerMrk,port->PortNew,&Port::Work,Qt::QueuedConnection);
    QObject::connect(this,&Proverka::startTimerMrk,port2->PortNew,&Port::Work,Qt::QueuedConnection);



    QObject::connect(this,&Proverka::signal_StartGSG,gsg,&gsgModel::slot_StartTimer,Qt::QueuedConnection);
    QObject::connect(this,&Proverka::setSIGNALtype,gsg,&gsgModel::setSIGNALtype,Qt::QueuedConnection);

    QObject::connect(this,&Proverka::signal_Gen_setCONTrol,gsg,&gsgModel::slot_Gen_setCONTrol,Qt::QueuedConnection);

    QObject::connect(this,&Proverka::signal_Gen_setSATid,gsg,&gsgModel::slot_Gen_setSATid,Qt::QueuedConnection);

    QObject::connect(this,&Proverka::startTimer,n6700,&N6700Model::slot_StartTimer,Qt::QueuedConnection);





    ListProverkaChanged(res);
}


const QString &Proverka::modeStart() const
{
    return m_modeStart;
}

void Proverka::setModeStart(const QString &modeStart)
{
    if(modeStart != m_modeStart)
    {
        m_modeStart = modeStart;

        emit signal_modeStartChanche(m_modeStart);

        QSqlQueryModel* SQL_etap;

        SQL_etap = BD->zaprosQueryModel("SELECT Etap,Id FROM Etap WHERE Etap LIKE '"+m_modeStart+"'");

        idEtap = SQL_etap->data(SQL_etap->index(0,1), Qt::EditRole).toString();

        qDebug () << idEtap;


    }
}

const QString &Proverka::fioStart() const
{
    return m_fioStart;
}

int Proverka::seTnamberProverkaStart() const
{
    return namberProverkaStart;
}

void Proverka::namberProverkaStart_write(const int &i)
{
    namberProverkaStart = i;
}

int Proverka::setNamberModeCamber() const
{
    return namberModeCamber;
}

void Proverka::getNamberModeCamber(const int &i)
{
    namberModeCamber = i;
}

bool Proverka::setautoProverka() const
{
    return autoProverka;
}

void Proverka::getautoProverka(const bool &i)
{
    autoProverka = i;
}

void Proverka::endOneProverka()
{
    emit signal_Finish();
    qDebug() << "LOL 3 signal_startColdHard = " << autoMode_NY_cold_hart;
    qDebug() << "Finish";

    emit signal_stopCamberWorkProverka();

    autoMode_NY_cold_hart = 1;kol_Auto_Start =0; //Устанавливаем НУ
    if(proverka1 != nullptr){proverka1->relizProverka->flag_auto = false;qDebug() << "flag_auto =" <<proverka1->relizProverka->flag_auto;}
    if(proverka2 != nullptr)proverka2->relizProverka->flag_auto = false;
    if(proverka3 != nullptr)proverka3->relizProverka->flag_auto = false;
    if(proverka4 != nullptr)proverka4->relizProverka->flag_auto = false;
    if(proverka5 != nullptr)proverka5->relizProverka->flag_auto = false;
    if(proverka6 != nullptr)proverka6->relizProverka->flag_auto = false;
    if(proverka7 != nullptr)proverka7->relizProverka->flag_auto = false;
    if(proverka8 != nullptr)proverka8->relizProverka->flag_auto = false;
}

void Proverka::setfioStart(const QString &fioStart)
{
    if(fioStart != m_fioStart)
    {
        m_fioStart = fioStart;

        emit signal_fioStartChanche(m_fioStart);

        QSqlQueryModel* SQL_user;

        SQL_user = BD->zaprosQueryModel("SELECT FIO,Id FROM User WHERE FIO LIKE '"+m_fioStart+"'");

        if(SQL_user->rowCount() > 0)
        {

        }
        else
        {
            BD->zaprosQueryModel("INSERT INTO User (FIO) VALUES('"+m_fioStart+"')");

            SQL_user = BD->zaprosQueryModel("SELECT FIO,Id FROM User WHERE FIO LIKE '"+m_fioStart+"'");
        }

        idUser = SQL_user->data(SQL_user->index(0,1), Qt::EditRole).toString();


    }
}


void Proverka::GetListProverkaChanged(QList<QObject*>* list)
{

    for(int i=0;i<list->count();i++)
    {
        m_ListProverkaItem->append(list->value(i));
    }

}


void Proverka::clearList()
{
    res.clear();
    m_ListProverkaItem->clear();
}

void Proverka::addList(QString tokGet, QString vGet, QString powerGet, QString gpsFindGet,QString glonassFindGet,
                       bool vnytrenGenGet, bool goodGet)
{
    //1
    //    proverka1 = new proverkaItem(1,n6700,gsg,port,false, false,"ЛЖ0001"); // Дописать код для спрашивания имени у приемника

    //    m_ListProverkaItem->append(proverka1);

    //    res.append(m_ListProverkaItem->last());

    //    //2
    //    proverka2 = new proverkaItem(2,n6700,gsg,port2,false, true,"ЛЖ0002");

    //    m_ListProverkaItem->append(proverka2);
    //    res.append(m_ListProverkaItem->last());
    //    //3
    //    proverka3 = new proverkaItem(3,n6700,gsg,port,true, false);
    //    m_ListProverkaItem->append(proverka3);
    //    res.append(m_ListProverkaItem->last());

    //   //4
    //    proverka4 = new proverkaItem(4,n6700,gsg,port,true, false);
    //    m_ListProverkaItem->append(proverka4);
    //    res.append(m_ListProverkaItem->last());

    //   //5
    //    proverka5 = new proverkaItem(1,n6700,gsg,port,true, false);
    //    m_ListProverkaItem->append(proverka5);
    //    res.append(m_ListProverkaItem->last());

    //    //6
    //    proverka6 = new proverkaItem(2,n6700,gsg,port,true, false);
    //    m_ListProverkaItem->append(proverka6);
    //    res.append(m_ListProverkaItem->last());

    //    //7
    //    proverka7 = new proverkaItem(3,n6700,gsg,port,true, false);
    //    m_ListProverkaItem->append(proverka7);
    //    res.append(m_ListProverkaItem->last());
    //    //8
    //    proverka8 = new proverkaItem(4,n6700,gsg,port,true, false);
    //    m_ListProverkaItem->append(proverka8);
    //    res.append(m_ListProverkaItem->last());


    //  ListProverkaChanged(res);

}

void Proverka::getR(int R)
{

    res.clear();


    for(int i =0; i < m_ListProverkaItem->count();i++)
    {

        res.append(m_ListProverkaItem->value(i));
    }



    ListProverkaChanged(res);

    // qDebug () << "ListProverkaChanged" << res << "R = " << gsg->R << "G = " << gsg->G;





}

void Proverka::getList(QVector<QString> list)
{
    res.clear();


    for(int i =0; i < m_ListProverkaItem->count();i++)
    {

        res.append(m_ListProverkaItem->value(i));
    }


    ListProverkaChanged(res);

    // qDebug () << "ListProverkaChanged" << res << "n6700->item1->tok  = " << n6700->item1->tok << "n6700->item1->tok  = " << n6700->item1->v;
}




void Proverka::slot_StartProverka()
{

    flagWork_Dont_PSIandRegylirovka = false;
    //Добавить код для проверки НА в БД .

    QSqlQueryModel* SQL_Etap;

    SQL_Etap  = BD->zaprosQueryModel("SELECT * FROM Serial");

    QStringList str;
    QStringList str_id;

    for(int i=0;i< SQL_Etap->rowCount();i++)
    {
        str.append(SQL_Etap->data(SQL_Etap->index(i,1), Qt::EditRole).toString());
        str_id.append(SQL_Etap->data(SQL_Etap->index(i,0), Qt::EditRole).toString());
    }


    int proverka_kolNP =0;

    idSerial.clear();

    for(int i=0;i< str.count();i++)
    {
        // if(str.value(i) == "ЛЖ0001" || str.value(i) == "ЛЖ0002")
        if(str.value(i) == "ЛЖ0001")
        {
            proverka_kolNP++;

            idSerial.append(SQL_Etap->data(SQL_Etap->index(i,0), Qt::EditRole).toString());

        }
    }

    QDateTime start;
    start = QDateTime::currentDateTime();

    //записываем дату старта проверки
    BD->zaprosQueryModel("INSERT INTO DateStartEnd (DateStart) VALUES('"+start.toString("dd.MM.yyyy  hh:mm:ss")+"')");

    SQL_Etap =  BD->zaprosQueryModel("SELECT DateStart,Id FROM DateStartEnd WHERE DateStart LIKE '"+start.toString("dd.MM.yyyy  hh:mm:ss")+"'");

    //Сохраняем ID начала заупска проверки
    idData = SQL_Etap->data(SQL_Etap->index(0,1), Qt::EditRole).toString();

    //    BD->zaprosQueryModel("INSERT INTO Result (DateStart) VALUES('"+start.toString("dd.MM.yyyy  hh:mm:ss")+"')");

    //    BD->zaprosQueryModel("UPDATE Result SET DateEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"' WHERE DateStart = '22.03.2017  15:03:24'");

    idLink.clear();

    //подсчит количества стартов
    kol_Auto_Start++;


    autoMode_NY_cold_hart++;

    //Установили НУ, потом установим Холод, после установим тепло.
    switch (kol_Auto_Start)
    {
    case 1: autoMode_NY_cold_hart = 1;break; //Устанавливаем НУ
    case 2: autoMode_NY_cold_hart = 2;break; //Устанавливаем -40
    case 3: autoMode_NY_cold_hart = 3; break; //Устанавливаем НУ после -40
    case 4: autoMode_NY_cold_hart = 4; break; //Устанавливаем +50
    case 5: autoMode_NY_cold_hart = 5; break; //Устанавливаем НУ после +50
    case 6: {
        autoMode_NY_cold_hart = 1;kol_Auto_Start =0; //Устанавливаем НУ
        if(proverka1 != nullptr)proverka1->relizProverka->flag_auto = false;
        if(proverka2 != nullptr)proverka2->relizProverka->flag_auto = false;
        if(proverka3 != nullptr)proverka3->relizProverka->flag_auto = false;
        if(proverka4 != nullptr)proverka4->relizProverka->flag_auto = false;
        if(proverka5 != nullptr)proverka5->relizProverka->flag_auto = false;
        if(proverka6 != nullptr)proverka6->relizProverka->flag_auto = false;
        if(proverka7 != nullptr)proverka7->relizProverka->flag_auto = false;
        if(proverka8 != nullptr)proverka8->relizProverka->flag_auto = false;
    }

    }

    for(int i=0;i< proverka_kolNP;i++)
    {
        BD->zaprosQueryModel("INSERT INTO Link_Result (IdUser,IdEtap,IdDate,IdSerial,IdTemp) VALUES('"+idUser+"','"+idEtap+"','"+idData+"','"+idSerial.value(i)+"','"+QString::number(autoMode_NY_cold_hart)+"')");

        SQL_Etap = BD->zaprosQueryModel("SELECT Id FROM Link_Result WHERE IdDate LIKE '"+idData+"' AND IdSerial LIKE '"+idSerial.value(i)+"' AND IdUser LIKE '"+idUser+"' AND IdEtap LIKE '"+idEtap+"' AND IdTemp LIKE '"+QString::number(autoMode_NY_cold_hart)+"'");
        //Записываем ID для результатов именно этого юсера этапа времени и прибора
        idLink.append(SQL_Etap->data(SQL_Etap->index(0,0), Qt::EditRole).toString());



        switch (i)
        {
        case 0: if(proverka1 != nullptr)proverka1->relizProverka->SetIdLink(idLink.last());proverka1->relizProverka->start = start; proverka1->relizProverka->autoMode_NY_cold_hart = autoMode_NY_cold_hart; break;
        case 1: if(proverka2 != nullptr)proverka2->relizProverka->SetIdLink(idLink.last());proverka2->relizProverka->start = start;proverka2->relizProverka->autoMode_NY_cold_hart = autoMode_NY_cold_hart; break;
        case 2: if(proverka3 != nullptr)proverka3->relizProverka->SetIdLink(idLink.last());proverka3->relizProverka->start = start;proverka3->relizProverka->autoMode_NY_cold_hart = autoMode_NY_cold_hart; break;
        case 3: if(proverka4 != nullptr)proverka4->relizProverka->SetIdLink(idLink.last());proverka4->relizProverka->start = start;proverka4->relizProverka->autoMode_NY_cold_hart = autoMode_NY_cold_hart; break;
        case 4: if(proverka5 != nullptr)proverka5->relizProverka->SetIdLink(idLink.last());proverka5->relizProverka->start = start;proverka5->relizProverka->autoMode_NY_cold_hart = autoMode_NY_cold_hart; break;
        case 5: if(proverka6 != nullptr)proverka6->relizProverka->SetIdLink(idLink.last());proverka6->relizProverka->start = start;proverka6->relizProverka->autoMode_NY_cold_hart = autoMode_NY_cold_hart; break;
        case 6: if(proverka7 != nullptr)proverka7->relizProverka->SetIdLink(idLink.last());proverka7->relizProverka->start = start;proverka7->relizProverka->autoMode_NY_cold_hart = autoMode_NY_cold_hart; break;
        case 7: if(proverka8 != nullptr)proverka8->relizProverka->SetIdLink(idLink.last());proverka8->relizProverka->start = start;proverka8->relizProverka->autoMode_NY_cold_hart = autoMode_NY_cold_hart; break;

        }
    }



    if(proverka_kolNP == Count_NP)
    {
        //////////////////////////////////////////////////////////////////////////////

        emit setSIGNALtype("START");
        emit signal_StartGSG();

        emit signal_clearView();
        emit signal_StartProverka(autoProverka, namberProverkaStart);
        emit startTimerMrk();
        emit startTimer();
    }
    else
    {
        qDebug () << "Not find NP in the BD!";

        //Дописать отмену загрузки имитатора
    }


}

void Proverka::slot_StartProverka2()
{


    flagWork_Dont_PSIandRegylirovka = true;
    //Добавить код для проверки НА в БД .

    QSqlQueryModel* SQL_Etap;

    SQL_Etap  = BD->zaprosQueryModel("SELECT * FROM Serial");

    QStringList str;
    QStringList str_id;

    for(int i=0;i< SQL_Etap->rowCount();i++)
    {
        str.append(SQL_Etap->data(SQL_Etap->index(i,1), Qt::EditRole).toString());
        str_id.append(SQL_Etap->data(SQL_Etap->index(i,0), Qt::EditRole).toString());
    }


    int proverka_kolNP =0;

    idSerial.clear();

    for(int i=0;i< str.count();i++)
    {
        // if(str.value(i) == "ЛЖ0001" || str.value(i) == "ЛЖ0002")
        if(str.value(i) == "ЛЖ0001")
        {
            proverka_kolNP++;

            idSerial.append(SQL_Etap->data(SQL_Etap->index(i,0), Qt::EditRole).toString());

        }
    }

    QDateTime start;
    start = QDateTime::currentDateTime();

    //записываем дату старта проверки
    BD->zaprosQueryModel("INSERT INTO DateStartEnd (DateStart) VALUES('"+start.toString("dd.MM.yyyy  hh:mm:ss")+"')");

    SQL_Etap =  BD->zaprosQueryModel("SELECT DateStart,Id FROM DateStartEnd WHERE DateStart LIKE '"+start.toString("dd.MM.yyyy  hh:mm:ss")+"'");

    //Сохраняем ID начала заупска проверки
    idData = SQL_Etap->data(SQL_Etap->index(0,1), Qt::EditRole).toString();

    //    BD->zaprosQueryModel("INSERT INTO Result (DateStart) VALUES('"+start.toString("dd.MM.yyyy  hh:mm:ss")+"')");

    //    BD->zaprosQueryModel("UPDATE Result SET DateEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"' WHERE DateStart = '22.03.2017  15:03:24'");

    idLink.clear();

    //подсчит количества стартов
    kol_Auto_Start++;


    autoMode_NY_cold_hart = 1; // НУ


    for(int i=0;i< proverka_kolNP;i++)
    {
        BD->zaprosQueryModel("INSERT INTO Link_Result (IdUser,IdEtap,IdDate,IdSerial,IdTemp) VALUES('"+idUser+"','"+idEtap+"','"+idData+"','"+idSerial.value(i)+"','"+QString::number(autoMode_NY_cold_hart)+"')");

        SQL_Etap = BD->zaprosQueryModel("SELECT Id FROM Link_Result WHERE IdDate LIKE '"+idData+"' AND IdSerial LIKE '"+idSerial.value(i)+"' AND IdUser LIKE '"+idUser+"' AND IdEtap LIKE '"+idEtap+"' AND IdTemp LIKE '"+QString::number(autoMode_NY_cold_hart)+"'");
        //Записываем ID для результатов именно этого юсера этапа времени и прибора
        idLink.append(SQL_Etap->data(SQL_Etap->index(0,0), Qt::EditRole).toString());



        switch (i)
        {
        case 0: if(proverka1 != nullptr)proverka1->relizProverka->SetIdLink(idLink.last());proverka1->relizProverka->start = start; proverka1->relizProverka->autoMode_NY_cold_hart = autoMode_NY_cold_hart; break;
        case 1: if(proverka2 != nullptr)proverka2->relizProverka->SetIdLink(idLink.last());proverka2->relizProverka->start = start;proverka2->relizProverka->autoMode_NY_cold_hart = autoMode_NY_cold_hart; break;
        case 2: if(proverka3 != nullptr)proverka3->relizProverka->SetIdLink(idLink.last());proverka3->relizProverka->start = start;proverka3->relizProverka->autoMode_NY_cold_hart = autoMode_NY_cold_hart; break;
        case 3: if(proverka4 != nullptr)proverka4->relizProverka->SetIdLink(idLink.last());proverka4->relizProverka->start = start;proverka4->relizProverka->autoMode_NY_cold_hart = autoMode_NY_cold_hart; break;
        case 4: if(proverka5 != nullptr)proverka5->relizProverka->SetIdLink(idLink.last());proverka5->relizProverka->start = start;proverka5->relizProverka->autoMode_NY_cold_hart = autoMode_NY_cold_hart; break;
        case 5: if(proverka6 != nullptr)proverka6->relizProverka->SetIdLink(idLink.last());proverka6->relizProverka->start = start;proverka6->relizProverka->autoMode_NY_cold_hart = autoMode_NY_cold_hart; break;
        case 6: if(proverka7 != nullptr)proverka7->relizProverka->SetIdLink(idLink.last());proverka7->relizProverka->start = start;proverka7->relizProverka->autoMode_NY_cold_hart = autoMode_NY_cold_hart; break;
        case 7: if(proverka8 != nullptr)proverka8->relizProverka->SetIdLink(idLink.last());proverka8->relizProverka->start = start;proverka8->relizProverka->autoMode_NY_cold_hart = autoMode_NY_cold_hart; break;

        }
    }

    if(proverka_kolNP == Count_NP)
    {
        //////////////////////////////////////////////////////////////////////////////

        emit setSIGNALtype("START");
        emit signal_StartGSG();

        emit signal_clearView();
        emit signal_StartProverka(autoProverka, namberProverkaStart);
        emit startTimerMrk();
        emit startTimer();
    }
    else
    {
        qDebug () << "Not find NP in the BD!";

        //Дописать отмену загрузки имитатора
    }
}

void Proverka::slot_StartProverka3()
{

    flagWork_Dont_PSIandRegylirovka = false;
    //Добавить код для проверки НА в БД .

    QSqlQueryModel* SQL_Etap;

    SQL_Etap  = BD->zaprosQueryModel("SELECT * FROM Serial");

    QStringList str;
    QStringList str_id;

    for(int i=0;i< SQL_Etap->rowCount();i++)
    {
        str.append(SQL_Etap->data(SQL_Etap->index(i,1), Qt::EditRole).toString());
        str_id.append(SQL_Etap->data(SQL_Etap->index(i,0), Qt::EditRole).toString());
    }


    int proverka_kolNP =0;

    idSerial.clear();

    for(int i=0;i< str.count();i++)
    {
        // if(str.value(i) == "ЛЖ0001" || str.value(i) == "ЛЖ0002")
        if(str.value(i) == "ЛЖ0001")
        {
            proverka_kolNP++;

            idSerial.append(SQL_Etap->data(SQL_Etap->index(i,0), Qt::EditRole).toString());

        }
    }

    QDateTime start;
    start = QDateTime::currentDateTime();

    //записываем дату старта проверки
    BD->zaprosQueryModel("INSERT INTO DateStartEnd (DateStart) VALUES('"+start.toString("dd.MM.yyyy  hh:mm:ss")+"')");

    SQL_Etap =  BD->zaprosQueryModel("SELECT DateStart,Id FROM DateStartEnd WHERE DateStart LIKE '"+start.toString("dd.MM.yyyy  hh:mm:ss")+"'");

    //Сохраняем ID начала заупска проверки
    idData = SQL_Etap->data(SQL_Etap->index(0,1), Qt::EditRole).toString();

   // BD->zaprosQueryModel("INSERT INTO Result (DateStart) VALUES('"+start.toString("dd.MM.yyyy  hh:mm:ss")+"')");

   // BD->zaprosQueryModel("UPDATE Result SET DateEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"' WHERE DateStart = '22.03.2017  15:03:24'");

    idLink.clear();

    //подсчит количества стартов
    kol_Auto_Start++;


    //Установили НУ, потом установим Холод, после установим тепло.
    switch (namberModeCamber)
    {
    case 1: autoMode_NY_cold_hart = 1;break; //Устанавливаем НУ
    case 2: autoMode_NY_cold_hart = 2;break; //Устанавливаем -40
    case 3: autoMode_NY_cold_hart = 3; break; //Устанавливаем НУ после -40
    case 4: autoMode_NY_cold_hart = 4; break; //Устанавливаем +50
    case 5: autoMode_NY_cold_hart = 5; break; //Устанавливаем НУ после +50
    case 6: {
        autoMode_NY_cold_hart = 1;kol_Auto_Start =0; //Устанавливаем НУ
        if(proverka1 != nullptr)proverka1->relizProverka->flag_auto = false;
        if(proverka2 != nullptr)proverka2->relizProverka->flag_auto = false;
        if(proverka3 != nullptr)proverka3->relizProverka->flag_auto = false;
        if(proverka4 != nullptr)proverka4->relizProverka->flag_auto = false;
        if(proverka5 != nullptr)proverka5->relizProverka->flag_auto = false;
        if(proverka6 != nullptr)proverka6->relizProverka->flag_auto = false;
        if(proverka7 != nullptr)proverka7->relizProverka->flag_auto = false;
        if(proverka8 != nullptr)proverka8->relizProverka->flag_auto = false;
    }

    }



    for(int i=0;i< proverka_kolNP;i++)
    {
        BD->zaprosQueryModel("INSERT INTO Link_Result (IdUser,IdEtap,IdDate,IdSerial,IdTemp) VALUES('"+idUser+"','"+idEtap+"','"+idData+"','"+idSerial.value(i)+"','"+QString::number(autoMode_NY_cold_hart)+"')");

        SQL_Etap = BD->zaprosQueryModel("SELECT Id FROM Link_Result WHERE IdDate LIKE '"+idData+"' AND IdSerial LIKE '"+idSerial.value(i)+"' AND IdUser LIKE '"+idUser+"' AND IdEtap LIKE '"+idEtap+"' AND IdTemp LIKE '"+QString::number(autoMode_NY_cold_hart)+"'");
        //Записываем ID для результатов именно этого юсера этапа времени и прибора
        idLink.append(SQL_Etap->data(SQL_Etap->index(0,0), Qt::EditRole).toString());

        switch (i)
        {
        case 0: if(proverka1 != nullptr)proverka1->relizProverka->SetIdLink(idLink.last());proverka1->relizProverka->start = start; proverka1->relizProverka->autoMode_NY_cold_hart = autoMode_NY_cold_hart; break;
        case 1: if(proverka2 != nullptr)proverka2->relizProverka->SetIdLink(idLink.last());proverka2->relizProverka->start = start;proverka2->relizProverka->autoMode_NY_cold_hart = autoMode_NY_cold_hart; break;
        case 2: if(proverka3 != nullptr)proverka3->relizProverka->SetIdLink(idLink.last());proverka3->relizProverka->start = start;proverka3->relizProverka->autoMode_NY_cold_hart = autoMode_NY_cold_hart; break;
        case 3: if(proverka4 != nullptr)proverka4->relizProverka->SetIdLink(idLink.last());proverka4->relizProverka->start = start;proverka4->relizProverka->autoMode_NY_cold_hart = autoMode_NY_cold_hart; break;
        case 4: if(proverka5 != nullptr)proverka5->relizProverka->SetIdLink(idLink.last());proverka5->relizProverka->start = start;proverka5->relizProverka->autoMode_NY_cold_hart = autoMode_NY_cold_hart; break;
        case 5: if(proverka6 != nullptr)proverka6->relizProverka->SetIdLink(idLink.last());proverka6->relizProverka->start = start;proverka6->relizProverka->autoMode_NY_cold_hart = autoMode_NY_cold_hart; break;
        case 6: if(proverka7 != nullptr)proverka7->relizProverka->SetIdLink(idLink.last());proverka7->relizProverka->start = start;proverka7->relizProverka->autoMode_NY_cold_hart = autoMode_NY_cold_hart; break;
        case 7: if(proverka8 != nullptr)proverka8->relizProverka->SetIdLink(idLink.last());proverka8->relizProverka->start = start;proverka8->relizProverka->autoMode_NY_cold_hart = autoMode_NY_cold_hart; break;

        }
    }

    if(proverka_kolNP == Count_NP)
    {
        //////////////////////////////////////////////////////////////////////////////

        emit setSIGNALtype("START");
        emit signal_StartGSG();

        emit signal_clearView();
        emit signal_StartProverka(autoProverka, namberProverkaStart);
        emit startTimerMrk();
        emit startTimer();
    }
    else
    {
        qDebug () << "Not find NP in the BD!";

        //Дописать отмену загрузки имитатора
    }
}


void Proverka::slot_StartProverka_Next()
{
    //    emit signal_StartProverka();
    //    emit startTimerMrk();
    //    emit startTimer();

}

void Proverka::slot_Tick(QString time,QString index)
{

    emit signal_Tick(time);
}


const QList<QObject*> Proverka::listProverka() const
{
    return *m_ListProverkaItem;
}




void Proverka::Set_Number_Spytnik(proverkaItem* flag)
{
    int liters;

    if(flag->relizProverka->liter == 38)
    {
        liters = 129;
    }
    else
    {
        liters = flag->relizProverka->liter+1;
    }

    QDateTime stay = QDateTime::currentDateTime();

    flag->relizProverka->stay_liter = stay;

    // int kol_NP = m_ListProverkaItem->count();


    if(proverka1 != NULL)
    {

        proverka1->relizProverka->stay_liter = stay;
    }

    if(proverka2 != NULL)
    {
        proverka2->relizProverka->stay_liter = stay;
    }

    if(proverka3 != NULL)
    {
        proverka3->relizProverka->stay_liter = stay;
    }

    if(proverka4 != NULL)
    {
        proverka4->relizProverka->stay_liter = stay;
    }

    if(proverka5 != NULL)
    {
        proverka5->relizProverka->stay_liter = stay;
    }

    if(proverka6 != NULL)
    {
        proverka6->relizProverka->stay_liter = stay;
    }

    if(proverka7 != NULL)
    {
        proverka7->relizProverka->stay_liter = stay;
    }

    if(proverka8 != NULL)
    {
        proverka8->relizProverka->stay_liter = stay;
    }


    qDebug () << " Устанавливаем спутник  = " << liters;
    switch (liters) {
    case 25:
        emit signal_Gen_setSATid("R14"); //-7 liter +


        if(proverka1 != NULL)
        {

            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-7','"+proverka1->relizProverka->IdLink+"')");
        }

        if(proverka2 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-7','"+proverka2->relizProverka->IdLink+"')");
        }

        if(proverka3 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-7','"+proverka3->relizProverka->IdLink+"')");
        }


        if(proverka4 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-7','"+proverka4->relizProverka->IdLink+"')");
        }

        if(proverka5 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-7','"+proverka5->relizProverka->IdLink+"')");
        }

        if(proverka6 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-7','"+proverka6->relizProverka->IdLink+"')");
        }

        if(proverka7 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-7','"+proverka7->relizProverka->IdLink+"')");
        }

        if(proverka8 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-7','"+proverka8->relizProverka->IdLink+"')");
        }


        break;
    case 26:
        emit signal_Gen_setSATid("R18"); //-6 liter +

        if(proverka1 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-6','"+proverka1->relizProverka->IdLink+"')");
        }

        if(proverka2 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-6','"+proverka2->relizProverka->IdLink+"')");
        }

        if(proverka3 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-6','"+proverka3->relizProverka->IdLink+"')");
        }

        if(proverka4 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-6','"+proverka4->relizProverka->IdLink+"')");
        }



        if(proverka5 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-6','"+proverka5->relizProverka->IdLink+"')");
        }

        if(proverka6 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-6','"+proverka6->relizProverka->IdLink+"')");
        }

        if(proverka7 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-6','"+proverka7->relizProverka->IdLink+"')");
        }

        if(proverka8 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-6','"+proverka8->relizProverka->IdLink+"')");
        }
        break;
    case 27:
        emit signal_Gen_setSATid("R10"); //-5 liter +

        // gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-5','"+flag->relizProverka->IdLink+"')");

        if(proverka1 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-5','"+proverka1->relizProverka->IdLink+"')");
        }

        if(proverka2 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-5','"+proverka2->relizProverka->IdLink+"')");
        }

        if(proverka3 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-5','"+proverka3->relizProverka->IdLink+"')");
        }


        if(proverka4 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-5','"+proverka4->relizProverka->IdLink+"')");
        }

        if(proverka5 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-5','"+proverka5->relizProverka->IdLink+"')");
        }

        if(proverka6 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-5','"+proverka6->relizProverka->IdLink+"')");
        }

        if(proverka7 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-5','"+proverka7->relizProverka->IdLink+"')");
        }

        if(proverka8 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-5','"+proverka8->relizProverka->IdLink+"')");
        }

        break;
    case 28:
        emit signal_Gen_setSATid("R6");  //-4 liter +
        // gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-4','"+flag->relizProverka->IdLink+"')");

        if(proverka1 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-4','"+proverka1->relizProverka->IdLink+"')");
        }

        if(proverka2 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-4','"+proverka2->relizProverka->IdLink+"')");
        }

        if(proverka3 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-4','"+proverka3->relizProverka->IdLink+"')");
        }

        if(proverka4 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-4','"+proverka4->relizProverka->IdLink+"')");
        }

        if(proverka5 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-4','"+proverka5->relizProverka->IdLink+"')");
        }

        if(proverka6 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-4','"+proverka6->relizProverka->Id+"')");
        }

        if(proverka7 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-4','"+proverka7->relizProverka->Id+"')");
        }

        if(proverka8 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-4','"+proverka8->relizProverka->Id+"')");
        }

        break;
    case 29:
        emit signal_Gen_setSATid("R22"); //-3 liter +
        //  gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-3','"+flag->relizProverka->IdLink+"')");

        if(proverka1 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-3','"+proverka1->relizProverka->IdLink+"')");
        }

        if(proverka2 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-3','"+proverka2->relizProverka->IdLink+"')");
        }

        if(proverka3 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-3','"+proverka3->relizProverka->IdLink+"')");
        }

        if(proverka4 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-3','"+proverka4->relizProverka->IdLink+"')");
        }

        if(proverka5 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-3','"+proverka5->relizProverka->IdLink+"')");
        }

        if(proverka6 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-3','"+proverka6->relizProverka->IdLink+"')");
        }

        if(proverka7 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-3','"+proverka7->relizProverka->IdLink+"')");
        }

        if(proverka8 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-4','"+proverka8->relizProverka->IdLink+"')");
        }

        break;
    case 30:
        emit signal_Gen_setSATid("R13");  //-2 liter +
        //  gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-2','"+flag->relizProverka->IdLink+"')");

        if(proverka1 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-2','"+proverka1->relizProverka->IdLink+"')");
        }

        if(proverka2 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-2','"+proverka2->relizProverka->IdLink+"')");
        }

        if(proverka3 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-2','"+proverka3->relizProverka->IdLink+"')");
        }


        if(proverka4 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-2','"+proverka4->relizProverka->IdLink+"')");
        }

        if(proverka5 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-2','"+proverka5->relizProverka->IdLink+"')");
        }

        if(proverka6 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-2','"+proverka6->relizProverka->IdLink+"')");
        }

        if(proverka7 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-2','"+proverka7->relizProverka->IdLink+"')");
        }

        if(proverka8 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-2','"+proverka8->relizProverka->IdLink+"')");
        }

        break;
    case 31:
        emit signal_Gen_setSATid("R12"); //-1 liter +
        //gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-1','"+flag->relizProverka->IdLink+"')");

        if(proverka1 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-1','"+proverka1->relizProverka->IdLink+"')");
        }

        if(proverka2 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-1','"+proverka2->relizProverka->IdLink+"')");
        }

        if(proverka3 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-1','"+proverka3->relizProverka->IdLink+"')");
        }



        if(proverka4 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-1','"+proverka4->relizProverka->IdLink+"')");
        }

        if(proverka5 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-1','"+proverka5->relizProverka->IdLink+"')");
        }

        if(proverka6 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-1','"+proverka6->relizProverka->IdLink+"')");
        }

        if(proverka7 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-1','"+proverka7->relizProverka->IdLink+"')");
        }

        if(proverka8 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-1','"+proverka8->relizProverka->IdLink+"')");
        }

        break;
    case 32:
        emit signal_Gen_setSATid("R11"); //0 liter +
        //gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','0','"+flag->relizProverka->IdLink+"')");

        if(proverka1 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','0','"+proverka1->relizProverka->IdLink+"')");
        }

        if(proverka2 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','0','"+proverka2->relizProverka->IdLink+"')");
        }

        if(proverka3 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','0','"+proverka3->relizProverka->IdLink+"')");
        }

        if(proverka4 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','0','"+proverka4->relizProverka->IdLink+"')");
        }

        if(proverka5 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','0','"+proverka5->relizProverka->IdLink+"')");
        }

        if(proverka6 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','0','"+proverka6->relizProverka->IdLink+"')");
        }

        if(proverka7 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','0','"+proverka7->relizProverka->IdLink+"')");
        }

        if(proverka8 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','0','"+proverka8->relizProverka->IdLink+"')");
        }

        break;
    case 33:
        emit signal_Gen_setSATid("R5"); //1 liter +
        //gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','1','"+flag->relizProverka->IdLink+"')");

        if(proverka1 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','1','"+proverka1->relizProverka->IdLink+"')");
        }

        if(proverka2 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','1','"+proverka2->relizProverka->IdLink+"')");
        }

        if(proverka3 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','1','"+proverka3->relizProverka->IdLink+"')");
        }


        if(proverka4 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','1','"+proverka4->relizProverka->IdLink+"')");
        }

        if(proverka5 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','1','"+proverka5->relizProverka->IdLink+"')");
        }

        if(proverka6 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','1','"+proverka6->relizProverka->IdLink+"')");
        }

        if(proverka7 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','1','"+proverka7->relizProverka->IdLink+"')");
        }

        if(proverka8 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','1','"+proverka8->relizProverka->IdLink+"')");
        }

        break;
    case 34:
        emit signal_Gen_setSATid("R20"); //2 liter +
        //gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','2','"+flag->relizProverka->IdLink+"')");

        if(proverka1 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','2','"+proverka1->relizProverka->IdLink+"')");
        }

        if(proverka2 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','2','"+proverka2->relizProverka->IdLink+"')");
        }

        if(proverka3 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','2','"+proverka3->relizProverka->IdLink+"')");
        }



        if(proverka4 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','2','"+proverka4->relizProverka->IdLink+"')");
        }

        if(proverka5 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','2','"+proverka5->relizProverka->IdLink+"')");
        }

        if(proverka6 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','2','"+proverka6->relizProverka->IdLink+"')");
        }

        if(proverka7 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','2','"+proverka7->relizProverka->IdLink+"')");
        }

        if(proverka8 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','2','"+proverka8->relizProverka->IdLink+"')");
        }

        break;
    case 35:
        emit signal_Gen_setSATid("R19"); //3 liter +
        //gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','3','"+flag->relizProverka->IdLink+"')");

        if(proverka1 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','3','"+proverka1->relizProverka->IdLink+"')");
        }

        if(proverka2 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','3','"+proverka2->relizProverka->IdLink+"')");
        }

        if(proverka3 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','3','"+proverka3->relizProverka->IdLink+"')");
        }



        if(proverka4 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','3','"+proverka4->relizProverka->IdLink+"')");
        }

        if(proverka5 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','3','"+proverka5->relizProverka->IdLink+"')");
        }

        if(proverka6 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','3','"+proverka6->relizProverka->IdLink+"')");
        }

        if(proverka7 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','3','"+proverka7->relizProverka->IdLink+"')");
        }

        if(proverka8 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','3','"+proverka8->relizProverka->IdLink+"')");
        }


        break;
    case 36:
        emit signal_Gen_setSATid("R17"); //4 liter +
        //gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','4','"+flag->relizProverka->IdLink+"')");

        if(proverka1 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','4','"+proverka1->relizProverka->IdLink+"')");
        }

        if(proverka2 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','4','"+proverka2->relizProverka->IdLink+"')");
        }

        if(proverka3 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','4','"+proverka3->relizProverka->IdLink+"')");
        }

        if(proverka4 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','4','"+proverka4->relizProverka->IdLink+"')");
        }



        if(proverka5 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','4','"+proverka5->relizProverka->IdLink+"')");
        }

        if(proverka6 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','4','"+proverka6->relizProverka->IdLink+"')");
        }

        if(proverka7 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','4','"+proverka7->relizProverka->IdLink+"')");
        }

        if(proverka8 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','4','"+proverka8->relizProverka->IdLink+"')");
        }

        break;
    case 37:
        emit signal_Gen_setSATid("R7"); //5 liter +
        //gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','5','"+flag->relizProverka->IdLink+"')");
        if(proverka1 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','5','"+proverka1->relizProverka->IdLink+"')");
        }

        if(proverka2 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','5','"+proverka2->relizProverka->IdLink+"')");
        }

        if(proverka3 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','5','"+proverka3->relizProverka->IdLink+"')");
        }


        if(proverka4 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','5','"+proverka4->relizProverka->IdLink+"')");
        }

        if(proverka5 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','5','"+proverka5->relizProverka->IdLink+"')");
        }

        if(proverka6 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','5','"+proverka6->relizProverka->IdLink+"')");
        }

        if(proverka7 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','5','"+proverka7->relizProverka->IdLink+"')");
        }

        if(proverka8 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','5','"+proverka8->relizProverka->IdLink+"')");
        }

        break;
    case 38:
        emit signal_Gen_setSATid("R8"); //6 liter +
        //gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','6','"+flag->relizProverka->IdLink+"')");

        if(proverka1 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','6','"+proverka1->relizProverka->IdLink+"')");
        }

        if(proverka2 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','6','"+proverka2->relizProverka->IdLink+"')");
        }

        if(proverka3 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','6','"+proverka3->relizProverka->IdLink+"')");
        }



        if(proverka4 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','6','"+proverka4->relizProverka->IdLink+"')");
        }

        if(proverka5 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','6','"+proverka5->relizProverka->IdLink+"')");
        }

        if(proverka6 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','6','"+proverka6->relizProverka->IdLink+"')");
        }

        if(proverka7 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','6','"+proverka7->relizProverka->IdLink+"')");
        }

        if(proverka8 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','6','"+proverka8->relizProverka->IdLink+"')");
        }
        break;
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    case 129:
        emit signal_Gen_setSATid("G1"); //G1 liter +
        //gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G1','"+flag->relizProverka->IdLink+"')");

        if(proverka1 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G1','"+proverka1->relizProverka->IdLink+"')");
        }

        if(proverka2 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G1','"+proverka2->relizProverka->IdLink+"')");
        }

        if(proverka3 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G1','"+proverka3->relizProverka->IdLink+"')");
        }

        if(proverka4 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G1','"+proverka4->relizProverka->IdLink+"')");
        }



        if(proverka5 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G1','"+proverka5->relizProverka->IdLink+"')");
        }

        if(proverka6 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G1','"+proverka6->relizProverka->IdLink+"')");
        }

        if(proverka7 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G1','"+proverka7->relizProverka->IdLink+"')");
        }

        if(proverka8 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G1','"+proverka8->relizProverka->IdLink+"')");
        }


        break;
    case 130:
        emit signal_Gen_setSATid("G2"); //G2 liter +
        //gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G2','"+flag->relizProverka->IdLink+"')");

        if(proverka1 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G2','"+proverka1->relizProverka->IdLink+"')");
        }

        if(proverka2 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G2','"+proverka2->relizProverka->IdLink+"')");
        }

        if(proverka3 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G2','"+proverka3->relizProverka->IdLink+"')");
        }



        if(proverka4 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G2','"+proverka4->relizProverka->IdLink+"')");
        }

        if(proverka5 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G2','"+proverka5->relizProverka->IdLink+"')");
        }

        if(proverka6 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G2','"+proverka6->relizProverka->IdLink+"')");
        }

        if(proverka7 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G2','"+proverka7->relizProverka->IdLink+"')");
        }

        if(proverka8 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G2','"+proverka8->relizProverka->IdLink+"')");
        }


        break;
    case 131:
        emit signal_Gen_setSATid("G3"); //G3 liter +
        //gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G3','"+flag->relizProverka->IdLink+"')");

        if(proverka1 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G3','"+proverka1->relizProverka->IdLink+"')");
        }

        if(proverka2 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G3','"+proverka2->relizProverka->IdLink+"')");
        }

        if(proverka3 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G3','"+proverka3->relizProverka->IdLink+"')");
        }



        if(proverka4 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G3','"+proverka4->relizProverka->IdLink+"')");
        }

        if(proverka5 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G3','"+proverka5->relizProverka->IdLink+"')");
        }

        if(proverka6 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G3','"+proverka6->relizProverka->IdLink+"')");
        }

        if(proverka7 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G3','"+proverka7->relizProverka->IdLink+"')");
        }

        if(proverka8 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G3','"+proverka8->relizProverka->IdLink+"')");
        }

        break;
    case 132:
        emit signal_Gen_setSATid("G4"); //G4 liter +
        //gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G4','"+flag->relizProverka->IdLink+"')");

        if(proverka1 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G4','"+proverka1->relizProverka->IdLink+"')");
        }

        if(proverka2 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G4','"+proverka2->relizProverka->IdLink+"')");
        }

        if(proverka3 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G4','"+proverka3->relizProverka->IdLink+"')");
        }



        if(proverka4 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G4','"+proverka4->relizProverka->IdLink+"')");
        }

        if(proverka5 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G4','"+proverka5->relizProverka->IdLink+"')");
        }

        if(proverka6 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G4','"+proverka6->relizProverka->IdLink+"')");
        }

        if(proverka7 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G4','"+proverka7->relizProverka->IdLink+"')");
        }

        if(proverka8 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G4','"+proverka8->relizProverka->IdLink+"')");
        }

        break;
    case 133:
        emit signal_Gen_setSATid("G5"); //G5 liter +
        //gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G5','"+flag->relizProverka->IdLink+"')");

        if(proverka1 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G5','"+proverka1->relizProverka->IdLink+"')");
        }

        if(proverka2 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G5','"+proverka2->relizProverka->IdLink+"')");
        }

        if(proverka3 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G5','"+proverka3->relizProverka->IdLink+"')");
        }


        if(proverka4 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G5','"+proverka4->relizProverka->IdLink+"')");
        }

        if(proverka5 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G5','"+proverka5->relizProverka->IdLink+"')");
        }

        if(proverka6 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G5','"+proverka6->relizProverka->IdLink+"')");
        }

        if(proverka7 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G5','"+proverka7->relizProverka->IdLink+"')");
        }

        if(proverka8 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G5','"+proverka8->relizProverka->IdLink+"')");
        }

        break;
    case 134:
        emit signal_Gen_setSATid("G6"); //G6 liter +
        //gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G6','"+flag->relizProverka->IdLink+"')");

        if(proverka1 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G6','"+proverka1->relizProverka->IdLink+"')");
        }

        if(proverka2 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G6','"+proverka2->relizProverka->IdLink+"')");
        }

        if(proverka3 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G6','"+proverka3->relizProverka->IdLink+"')");
        }



        if(proverka4 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G6','"+proverka4->relizProverka->IdLink+"')");
        }

        if(proverka5 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G6','"+proverka5->relizProverka->IdLink+"')");
        }

        if(proverka6 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G6','"+proverka6->relizProverka->IdLink+"')");
        }

        if(proverka7 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G6','"+proverka7->relizProverka->IdLink+"')");
        }

        if(proverka8 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G6','"+proverka8->relizProverka->IdLink+"')");
        }

        break;
    case 135:
        emit signal_Gen_setSATid("G7"); //G7 liter +
        //gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G7','"+flag->relizProverka->IdLink+"')");

        if(proverka1 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G7','"+proverka1->relizProverka->IdLink+"')");
        }

        if(proverka2 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G7','"+proverka2->relizProverka->IdLink+"')");
        }

        if(proverka3 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G7','"+proverka3->relizProverka->IdLink+"')");
        }


        if(proverka4 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G7','"+proverka4->relizProverka->IdLink+"')");
        }

        if(proverka5 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G7','"+proverka5->relizProverka->IdLink+"')");
        }

        if(proverka6 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G7','"+proverka6->relizProverka->IdLink+"')");
        }

        if(proverka7 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G7','"+proverka7->relizProverka->IdLink+"')");
        }

        if(proverka8 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G7','"+proverka8->relizProverka->IdLink+"')");
        }

        break;
    case 136:
        emit signal_Gen_setSATid("G8"); //G8 liter +
        //gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G8','"+flag->relizProverka->IdLink+"')");

        if(proverka1 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G8','"+proverka1->relizProverka->IdLink+"')");
        }

        if(proverka2 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G8','"+proverka2->relizProverka->IdLink+"')");
        }

        if(proverka3 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G8','"+proverka3->relizProverka->IdLink+"')");
        }


        if(proverka4 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G8','"+proverka4->relizProverka->IdLink+"')");
        }

        if(proverka5 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G8','"+proverka5->relizProverka->IdLink+"')");
        }

        if(proverka6 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G8','"+proverka6->relizProverka->IdLink+"')");
        }

        if(proverka7 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G8','"+proverka7->relizProverka->IdLink+"')");
        }

        if(proverka8 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G8','"+proverka8->relizProverka->IdLink+"')");
        }


        break;
    case 137:
        emit signal_Gen_setSATid("G9"); //G9 liter +
        //gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G9','"+flag->relizProverka->IdLink+"')");

        if(proverka1 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G9','"+proverka1->relizProverka->IdLink+"')");
        }

        if(proverka2 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G9','"+proverka2->relizProverka->IdLink+"')");
        }

        if(proverka3 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G9','"+proverka3->relizProverka->IdLink+"')");
        }



        if(proverka4 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G9','"+proverka4->relizProverka->IdLink+"')");
        }

        if(proverka5 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G9','"+proverka5->relizProverka->IdLink+"')");
        }

        if(proverka6 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G9','"+proverka6->relizProverka->IdLink+"')");
        }

        if(proverka7 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G9','"+proverka7->relizProverka->IdLink+"')");
        }

        if(proverka8 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G9','"+proverka8->relizProverka->IdLink+"')");
        }


        break;
    case 138:
        emit signal_Gen_setSATid("G10"); //G10 liter +
        //gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G10','"+flag->relizProverka->IdLink+"')");

        if(proverka1 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G10','"+proverka1->relizProverka->IdLink+"')");
        }

        if(proverka2 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G10','"+proverka2->relizProverka->IdLink+"')");
        }

        if(proverka3 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G10','"+proverka3->relizProverka->IdLink+"')");
        }



        if(proverka4 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G10','"+proverka4->relizProverka->IdLink+"')");
        }

        if(proverka5 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G10','"+proverka5->relizProverka->IdLink+"')");
        }

        if(proverka6 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G10','"+proverka6->relizProverka->IdLink+"')");
        }

        if(proverka7 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G10','"+proverka7->relizProverka->IdLink+"')");
        }

        if(proverka8 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G10','"+proverka8->relizProverka->IdLink+"')");
        }

        break;
    case 139:
        emit signal_Gen_setSATid("G11"); //G11 liter +
        //gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G11','"+flag->relizProverka->IdLink+"')");

        if(proverka1 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G11','"+proverka1->relizProverka->IdLink+"')");
        }

        if(proverka2 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G11','"+proverka2->relizProverka->IdLink+"')");
        }

        if(proverka3 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G11','"+proverka3->relizProverka->IdLink+"')");
        }


        if(proverka4 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G11','"+proverka4->relizProverka->IdLink+"')");
        }

        if(proverka5 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G11','"+proverka5->relizProverka->IdLink+"')");
        }

        if(proverka6 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G11','"+proverka6->relizProverka->IdLink+"')");
        }

        if(proverka7 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G1','"+proverka7->relizProverka->IdLink+"')");
        }

        if(proverka8 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G11','"+proverka8->relizProverka->IdLink+"')");
        }

        break;
    case 140:
        emit signal_Gen_setSATid("G12"); //G12 liter +
        //gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G12','"+flag->relizProverka->IdLink+"')");

        if(proverka1 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G12','"+proverka1->relizProverka->IdLink+"')");
        }

        if(proverka2 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G12','"+proverka2->relizProverka->IdLink+"')");
        }

        if(proverka3 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G12','"+proverka3->relizProverka->IdLink+"')");
        }

        if(proverka4 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G12','"+proverka4->relizProverka->IdLink+"')");
        }



        if(proverka5 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G12','"+proverka5->relizProverka->IdLink+"')");
        }

        if(proverka6 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G1','"+proverka6->relizProverka->IdLink+"')");
        }

        if(proverka7 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G12','"+proverka7->relizProverka->IdLink+"')");
        }

        if(proverka8 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G12','"+proverka8->relizProverka->IdLink+"')");
        }

        break;
    case 141:
        emit signal_Gen_setSATid("G13"); //G13 liter +
        //gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G13','"+flag->relizProverka->IdLink+"')");

        if(proverka1 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G13','"+proverka1->relizProverka->IdLink+"')");
        }

        if(proverka2 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G13','"+proverka2->relizProverka->IdLink+"')");
        }

        if(proverka3 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G13','"+proverka3->relizProverka->IdLink+"')");
        }

        if(proverka4 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G13','"+proverka4->relizProverka->IdLink+"')");
        }



        if(proverka5 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G13','"+proverka5->relizProverka->IdLink+"')");
        }

        if(proverka6 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G13','"+proverka6->relizProverka->IdLink+"')");
        }

        if(proverka7 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G13','"+proverka7->relizProverka->IdLink+"')");
        }

        if(proverka8 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G13','"+proverka8->relizProverka->IdLink+"')");
        }


        break;
    case 142:
        emit signal_Gen_setSATid("G14"); //G14 liter +
        //gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G14','"+flag->relizProverka->IdLink+"')");
        if(proverka1 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G14','"+proverka1->relizProverka->IdLink+"')");
        }

        if(proverka2 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G14','"+proverka2->relizProverka->IdLink+"')");
        }

        if(proverka3 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G14','"+proverka3->relizProverka->IdLink+"')");
        }



        if(proverka4 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G14','"+proverka4->relizProverka->IdLink+"')");
        }

        if(proverka5 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G14','"+proverka5->relizProverka->IdLink+"')");
        }

        if(proverka6 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G14','"+proverka6->relizProverka->IdLink+"')");
        }

        if(proverka7 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G14','"+proverka7->relizProverka->IdLink+"')");
        }

        if(proverka8 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G14','"+proverka8->relizProverka->IdLink+"')");
        }

        break;
    case 143:
        emit signal_Gen_setSATid("G15"); //G15 liter +
        //gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G15','"+flag->relizProverka->IdLink+"')");

        if(proverka1 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G15','"+proverka1->relizProverka->IdLink+"')");
        }

        if(proverka2 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G15','"+proverka2->relizProverka->IdLink+"')");
        }

        if(proverka3 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G15','"+proverka3->relizProverka->IdLink+"')");
        }

        if(proverka4 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G15','"+proverka4->relizProverka->IdLink+"')");
        }

        if(proverka5 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G15','"+proverka5->relizProverka->IdLink+"')");
        }

        if(proverka6 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G15','"+proverka6->relizProverka->IdLink+"')");
        }

        if(proverka7 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G15','"+proverka7->relizProverka->IdLink+"')");
        }

        if(proverka8 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G15','"+proverka8->relizProverka->IdLink+"')");
        }

        break;
    case 144:
        emit signal_Gen_setSATid("G16"); //G16 liter +
        //gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G16','"+flag->relizProverka->IdLink+"')");

        if(proverka1 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G16','"+proverka1->relizProverka->IdLink+"')");
        }

        if(proverka2 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G16','"+proverka2->relizProverka->IdLink+"')");
        }

        if(proverka3 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G16','"+proverka3->relizProverka->IdLink+"')");
        }

        if(proverka4 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G16','"+proverka4->relizProverka->IdLink+"')");
        }

        if(proverka5 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G16','"+proverka5->relizProverka->IdLink+"')");
        }

        if(proverka6 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G16','"+proverka6->relizProverka->IdLink+"')");
        }

        if(proverka7 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G16','"+proverka7->relizProverka->IdLink+"')");
        }

        if(proverka8 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G16','"+proverka8->relizProverka->IdLink+"')");
        }

        break;
    case 145:
        emit signal_Gen_setSATid("G17"); //G17 liter +
        //gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G17','"+flag->relizProverka->IdLink+"')");

        if(proverka1 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G17','"+proverka1->relizProverka->IdLink+"')");
        }

        if(proverka2 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G17','"+proverka2->relizProverka->IdLink+"')");
        }

        if(proverka3 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G17','"+proverka3->relizProverka->IdLink+"')");
        }

        if(proverka4 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G17','"+proverka4->relizProverka->IdLink+"')");
        }

        if(proverka5 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G17','"+proverka5->relizProverka->IdLink+"')");
        }

        if(proverka6 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G17','"+proverka6->relizProverka->IdLink+"')");
        }

        if(proverka7 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G17','"+proverka7->relizProverka->IdLink+"')");
        }

        if(proverka8 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G17','"+proverka8->relizProverka->IdLink+"')");
        }

        break;
    case 146:
        emit signal_Gen_setSATid("G18"); //G18 liter +
        //gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G18','"+flag->relizProverka->IdLink+"')");

        if(proverka1 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G18','"+proverka1->relizProverka->IdLink+"')");
        }

        if(proverka2 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G18','"+proverka2->relizProverka->IdLink+"')");
        }

        if(proverka3 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G18','"+proverka3->relizProverka->IdLink+"')");
        }

        if(proverka4 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G18','"+proverka4->relizProverka->IdLink+"')");
        }

        if(proverka5 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G18','"+proverka5->relizProverka->IdLink+"')");
        }

        if(proverka6 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G18','"+proverka6->relizProverka->IdLink+"')");
        }

        if(proverka7 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G18','"+proverka7->relizProverka->IdLink+"')");
        }

        if(proverka8 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G18','"+proverka8->relizProverka->IdLink+"')");
        }

        break;
    case 147:
        emit signal_Gen_setSATid("G19"); //G19 liter +
        //gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G19','"+flag->relizProverka->IdLink+"')");

        if(proverka1 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G19','"+proverka1->relizProverka->IdLink+"')");
        }

        if(proverka2 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G19','"+proverka2->relizProverka->IdLink+"')");
        }

        if(proverka3 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G19','"+proverka3->relizProverka->IdLink+"')");
        }

        if(proverka4 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G19','"+proverka4->relizProverka->IdLink+"')");
        }

        if(proverka5 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G19','"+proverka5->relizProverka->IdLink+"')");
        }

        if(proverka6 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G19','"+proverka6->relizProverka->IdLink+"')");
        }

        if(proverka7 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G19','"+proverka7->relizProverka->IdLink+"')");
        }

        if(proverka8 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G19','"+proverka8->relizProverka->IdLink+"')");
        }

        break;
    case 148:
        emit signal_Gen_setSATid("G20"); //G20 liter +
        //gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G20','"+flag->relizProverka->IdLink+"')");

        if(proverka1 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G20','"+proverka1->relizProverka->IdLink+"')");
        }

        if(proverka2 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G20','"+proverka2->relizProverka->IdLink+"')");
        }

        if(proverka3 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G20','"+proverka3->relizProverka->IdLink+"')");
        }


        if(proverka4 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G20','"+proverka4->relizProverka->IdLink+"')");
        }

        if(proverka5 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G1','"+proverka5->relizProverka->IdLink+"')");
        }

        if(proverka6 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G20','"+proverka6->relizProverka->IdLink+"')");
        }

        if(proverka7 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G20','"+proverka7->relizProverka->IdLink+"')");
        }

        if(proverka8 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G20','"+proverka8->relizProverka->IdLink+"')");
        }

        break;
    case 149:
        emit signal_Gen_setSATid("G21"); //G21 liter +
        // gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G21','"+flag->relizProverka->IdLink+"')");

        if(proverka1 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G21','"+proverka1->relizProverka->IdLink+"')");
        }

        if(proverka2 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G21','"+proverka2->relizProverka->IdLink+"')");
        }

        if(proverka3 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G21','"+proverka3->relizProverka->IdLink+"')");
        }


        if(proverka4 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G21','"+proverka4->relizProverka->IdLink+"')");
        }

        if(proverka5 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G21','"+proverka5->relizProverka->IdLink+"')");
        }

        if(proverka6 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G21','"+proverka6->relizProverka->IdLink+"')");
        }

        if(proverka7 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G21','"+proverka7->relizProverka->IdLink+"')");
        }

        if(proverka8 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G21','"+proverka8->relizProverka->IdLink+"')");
        }

        break;
    case 150:
        emit signal_Gen_setSATid("G22"); //G22 liter +
        //gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G22','"+flag->relizProverka->IdLink+"')");

        if(proverka1 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G22','"+proverka1->relizProverka->IdLink+"')");
        }

        if(proverka2 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G22','"+proverka2->relizProverka->IdLink+"')");
        }

        if(proverka3 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G22','"+proverka3->relizProverka->IdLink+"')");
        }

        if(proverka4 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G22','"+proverka4->relizProverka->IdLink+"')");
        }

        if(proverka5 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G22','"+proverka5->relizProverka->IdLink+"')");
        }

        if(proverka6 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G22','"+proverka6->relizProverka->IdLink+"')");
        }

        if(proverka7 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G22','"+proverka7->relizProverka->IdLink+"')");
        }

        if(proverka8 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G22','"+proverka8->relizProverka->IdLink+"')");
        }
        break;
    case 151:
        emit signal_Gen_setSATid("G23"); //G23 liter +
        // gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G23','"+flag->relizProverka->IdLink+"')");

        if(proverka1 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G23','"+proverka1->relizProverka->IdLink+"')");
        }

        if(proverka2 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G23','"+proverka2->relizProverka->IdLink+"')");
        }

        if(proverka3 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G23','"+proverka3->relizProverka->IdLink+"')");
        }


        if(proverka4 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G23','"+proverka4->relizProverka->IdLink+"')");
        }

        if(proverka5 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G23','"+proverka5->relizProverka->IdLink+"')");
        }

        if(proverka6 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G23','"+proverka6->relizProverka->IdLink+"')");
        }

        if(proverka7 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G23','"+proverka7->relizProverka->IdLink+"')");
        }

        if(proverka8 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G23','"+proverka8->relizProverka->IdLink+"')");
        }

        break;
    case 152:
        emit signal_Gen_setSATid("G24"); //G24 liter +
        // gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G24','"+flag->relizProverka->IdLink+"')");

        if(proverka1 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G24','"+proverka1->relizProverka->IdLink+"')");
        }

        if(proverka2 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G24','"+proverka2->relizProverka->IdLink+"')");
        }

        if(proverka3 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G24','"+proverka3->relizProverka->IdLink+"')");
        }

        if(proverka4 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G24','"+proverka4->relizProverka->IdLink+"')");
        }


        if(proverka5 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G24','"+proverka5->relizProverka->IdLink+"')");
        }

        if(proverka6 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G24','"+proverka6->relizProverka->IdLink+"')");
        }

        if(proverka7 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G24','"+proverka7->relizProverka->IdLink+"')");
        }

        if(proverka8 != NULL)
        {
            gsg->BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G24','"+proverka8->relizProverka->IdLink+"')");
        }

        break;
    default:
        break;
    }
}

void Proverka::SetBD(BData *bd)
{
    BD = bd;


}





////////////////////////////////////////////////////////////////////////////////////////////////////

void Proverka::slot_EndProverka(int index)
{

    qDebug () << "slot_EndProverka = " << index;

    start = 0;

    switch (index)
    {
    case 1:

        for(int i=0;i <Count_NP;i++) // for(int i=0;i <2;i++)
        {
            proverkaItem* flag = (proverkaItem*)m_ListProverkaItem->value(i);


            if( flag->relizProverka->job == false)
            {
                start--;


                if(flag->relizProverka->Good)
                {
                    flag->relizProverkaTimer->signal_GoodQML(true);
                    qDebug () << "Вызов переключения: " << QString::number(index);
                    emit signal_GoodQML(QString::number(index));
                }
                else
                {
                    flag->relizProverkaTimer->signal_GoodQML(false);
                }

                emit flag->relizProverka->signal_IndexProverka(QString::number(flag->relizProverka->countProverka));

            }

            start++;
        }


        if(start <=0)
        {
            start = 0 ;

            proverkaItem* flag;
            for(int i=0;i <Count_NP;i++)
            {
                flag = (proverkaItem*)m_ListProverkaItem->value(i);

                flag->relizProverka->job  = true;


                //                if(flag->relizProverka->Good)
                //                {
                //                    flag->relizProverkaTimer->signal_GoodQML(true);
                //                    qDebug () << "Вызов переключения: " << QString::number(index);
                //                    emit signal_GoodQML(QString::number(index));
                //                }
                //                else
                //                {
                //                    flag->relizProverka->job = false;
                //                    flag->relizProverkaTimer->signal_GoodQML(false);
                //                }

            }


            if(flag->relizProverka->flag_auto == false)
            {
                endOneProverka();

            }

            emit signal_StartProverka_2();

        }
        else
        {
            //  emit signal_BadQML(proverka1->relizProverka->countProverka);
        }

        break;
    case 2:
        qDebug () << "signal_StartProverka_2();";

        for(int i=0;i <Count_NP;i++)
        {
            proverkaItem* flag = (proverkaItem*)m_ListProverkaItem->value(i);


            if( flag->relizProverka->job == false)
            {
                start--;

                if(flag->relizProverka->proverka2_part >=1)
                {
                    int proverka =0;

                    for(int i=0; i < flag->relizProverka->list_flag_good_2[i];i++)
                    {
                        proverka++;

                        if(flag->relizProverka->list_flag_good_2[i] == true)
                        {
                            proverka--;
                        }
                    }

                    if(proverka <=0)
                    {
                        flag->relizProverkaTimer->signal_GoodQML(true);

                        qDebug () << "signal_GoodQML = " << flag->relizProverka->flag_good_2  <<  " and " <<  flag->relizProverka->proverka2_part;

                        emit signal_GoodQML(QString::number(index));
                    }
                    else
                    {
                        flag->relizProverkaTimer->signal_GoodQML(false);
                    }

                    emit flag->relizProverka->signal_IndexProverka(QString::number(flag->relizProverka->countProverka));
                }
            }

            start++;
        }

        if(start <=0)
        {
            qDebug () << start;

            //  emit signal_GoodQML(proverka1->relizProverka->countProverka);
            start = 0 ;
            proverkaItem* flag;

            for(int i=0;i <Count_NP;i++)
            {
                flag = (proverkaItem*)m_ListProverkaItem->value(i);

                flag->relizProverka->job = true;


            }

            if(flag->relizProverka->flag_auto == false)
            {
                endOneProverka();

            }


            emit signal_StartProverka_2();
        }
        else
        {
            //  emit signal_BadQML(proverka1->relizProverka->countProverka);
        }
        break;
    case 3:
        qDebug () << "signal_StartProverka_3();";

        for(int i=0;i <Count_NP;i++)
        {
            proverkaItem* flag = (proverkaItem*)m_ListProverkaItem->value(i);


            if( flag->relizProverka->job == false)
            {
                start--;

                if(flag->relizProverka->proverka2_part >=1)
                {
                    int proverka =0;

                    for(int i=0; i < flag->relizProverka->list_flag_good_3[i];i++)
                    {
                        proverka++;

                        if(flag->relizProverka->list_flag_good_3[i] == true)
                        {
                            proverka--;
                        }
                    }

                    if(proverka <=0)
                    {
                        flag->relizProverkaTimer->signal_GoodQML(true);

                        qDebug () << "signal_GoodQML = " << flag->relizProverka->flag_good_3  <<  " and " <<  flag->relizProverka->proverka2_part;

                        emit signal_GoodQML(QString::number(index));
                    }
                    else
                    {
                        flag->relizProverkaTimer->signal_GoodQML(false);
                    }

                    emit flag->relizProverka->signal_IndexProverka(QString::number(flag->relizProverka->countProverka));
                }
            }

            start++;
        }

        if(start <=0)
        {
            qDebug () << start;

            //  emit signal_GoodQML(proverka1->relizProverka->countProverka);
            start = 0 ;
            proverkaItem* flag;

            for(int i=0;i <Count_NP;i++)
            {
                flag = (proverkaItem*)m_ListProverkaItem->value(i);

                flag->relizProverka->job = true;

            }

            if(flag->relizProverka->flag_auto == false)
            {
                endOneProverka();

            }

            emit signal_StartProverka_2();

        }
        else
        {
            //  emit signal_BadQML(proverka1->relizProverka->countProverka);
        }
        break;
    case 4:
        for(int i=0;i <Count_NP;i++)
        {
            proverkaItem* flag = (proverkaItem*)m_ListProverkaItem->value(i);


            if( flag->relizProverka->job == false)
            {
                start--;

                if(flag->relizProverka->flag_good_4)
                {
                    flag->relizProverkaTimer->signal_GoodQML(true);
                    qDebug () << "Вызов переключения: " << QString::number(index);
                    emit signal_GoodQML(QString::number(index));
                }
                else
                {
                    flag->relizProverkaTimer->signal_GoodQML(false);
                }

                emit flag->relizProverka->signal_IndexProverka(QString::number(flag->relizProverka->countProverka));
            }

            start++;
        }

        if(start <=0)
        {
            qDebug () << start;

            emit setSIGNALtype("STOP");


            proverkaItem* flag;


            //  emit signal_GoodQML(proverka1->relizProverka->countProverka);
            start = 0 ;
            for(int i=0;i <Count_NP;i++)
            {
                flag = (proverkaItem*)m_ListProverkaItem->value(i);

                flag->relizProverka->job = true;

            }

            if(flag->relizProverka->flag_auto == false)
            {
                endOneProverka();

            }

            emit signal_StartProverka_5();
        }
        else
        {
            //  emit signal_BadQML(proverka1->relizProverka->countProverka);
        }
        break;
    case 5:
        for(int i=0;i <Count_NP;i++)
        {
            proverkaItem* flag = (proverkaItem*)m_ListProverkaItem->value(i);


            if( flag->relizProverka->job == false)
            {
                start--;
            }

            start++;
        }

        if(start <=0)
        {
            qDebug () << start;

            emit signal_Gen_setCONTrol("STOP");


            proverkaItem* flag = nullptr;

            start = 0 ;
            for(int i=0;i <Count_NP;i++)
            {
                flag = (proverkaItem*)m_ListProverkaItem->value(i);

                flag->relizProverka->job = true;

                if(flag->relizProverka->proverka5_part >=36)
                {
                    int proverka = 0;

                    for(int i =0; i < flag->relizProverka->list_flag_good_5.count(); i++)
                    {
                        proverka++;
                        if(flag->relizProverka->list_flag_good_5[i] == true)
                        {
                            proverka--;
                        }

                    }

                    if(proverka == 0)
                    {
                        flag->relizProverkaTimer->signal_GoodQML(true);
                        qDebug () << "Вызов переключения: " << QString::number(index);
                        emit signal_GoodQML(QString::number(index));
                    }
                    else
                    {
                        flag->relizProverkaTimer->signal_GoodQML(false);
                    }

                }

                if(flag->relizProverka->flag_good_5 == false && flag->relizProverka->proverka5_part < 36)
                {
                    flag->relizProverka->job = false;
                }

            }

            qDebug () << "ENd Proverka";
            qDebug () << "Литера: " << flag->relizProverka->liter - 32;


            //Окончание проверки
            if(flag->relizProverka->liter >=151)
            {
                emit signal_StartProverka_5();
                break;
            }


            if(flag->relizProverka->liter < 38)
            {
                Set_Number_Spytnik(flag);
            }
            else
            {
                if(flag->relizProverka->liter == 38)
                {
                    Set_Number_Spytnik(flag);
                }
                else
                {
                    Set_Number_Spytnik(flag);
                }
            }


            if(flag->relizProverka->flag_auto == false)
            {
                endOneProverka();

            }
            else
            {
                emit signal_Gen_setCONTrol("START");

                emit signal_StartProverka_5();
            }






        }
        else
        {
            //  emit signal_BadQML(proverka1->relizProverka->countProverka);
        }
        break;


        ///////////////////////
    case 7:
        for(int i=0;i <Count_NP;i++)
        {
            proverkaItem* flag = (proverkaItem*)m_ListProverkaItem->value(i);


            if( flag->relizProverka->job == false)
            {
                start--;

                if(flag->relizProverka->flag_good_7)
                {
                    flag->relizProverkaTimer->signal_GoodQML(true);
                    qDebug () << "Вызов переключения: " << QString::number(index);
                    emit signal_GoodQML(QString::number(index));
                }
                else
                {
                    flag->relizProverkaTimer->signal_GoodQML(false);
                }

                emit flag->relizProverka->signal_IndexProverka(QString::number(flag->relizProverka->countProverka));
            }

            start++;
        }

        if(start <=0)
        {
            qDebug () << start;

            //  emit setSIGNALtype("STOP");


            proverkaItem* flag;


            //  emit signal_GoodQML(proverka1->relizProverka->countProverka);
            start = 0 ;
            for(int i=0;i <Count_NP;i++)
            {
                flag = (proverkaItem*)m_ListProverkaItem->value(i);

                flag->relizProverka->job = true;


            }

            if(flag->relizProverka->flag_auto == false)
            {
                endOneProverka();

            }

            emit signal_Next();

            // emit signal_StartProverka_5();
        }
        else
        {
            //  emit signal_BadQML(proverka1->relizProverka->countProverka);
        }
        break;

        ////////////////////////////////

    case 8:
        for(int i=0;i <Count_NP;i++)
        {
            proverkaItem* flag = (proverkaItem*)m_ListProverkaItem->value(i);


            if( flag->relizProverka->job == false)
            {
                start--;

                if(flag->relizProverka->flag_good_8)
                {
                    flag->relizProverkaTimer->signal_GoodQML(true);
                    qDebug () << "Вызов переключения: " << QString::number(index);
                    emit signal_GoodQML(QString::number(index));
                }
                else
                {
                    flag->relizProverkaTimer->signal_GoodQML(false);
                }

                emit flag->relizProverka->signal_IndexProverka(QString::number(flag->relizProverka->countProverka));
            }

            start++;
        }

        if(start <=0)
        {
            qDebug () << start;

            //   emit setSIGNALtype("STOP");


            proverkaItem* flag;


            //  emit signal_GoodQML(proverka1->relizProverka->countProverka);
            start = 0 ;
            for(int i=0;i <Count_NP;i++)
            {
                flag = (proverkaItem*)m_ListProverkaItem->value(i);

                flag->relizProverka->job = true;


            }

            if(flag->relizProverka->flag_auto == false)
            {
                endOneProverka();

            }

            emit signal_Next();

            // emit signal_StartProverka_5();
        }
        else
        {
            //  emit signal_BadQML(proverka1->relizProverka->countProverka);
        }
        break;

        /////////////////
    case 9:
        for(int i=0;i <Count_NP;i++)
        {
            proverkaItem* flag = (proverkaItem*)m_ListProverkaItem->value(i);

            if( flag->relizProverka->job == false)
            {
                start--;

                if(flag->relizProverka->flag_good_9)
                {
                    flag->relizProverkaTimer->signal_GoodQML(true);
                    qDebug () << "Вызов переключения: " << QString::number(index);
                    emit signal_GoodQML(QString::number(index));
                }
                else
                {
                    flag->relizProverkaTimer->signal_GoodQML(false);
                }

                emit flag->relizProverka->signal_IndexProverka(QString::number(flag->relizProverka->countProverka));
            }

            start++;
        }

        if(start <=0)
        {
            qDebug () << start;

            //  emit setSIGNALtype("STOP");


            proverkaItem* flag;

            start = 0 ;

            for(int i=0;i <Count_NP;i++)
            {
                flag = (proverkaItem*)m_ListProverkaItem->value(i);
                flag->relizProverka->job = true;


            }


            if(flag->relizProverka->flag_auto == false)
            {
                endOneProverka();

            }

            emit signal_Next();

            // emit signal_StartProverka_5();

        }
        else
        {
            //  emit signal_BadQML(proverka1->relizProverka->countProverka);
        }
        break;

        /////////////////
    case 10:
        for(int i=0;i <Count_NP;i++)
        {
            proverkaItem* flag = (proverkaItem*)m_ListProverkaItem->value(i);


            if( flag->relizProverka->job == false)
            {
                start--;

                if(flag->relizProverka->flag_good_10)
                {
                    flag->relizProverkaTimer->signal_GoodQML(true);
                    // qDebug () << "Вызов переключения: " << QString::number(index);
                    //emit signal_GoodQML(QString::number(index));
                }
                else
                {
                    flag->relizProverkaTimer->signal_GoodQML(false);
                }

                emit flag->relizProverka->signal_IndexProverka(QString::number(flag->relizProverka->countProverka));
            }

            start++;
        }

        if(start <=0)
        {
            qDebug () << start;

            emit setSIGNALtype("STOP");


            proverkaItem* flag;


            //  emit signal_GoodQML(proverka1->relizProverka->countProverka);

            start = 0 ;

            for(int i=0;i <Count_NP;i++)
            {
                flag = (proverkaItem*)m_ListProverkaItem->value(i);
                flag->relizProverka->job = true;


            }

            if(flag->relizProverka->flag_auto == false)
            {
                endOneProverka();
            }
            else
            {
                qDebug() << "LOL 3 signal_startColdHard = " << autoMode_NY_cold_hart;
                // emit signal_startColdHard();

                //WARNING
                //Дописать код обнуления информации на странице

                emit signal_clearView();


                if(flagWork_Dont_PSIandRegylirovka)
                {
                    autoMode_NY_cold_hart = 5;
                }

                switch (autoMode_NY_cold_hart)
                {
                //запустить камеру для НУ
                case 0: emit signal_startNY();break; //Устанавливаем НУ

                    //запустить камеру для охлаждения
                case 1:  emit signal_startCold();break; //Устанавливаем -40
                    //запустить камеру для НУ
                case 2: emit signal_startNY(); break; //Устанавливаем НУ после -40
                    //запустить камеру для нагрева
                case 3: emit signal_startHord(); break; //Устанавливаем +50
                    //запустить камеру для НУ
                case 4: emit signal_startNY(); break; //Устанавливаем НУ после +50

                case 5:
                {
                    emit signal_Finish();
                    qDebug() << "Finish";

                    emit signal_stopCamberWorkProverka();

                    autoMode_NY_cold_hart = 1;kol_Auto_Start =0; //Устанавливаем НУ
                    if(proverka1 != nullptr){proverka1->relizProverka->flag_auto = false;qDebug() << "flag_auto =" <<proverka1->relizProverka->flag_auto;}
                    if(proverka2 != nullptr)proverka2->relizProverka->flag_auto = false;
                    if(proverka3 != nullptr)proverka3->relizProverka->flag_auto = false;
                    if(proverka4 != nullptr)proverka4->relizProverka->flag_auto = false;
                    if(proverka5 != nullptr)proverka5->relizProverka->flag_auto = false;
                    if(proverka6 != nullptr)proverka6->relizProverka->flag_auto = false;
                    if(proverka7 != nullptr)proverka7->relizProverka->flag_auto = false;
                    if(proverka8 != nullptr)proverka8->relizProverka->flag_auto = false;
                }
                }

                // slot_StartProverka();

            }

            // emit signal_StartProverka_5();
        }
        else
        {
            //  emit signal_BadQML(proverka1->relizProverka->countProverka);
        }
        break;

    default:
        break;
    }
}




//Слот для запуска имитатора генератора для 5 првоерки.
void Proverka::slot_start_GEN()
{
    kol_cignalov++;

    if(kol_cignalov == Count_NP) //m_ListProverkaItem->count()
    {
        qDebug() << "m_ListProverkaItem->count() = " << m_ListProverkaItem->count() << " ==  "<< kol_cignalov<<":kol_cignalov";
    }
    else
    {
        qDebug() << "kol_cignalov = " << kol_cignalov << endl;
        return;
    }

    proverkaItem* flag2 = nullptr;

    for(int i=0;i <Count_NP;i++)
    {
        proverkaItem* flag = (proverkaItem*)m_ListProverkaItem->value(i);


        if( flag->relizProverka->job == false)
        {
            start--;
        }
        else
        {
            flag2 = (proverkaItem*)m_ListProverkaItem->value(i);
        }

        start++;
    }



    qDebug () << "Start Proverka";

    if(flag2->relizProverka->liter <= 36)
    {
        flag2->relizProverka->liter = 26;
    }
    else
    {
        flag2->relizProverka->liter = 150;//26; //130
    }


    qDebug () << "flag2->relizProverka->liter = " << flag2->relizProverka->liter;


    flag2->relizProverka->liter = flag2->relizProverka->liter -1;

    qDebug () << "Start liters =" << flag2->relizProverka->liter;

    Set_Number_Spytnik(flag2);


    emit signal_Gen_setCONTrol("START");

    // emit signal_StartProverka_2();

    kol_cignalov = 0;

    emit signal_StartProverka_5();

}

//Слот для запуска имитатора для 6 првоерки с осцилографом .
void Proverka::slot_start_Imitator()
{
    kol_cignalov_Start_Imitator++;

    if(kol_cignalov_Start_Imitator == Count_NP) //m_ListProverkaItem->count()
    {
        qDebug() << "m_ListProverkaItem->count() = " << m_ListProverkaItem->count() << " ==  "<< kol_cignalov_Start_Imitator<<":kol_cignalov_Start_Imitator";
    }
    else
    {
        qDebug() << "kol_cignalov_Start_Imitator = " << kol_cignalov_Start_Imitator << endl;
        return;
    }


    qDebug () << "Start Proverka Осцилограф";


    emit setSIGNALtype("START");

    kol_cignalov_Start_Imitator = 0;

    // emit signal_StartProverka_5();
}

void Proverka::slot_next()
{
    emit signal_StartProverka_5(); // продолжить выполнение проверки после переключения
}



