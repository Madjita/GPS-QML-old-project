#include "relizproverka.h"
#include <QDebug>

#include <math.h>

RelizProverka::RelizProverka(int indexGet,int _indexList,N6700Model* n6700Get,
                             gsgModel* gsgModelGet, OsuilografModel* _os, tp8Model* _tp, PortModel* portGet,QString GetName,QObject *parent):
    QObject(parent),
    Name(GetName),
    index(indexGet),
    gsg(gsgModelGet),
    n6700(n6700Get),
    os(_os),
    tp(_tp),
    port(portGet),
    minut(0),
    count(0),
    countProverka(0),
    Good(false),
    liter(26),
    indexList(_indexList)
{

    this->moveToThread(new QThread()); //Переместили класс GSG в новый поток
    connect(this, &RelizProverka::signal_finished, this->thread(), &QThread::quit);
    connect(this->thread(),&QThread::started,this,&RelizProverka::process_start);
    this->thread()->start();



    flag_auto = false; //автоматическая проверка включена
    flag_start_mrk = false;

}

RelizProverka::~RelizProverka()
{
    qDebug() << "DELETE RelizProverka";
    emit signal_finished();
}

void RelizProverka::SetIdLink(QString GetIdLink)
{
    IdLink = GetIdLink;
}

//QString id;


void RelizProverka::process_start()
{


    connect(this,&RelizProverka::Work_signal,n6700->n6700,&N6700::Work); //

    connect(this,&RelizProverka::startTimer,n6700,&N6700Model::slot_StartTimer); //

    connect(this,&RelizProverka::setOutput,n6700,&N6700Model::setOut); //
    connect(this,&RelizProverka::setVolt,n6700,&N6700Model::setVolt); //

    //От QML на запуск других проверок

    //   connect(n6700,&N6700Model::signal_proverka,this,&RelizProverka::slot_proverka);


    connect(this,&RelizProverka::signal_StartProverka_1,this,&RelizProverka::proverka_rabotosposobnosti_NP_ID_1);
    connect(this,&RelizProverka::signal_StartProverka_2,this,&RelizProverka::proverka_rabotosposobnosti_Ponijennoe_naprRjenie_NP_ID_2);
    connect(this,&RelizProverka::signal_StartProverka_3,this,&RelizProverka::proverka_rabotosposobnosti_Ponijennoe_naprRjenie_NP_ID_3);
    connect(this,&RelizProverka::signal_StartProverka_4,this,&RelizProverka::proverka_rabotosposobnosti_Ponijennoe_naprRjenie_NP_ID_4);
    connect(this,&RelizProverka::signal_StartProverka_5,this,&RelizProverka::proverka_rabotosposobnosti_Ponijennoe_naprRjenie_NP_ID_5);
    connect(this,&RelizProverka::signal_StartProverka_6,this,&RelizProverka::proverka_rabotosposobnosti_Ponijennoe_naprRjenie_NP_ID_6);
    connect(this,&RelizProverka::signal_StartProverka_7,this,&RelizProverka::proverka_rabotosposobnosti_Ponijennoe_naprRjenie_NP_ID_7);
    connect(this,&RelizProverka::signal_StartProverka_8,this,&RelizProverka::proverka_rabotosposobnosti_Ponijennoe_naprRjenie_NP_ID_8);
    connect(this,&RelizProverka::signal_StartProverka_9,this,&RelizProverka::proverka_rabotosposobnosti_Ponijennoe_naprRjenie_NP_ID_9);
    connect(this,&RelizProverka::signal_StartProverka_10,this,&RelizProverka::proverka_rabotosposobnosti_Ponijennoe_naprRjenie_NP_ID_10);


    sem = new QSemaphore();
    mut = new QMutex();

}



//void RelizProverka::process_start_timer()
//{
//    connect(timer,SIGNAL(timeout()),this,SLOT(time()));
//    connect(this,SIGNAL(startTimer1(int)),timer,SLOT(start(int)));
//    connect(this,&RelizProverka::stopTimer1,timer,&QTimer::stop);
//}



//void RelizProverka::process_start_timer2()
//{
//    connect(timer2,SIGNAL(timeout()),this,SLOT(time2()));
//    connect(this,SIGNAL(startTimer2(int)),timer2,SLOT(start(int)));
//    connect(this,&RelizProverka::stopTimer2,timer,&QTimer::stop);
//}


void RelizProverka::waitMRKLoad()
{
    emit signal_qmlText("Ждем запуска приемника");

    this->port->PortNew->flag_waitloadingMRK = true;
    this->port->PortNew->flag_waitloadingMRK_proverka = true;
    this->port->PortNew->flag_end_MRK = false;
    this->port->slot_Work();

    qDebug() << "Wait start mrk : index = " << index;
    //Ждем загрузки приемников
    sem->acquire();
}


//**********************************************************************************
//1.	Проверка работоспособности приемника
void RelizProverka::proverka_rabotosposobnosti_NP_ID_1()
{

//    proverka_rabotosposobnosti_Ponijennoe_naprRjenie_NP_ID_2();
//    return;


    qDebug () << "Start proverka_rabotosposobnosti_NP_ID_1 proverka : " << index;


    // Подсчет проверок
    countProverka = 1;  //№1

    emit signal_StartProverkaIndex(QString::number(countProverka));

    //Cигнал для установки напряжения на источнике питания на том блоке к которому подсоединен приемник
    emit setVolt(QString::number(index),"5");
    //Подача питания на приемник
    emit setOutput(QString::number(index),true);

    //Ждем загрузки приемников
    waitMRKLoad();

    //Флаг работы, приемник включен
    job = true;

    if(flag_start_mrk)
    {
        //Сигнал начала запуска таймера для проверки (3.30 минут)
        emit startWork();
    }
    else
    {
        Good = flag_start_mrk;
    }

    qDebug () << "Start 1 proverka";
    //Время начала проверки для записи в БД
    stay = QDateTime::currentDateTime();
    qDebug () << "proverka Start  1 wait : index = " << index;
    qDebug () << "Data Start: " << start.toString("dd.MM.yyyy  hh:mm:ss");
    //Запись в БД начала проверки

    emit sendZaprozToBD("INSERT INTO Result (DateStayStart,IdLink,IdProverki) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','"+IdLink+"','"+QString::number(countProverka)+"')");

    if(flag_start_mrk)
    {
        //Семафор для остановки потока проверок для того чтоб подождать пока не найдутся спутники (не выполнится сама проверка)
        sem->acquire();
    }

    //По окончанию работы проверки переводим флаг работы в ложь, для показания того, что приемник прошел проверку и может быть выключен
    job = false;
    qDebug () << "Finish proverka : index = " << index;

    //Флаг показывающий прошел ли приемник данную проверку или нет
    flag_good_1 = Good;

    if(flag_good_1)
    {
        emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                              "Sootv = 'Соотв.'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
    }
    else
    {
        emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                              "Sootv = 'Не соотв.'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
    }



    //Сигнал выключения питания приемника по окончанию проверки
    emit setOutput(QString::number(index),false);
    //Сигнал завершения проверки
    emit signal_EndProverka(countProverka,indexList);

    qDebug () << "Wait all mrk : index = " << index;

    //Остановка потока для того чтоб дождаться остальные приемники для перехода на следующую првоерку
    sem->acquire();

    //Если проверка автоматическая то продолжать дальше
    if(flag_auto)
    {
        proverka_rabotosposobnosti_Ponijennoe_naprRjenie_NP_ID_2();
    }
}
//**********************************************************************************

//**********************************************************************************
//2.	Проверка на воздействие пониженного напряжения питания
void RelizProverka::proverka_rabotosposobnosti_Ponijennoe_naprRjenie_NP_ID_2()  //Дописать код для управления  камерой климотов и поставить таймер на ждание 3 часов и 2 часов
{
    // Подсчет проверок
    countProverka = 2;  //№2

    qDebug() << "Start proverka_rabotosposobnosti_Ponijennoe_naprRjenie_NP_ID_2 proverka : index = " << index;


    emit signal_StartProverkaIndex(QString::number(countProverka));

    //Cигнал для установки напряжения на источнике питания на том блоке к которому подсоединен приемник
    emit setVolt(QString::number(index),"4.74");
    //Подача питания на приемник
    emit setOutput(QString::number(index),true);

    //Ждем загрузки приемников
    waitMRKLoad();


    //Флаг работы, приемник включен
    job = true;
    //Очитска листа с результатоми этапов 2 проверки по разным напряжениями питания
    list_flag_good_2.clear();

    if(flag_start_mrk)
    {
        //Сигнал начала запуска таймера для проверки (3.30 минут)
        emit startWork();
    }
    else
    {
        Good = flag_start_mrk;
    }

    qDebug () << "Start 2 proverka";
    qDebug () << "Proverka begin is  2  Wait : index =" << index;
    //Время начала проверки для записи в БД
    stay = QDateTime::currentDateTime();
    //Запись в БД начала проверки
    emit sendZaprozToBD("INSERT INTO Result (DateStayStart,IdLink,IdProverki) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','"+IdLink+"','"+QString::number(countProverka)+"')");

    if(flag_start_mrk)
    {
        //Семафор для остановки потока проверок для того чтоб подождать пока не найдутся спутники (не выполнится сама проверка)
        sem->acquire();
    }

    //По окончанию работы проверки переводим флаг работы в ложь, для показания того, что приемник прошел проверку и может быть выключен
    job = false;
    qDebug () << "Proverka 2 Finish : index =" << index;
    //Флаг показывающий прошел ли приемник данную проверку или нет
    flag_good_2 = Good;
    //Добавление результата в лист
    list_flag_good_2.append(flag_good_2);

    if(flag_good_2)
    {
        switch (index) {
        case 1:
        {
            emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                                  "Sootv = '"+n6700->n6700->v_1+" В'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
            break;
        }
        case 2:
        {
            emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                                  "Sootv = '"+n6700->n6700->v_2+" В'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
            break;
        }
        case 3:
        {
            emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                                  "Sootv = '"+n6700->n6700->v_3+" В'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
            break;
        }
        case 4:
        {
            emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                                  "Sootv = '"+n6700->n6700->v_4+" В'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
            break;
        }
        case 5:
        {
            emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                                  "Sootv = '"+n6700->n6700->v_1+" В'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
            break;
        }
        case 6:
        {
            emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                                  "Sootv = '"+n6700->n6700->v_2+" В'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
            break;
        }
        case 7:
        {
            emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                                  "Sootv = '"+n6700->n6700->v_3+" В'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
            break;
        }
        case 8:
        {
            emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                                  "Sootv = '"+n6700->n6700->v_4+" В'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
            break;
        }

        }

    }
    else
    {
        // emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
        //                                                                                                                             "Sootv = 'Не соотв.'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");

        switch (index) {
        case 1:
        {
            emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                                  "Sootv = '"+n6700->n6700->v_1+" В'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
            break;
        }
        case 2:
        {
            emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                                  "Sootv = '"+n6700->n6700->v_2+" В'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
            break;
        }
        case 3:
        {
            emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                                  "Sootv = '"+n6700->n6700->v_3+" В'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
            break;
        }
        case 4:
        {
            emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                                  "Sootv = '"+n6700->n6700->v_4+" В'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
            break;
        }
        case 5:
        {
            emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                                  "Sootv = '"+n6700->n6700->v_1+" В'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
            break;
        }
        case 6:
        {
            emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                                  "Sootv = '"+n6700->n6700->v_2+" В'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
            break;
        }
        case 7:
        {
            emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                                  "Sootv = '"+n6700->n6700->v_3+" В'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
            break;
        }
        case 8:
        {
            emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                                  "Sootv = '"+n6700->n6700->v_4+" В'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
            break;
        }

        }
    }


    //Сигнал завершения проверки
    emit signal_EndProverka(countProverka,indexList);
    qDebug () << "Wait all mrk : index =" << index;

    //Сигнал выключения питания приемника по окончанию проверки
    emit setOutput(QString::number(index),false);



    //Остановка потока для того чтоб дождаться остальные приемники для перехода на следующую првоерку
    sem->acquire();


    if(flag_auto)
    {
        proverka_rabotosposobnosti_Ponijennoe_naprRjenie_NP_ID_3();
    }

}
//**********************************************************************************


//**********************************************************************************
//3.	Проверка работоспособности приемника на воздействие повышенного напряжения питания
void RelizProverka::proverka_rabotosposobnosti_Ponijennoe_naprRjenie_NP_ID_3()
{
    // Подсчет проверок
    countProverka = 3;  //№3

    qDebug() << "Start proverka_rabotosposobnosti_Ponijennoe_naprRjenie_NP_ID_3 proverka : index = " << index;


    emit signal_StartProverkaIndex(QString::number(countProverka));
    //Cигнал для установки напряжения на источнике питания на том блоке к которому подсоединен приемник
    emit setVolt(QString::number(index),"5.24");
    //Подача питания на приемник
    emit setOutput(QString::number(index),true);

    //Ждем загрузки приемников
    waitMRKLoad();


    //Флаг работы, приемник включен
    job = true;
    //Очитска листа с результатоми этапов 2 проверки по разным напряжениями питания
    list_flag_good_3.clear();

    if(flag_start_mrk)
    {
        //Сигнал начала запуска таймера для проверки (3.30 минут)
        emit startWork();
    }
    else
    {
        Good =  flag_start_mrk;
    }

    qDebug () << "Start 3 proverka";
    //Время начала проверки для записи в БД
    stay = QDateTime::currentDateTime();
    //Запись в БД начала проверки
    emit sendZaprozToBD("INSERT INTO Result (DateStayStart,IdLink,IdProverki) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','"+IdLink+"','"+QString::number(countProverka)+"')");
    qDebug () << "proverka begin  3 wait : index =" << index;

    if(flag_start_mrk)
    {
        //Семафор для остановки потока проверок для того чтоб подождать пока не найдутся спутники (не выполнится сама проверка)
        sem->acquire();
    }
    //По окончанию работы проверки переводим флаг работы в ложь, для показания того, что приемник прошел проверку и может быть выключен
    job = false;
    qDebug () << "Proverka 3 part 1 is Finish : index =" << index;
    //Флаг показывающий прошел ли приемник данную проверку или нет
    flag_good_3 = Good;
    //Добавление результата в лист
    list_flag_good_3.append(flag_good_3);

    if(flag_good_3)
    {
        //          emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
        //                                                                                                                                     "Sootv = 'Соотв.'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");

        switch (index) {
        case 1:
        {
            emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                                  "Sootv = '"+n6700->n6700->v_1+" В'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
            break;
        }
        case 2:
        {
            emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                                  "Sootv = '"+n6700->n6700->v_2+" В'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
            break;
        }
        case 3:
        {
            emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                                  "Sootv = '"+n6700->n6700->v_3+" В'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
            break;
        }
        case 4:
        {
            emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                                  "Sootv = '"+n6700->n6700->v_4+" В'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
            break;
        }
        case 5:
        {
            emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                                  "Sootv = '"+n6700->n6700->v_1+" В'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
            break;
        }
        case 6:
        {
            emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                                  "Sootv = '"+n6700->n6700->v_2+" В'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
            break;
        }
        case 7:
        {
            emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                                  "Sootv = '"+n6700->n6700->v_3+" В'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
            break;
        }
        case 8:
        {
            emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                                  "Sootv = '"+n6700->n6700->v_4+" В'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
            break;
        }

        }

    }
    else
    {
        // emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
        //                                                                                                                             "Sootv = 'Не соотв.'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");

        switch (index) {
        case 1:
        {
            emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                                  "Sootv = '"+n6700->n6700->v_1+" В'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
            break;
        }
        case 2:
        {
            emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                                  "Sootv = '"+n6700->n6700->v_2+" В'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
            break;
        }
        case 3:
        {
            emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                                  "Sootv = '"+n6700->n6700->v_3+" В'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
            break;
        }
        case 4:
        {
            emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                                  "Sootv = '"+n6700->n6700->v_4+" В'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
            break;
        }
        case 5:
        {
            emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                                  "Sootv = '"+n6700->n6700->v_1+" В'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
            break;
        }
        case 6:
        {
            emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                                  "Sootv = '"+n6700->n6700->v_2+" В'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
            break;
        }
        case 7:
        {
            emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                                  "Sootv = '"+n6700->n6700->v_3+" В'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
            break;
        }
        case 8:
        {
            emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                                  "Sootv = '"+n6700->n6700->v_4+" В'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
            break;
        }

        }

    }

    //Сигнал выключения питания приемника по окончанию проверки
    emit setOutput(QString::number(index),false);
    //Сигнал завершения проверки
    emit signal_EndProverka(countProverka,indexList);
    qDebug () << "Wait all mrk : index =" << index;


    //Остановка потока для того чтоб дождаться остальные приемники для перехода на следующую првоерку
    sem->acquire();

    if(flag_auto)
    {
        proverka_rabotosposobnosti_Ponijennoe_naprRjenie_NP_ID_4();

        //proverka_rabotosposobnosti_Ponijennoe_naprRjenie_NP_ID_10();
    }

}
//**********************************************************************************


//**********************************************************************************
//4.	Проверка мощности, потребляемой приемником от источника постоянного тока
void RelizProverka::proverka_rabotosposobnosti_Ponijennoe_naprRjenie_NP_ID_4()
{
    // Подсчет проверок
    countProverka = 4;  //№4

    qDebug() << "Start proverka_rabotosposobnosti_Ponijennoe_naprRjenie_NP_ID_4 proverka : index = " << index;

    emit signal_StartProverkaIndex(QString::number(countProverka));
    //Cигнал для установки напряжения на источнике питания на том блоке к которому подсоединен приемник
    emit setVolt(QString::number(index),"5");
    //Подача питания на приемник
    emit setOutput(QString::number(index),true);

    //Ждем загрузки приемников
    waitMRKLoad();


    //Флаг работы, приемник включен
    job = true;
    //Время начала проверки для записи в БД
    stay = QDateTime::currentDateTime();
    //Запись в БД начала проверки
    emit sendZaprozToBD("INSERT INTO Result (DateStayStart,IdLink,IdProverki) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','"+IdLink+"','"+QString::number(countProverka)+"')");



    if(flag_start_mrk)
    {
        //Сигнал начала запуска таймера для проверки (3.30 минут)
        emit startWork();
    }
    else
    {
        Good = flag_start_mrk;
    }
    qDebug () << "Запустили 4 проверку";
    qDebug () << "Проверка Начата  4 = " << sem->available();
    if(flag_start_mrk)
    {
        //Семафор для остановки потока проверок для того чтоб подождать пока не найдутся спутники (не выполнится сама проверка)
        sem->acquire();
    }

    //По окончанию работы проверки переводим флаг работы в ложь, для показания того, что приемник прошел проверку и может быть выключен
    job = false;
    qDebug () << "Проверка 4  Прошла = " << sem->available();
    //Флаг показывающий прошел ли приемник данную проверку или нет
    flag_good_4 = Good;

    double p = 0;

    if(flag_good_4)
    {
        //emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
        //                                                                                                                            "Sootv = 'Соотв.'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");

        switch (index) {
        case 1:
        {
            p = n6700->n6700->v_1.toDouble()*n6700->n6700->i_1.toDouble();

            emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                                  "Sootv = '"+QString::number(p,'\0',3)+" Вт'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
            break;
        }
        case 2:
        {
            p = n6700->n6700->v_2.toDouble()*n6700->n6700->i_2.toDouble();

            emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                                  "Sootv = '"+QString::number(p,'\0',3)+" Вт'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
            break;
        }
        case 3:
        {
            p = n6700->n6700->v_3.toDouble()*n6700->n6700->i_3.toDouble();

            emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                                  "Sootv = '"+QString::number(p,'\0',3)+" Вт'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
            break;
        }
        case 4:
        {
            p = n6700->n6700->v_4.toDouble()*n6700->n6700->i_4.toDouble();

            emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                                  "Sootv = '"+QString::number(p,'\0',3)+" Вт'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
            break;
        }
        case 5:
        {
            p = n6700->n6700->v_1.toDouble()*n6700->n6700->i_1.toDouble();

            emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                                  "Sootv = '"+QString::number(p,'\0',3)+" Вт'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
            break;
        }
        case 6:
        {
            p = n6700->n6700->v_2.toDouble()*n6700->n6700->i_2.toDouble();

            emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                                  "Sootv = '"+QString::number(p,'\0',3)+" Вт'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
            break;
        }
        case 7:
        {
            p = n6700->n6700->v_3.toDouble()*n6700->n6700->i_3.toDouble();

            emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                                  "Sootv = '"+QString::number(p,'\0',3)+" Вт'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
            break;
        }
        case 8:
        {
            p = n6700->n6700->v_4.toDouble()*n6700->n6700->i_4.toDouble();

            emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                                  "Sootv = '"+QString::number(p,'\0',3)+" Вт'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
            break;
        }

        }

    }
    else
    {
        // emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
        //                                                                                                                             "Sootv = 'Не соотв.'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");

        switch (index) {
        case 1:
        {
            p = n6700->n6700->v_1.toDouble()*n6700->n6700->i_1.toDouble();

            emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                                  "Sootv = '"+QString::number(p,'\0',3)+" Вт'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
            break;
        }
        case 2:
        {
            p = n6700->n6700->v_2.toDouble()*n6700->n6700->i_2.toDouble();

            emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                                  "Sootv = '"+QString::number(p,'\0',3)+" Вт'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
            break;
        }
        case 3:
        {
            p = n6700->n6700->v_3.toDouble()*n6700->n6700->i_3.toDouble();

            emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                                  "Sootv = '"+QString::number(p,'\0',3)+" Вт'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
            break;
        }
        case 4:
        {
            p = n6700->n6700->v_4.toDouble()*n6700->n6700->i_4.toDouble();

            emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                                  "Sootv = '"+QString::number(p,'\0',3)+" Вт'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
            break;
        }
        case 5:
        {
            p = n6700->n6700->v_1.toDouble()*n6700->n6700->i_1.toDouble();

            emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                                  "Sootv = '"+QString::number(p,'\0',3)+" Вт'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
            break;
        }
        case 6:
        {
            p = n6700->n6700->v_2.toDouble()*n6700->n6700->i_2.toDouble();

            emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                                  "Sootv = '"+QString::number(p,'\0',3)+" Вт'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
            break;
        }
        case 7:
        {
            p = n6700->n6700->v_3.toDouble()*n6700->n6700->i_3.toDouble();

            emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                                  "Sootv = '"+QString::number(p,'\0',3)+" Вт'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
            break;
        }
        case 8:
        {
            p = n6700->n6700->v_4.toDouble()*n6700->n6700->i_4.toDouble();

            emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                                  "Sootv = '"+QString::number(p,'\0',3)+" Вт'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
            break;
        }

        }
    }

    if(p <=15.0)
    {
        qDebug() <<"GOOD p = " << p;
        flag_good_4 = true;
    }
    else
    {
        qDebug() <<"BEAD p = " << p;

        flag_good_4 = false;
    }

    //Сигнал выключения питания приемника по окончанию проверки
    emit setOutput(QString::number(index),false);
    //Сигнал завершения проверки
    emit signal_EndProverka(countProverka,indexList);
    qDebug () << "Wait all mrk : index =" << index;


    //Остановка потока для того чтоб дождаться остальные приемники для перехода на следующую првоерку
    sem->acquire();

    if(flag_auto)
    {
        proverka_rabotosposobnosti_Ponijennoe_naprRjenie_NP_ID_5();

       // proverka_rabotosposobnosti_Ponijennoe_naprRjenie_NP_ID_6();
    }

}
//**********************************************************************************


//**********************************************************************************
//5.	Проверка поиска, захвата и сопровождения сигнала с уровнем мощности минус 160 дБВт(-130 dBm)
void RelizProverka::proverka_rabotosposobnosti_Ponijennoe_naprRjenie_NP_ID_5()
{
    // Подсчет проверок
    countProverka = 5;  //№5

    qDebug() << "Start proverka_rabotosposobnosti_Ponijennoe_naprRjenie_NP_ID_5 proverka : index = " << index;

    //  liter = 151; //  убрать поставить последний GPS (на время потом убрать)

    emit signal_StartProverkaIndex(QString::number(countProverka));

    //Cигнал на запуск имитации 1 спутника на имитаторе
    emit signal_startGen();

    qDebug () << "Start 5 proverka";
    //Время начала проверки для записи в БД
    stay = QDateTime::currentDateTime();
    //Запись в БД начала проверки
    emit sendZaprozToBD("INSERT INTO Result (DateStayStart,IdLink,IdProverki) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','"+IdLink+"','"+QString::number(countProverka)+"')");
    //Часть проверки (зануление, инициализация)
    proverka5_part = 0;
    //Отчистка листа результатов для данной проверки
    list_flag_good_5.clear();


    emit signal_qmlText("Загрузка имитатора");
    //Семафор для остановки потока для того чтоб подождать пока не запуститься имитатор
    sem->acquire();


    //  proverka5_part = 35; //убрать




    // for(int i = 129; i <= 151;i++) //убрал G24 так как на имитаторе не ставится. (151)  26

    //Цикол для проверки чувствительности по ГЛОНАСС установка литер от -7 до 0 и проверка их
    // for(int i = 26; i <= 151;i++) //убрал G24 так как на имитаторе не ставится. (151)  26
    //for(int i = 129; i <= 151;i++) //убрал G24 так как на имитаторе не ставится. (151)  26
    //  for(int i = 151; i <= 151;i++) //убрал G24 так как на имитаторе не ставится. (151)  26
    for(int i = 26; i <= 151;i++) //убрал G24 так как на имитаторе не ставится. (151)  26
    {

        qDebug() << "First:  I = " << i << " Litrea = " << liter;
        //Добавление части проверки
        proverka5_part++;

        //Установка литеры
        liter = i;

        qDebug() << "Second: I = " <<    i << " Litrea = " << liter;
        qDebug() << "proverka5_part: " << proverka5_part << " : " << index;

        if(i == 39)
        {
            //Перевод на GPS
            i = 129;
            liter = i;
        }
        else
        {
            //Глонасс
            liter = i;
        }

        //Cигнал для установки напряжения на источнике питания на том блоке к которому подсоединен приемник
        emit setVolt(QString::number(index),"5");
        //Подача питания на приемник
        emit setOutput(QString::number(index),true);

        //Ждем загрузки приемников
        waitMRKLoad();

        //Флаг работы, приемник включен
        job = true;

        if(flag_start_mrk)
        {
            //Сигнал начала запуска таймера для проверки (3.30 минут)
            emit startWork_liters();
        }
        else
        {
            Good = flag_start_mrk;
        }

        qDebug () << "Proverka Begin  5: liters № " << (i-32);
        if(flag_start_mrk)
        {
            //Семафор для остановки потока проверок для того чтоб подождать пока не найдутся спутники (не выполнится сама проверка)
            sem->acquire();
        }
        //По окончанию работы проверки переводим флаг работы в ложь, для показания того, что приемник прошел проверку и может быть выключен
        job = false;
        qDebug () << "Proverka 5: liters № "  << (i-32);
        //Флаг показывающий прошел ли приемник данную проверку или нет
        flag_good_5 = Good;
        //Добавление результата в лист
        list_flag_good_5.append(flag_good_5);

        qDebug () <<" Finish = " << this->Name << "Id: " << Id << " Date_Stay = " << stay_liter.toString("dd.MM.yyyy  hh:mm:ss");

        if(flag_good_5)
        {
            emit sendZaprozToBD("UPDATE Liter SET Sootv = 'Соотв.'"
                                " WHERE Date = '"+stay_liter.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
        }
        else
        {
            emit sendZaprozToBD("UPDATE Liter SET Sootv = 'Не соотв.'"
                                " WHERE Date = '"+stay_liter.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
        }

        //Сигнал выключения питания приемника по окончанию проверки
        emit setOutput(QString::number(index),false);
        //Сигнал завершения проверки
        emit signal_EndProverka(countProverka,indexList);
        qDebug () << "Wait all np : index " << index;

        //Остановка потока для того чтоб дождаться остальные приемники для перехода на следующую првоерку
        qDebug () << "Np = " << index;
        sem->acquire();

    }

    qDebug () << "END proverki on the liters = " << sem->available() << "data: " << stay_liter.toString("dd.MM.yyyy  hh:mm:ss");

    bool flagGood = false;

    for (auto n : list_flag_good_5)
    {
        if(n == false)
        {
            break;
        }
        else
        {
            flagGood = true;
        }
    }


    if(flagGood)
    {
        emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                              "Sootv = 'Соотв.'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
    }
    else
    {
        emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                              "Sootv = 'Не соотв.'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");

    }

    emit signal_IndexProverka(QString::number(countProverka));


    //    //Остановка потока для того чтоб дождаться остальные приемники для перехода на следующую првоерку
    //sem->acquire();


    if(flag_auto)
    {
        proverka_rabotosposobnosti_Ponijennoe_naprRjenie_NP_ID_6();
    }

}



void RelizProverka::proverka_rabotosposobnosti_Ponijennoe_naprRjenie_NP_ID_6()
{


    countProverka = 6; //№6

    qDebug() << "Start proverka_rabotosposobnosti_Ponijennoe_naprRjenie_NP_ID_6 proverka : index = " << index;

    QList<bool> listBool;


    //Флаг работы, приемник включен
    job = true;
    flag_300MGH_6Proverka = false;

    //Запустить переключение проверки (QML)
    emit signal_StartProverkaIndex(QString::number(countProverka));

    emit signal_beginOSProv(index);
    //ждем запуска последовательной проверки
    sem->acquire();

    stay = QDateTime::currentDateTime();
    qDebug () << "Proverka 6 start  WAIT";
    qDebug () << "Data start: " << start.toString("dd.MM.yyyy  hh:mm:ss");

    //Запись в БД начала проверки
    emit sendZaprozToBD("INSERT INTO Result (DateStayStart,IdLink,IdProverki) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','"+IdLink+"','"+QString::number(countProverka)+"')");


    os->os->flag_change_seitings = true;
    os->slot_Change_Seitings();

    tp->slot_comand7_Set_zatyxanie_10(static_cast<char>(0x9B),index);
    sem->acquire();


    tp->slot_comand6_Connect_Vx2_10_ext(index);
    sem->acquire();


    tp->slot_comand5_Connect_10MG(0x01,index); //(*QString::number(index).toUtf8().data()-0x30);
    sem->acquire();


    for(int i=0x9B; i > 0; i--)
    {

        //qDebug() << (UINT)i;

        tp->slot_comand7_Set_zatyxanie_10(static_cast<char>(i),index);
        sem->acquire();

        QString VMAX = os->os->getVRMS("CHANnel2");

        qDebug () <<"VMAX = " << VMAX;

        qDebug () <<"VMAX.toDouble() = " << VMAX.toDouble();

        if(VMAX.toDouble() >= 0.2 &&  VMAX.toDouble() <= 0.23 ) //мили вольты
        {
            flag_good_6 = true;
            listBool.append(flag_good_6);

            qDebug () <<"[VMAX.toDouble() >= 0.2 &&  VMAX.toDouble() <= 0.23  ] = " << flag_good_6;

            break;

        }
        else
        {
            flag_good_6 = false;

            if(( VMAX.toDouble() > 0.3 ) || (VMAX.toDouble() == 0.0) )
            {
                listBool.append(flag_good_6);
                break;
            }

        }
    }


    tp->slot_comand8_Connect_10_in(0x01,index);
    sem->acquire();

    tp->slot_comand3_Connect_Vx2_1C(index);
    sem->acquire();

    tp->slot_comand5_Connect_10MG(*QString::number(index).toUtf8().data()-0x30,index);
    sem->acquire();


    //Cигнал для установки напряжения на источнике питания на том блоке к которому подсоединен приемник
    emit setVolt(QString::number(index),"5");
    //Подача питания на приемник
    emit setOutput(QString::number(index),true);
    //Ждем загрузки приемников
    waitMRKLoad();

    if(flag_start_mrk)
    {
        //Сигнал начала запуска таймера для проверки (3.30 минут)
        emit startWork();
        qDebug () << "Start find spytniks 6 proverki : index =  " << index;
        //Семафор для остановки потока проверок для того чтоб подождать пока не найдутся спутники (не выполнится сама проверка)
        sem->acquire();
    }
    else
    {
        Good  = flag_start_mrk;
    }

    //По окончанию работы проверки переводим флаг работы в ложь, для показания того, что приемник прошел проверку и может быть выключен
    job = false;
    qDebug () << "Proverka find spytniks 6  END : index =  " << index;

    //Флаг показывающий прошел ли приемник данную проверку или нет
    flag_good_6 = Good;

    listBool.append(flag_good_6);


//    if(flag_good_6)
//    {
//        emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
//                                                                                                                              "Sootv = 'Соотв.'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
//    }
//    else
//    {
//        emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
//                                                                                                                              "Sootv = 'Не соотв.'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
//    }

    //Сигнал выключения питания приемника по окончанию проверки
    emit setOutput(QString::number(index),false);

    tp->slot_comand8_Connect_10_in(*QString::number(index).toUtf8().data()-0x30,index);
    sem->acquire();

    //Сигнал завершения проверки
    emit signal_EndProverka(countProverka,indexList);
    sem->acquire();

    qDebug () << "END part 1 proverka 6 : index =" << index;

    flag_300MGH_6Proverka = true;

    emit signal_StartProverkaIndex(QString::number(countProverka));

    ///////////////////////////////////////////////////////////////

    //Флаг работы, приемник включен
    job = true;

    os->os->flag_change_seitings = true;
    os->slot_Change_Seitings();

    tp->slot_comand7_Set_zatyxanie_10(static_cast<char>(0x9B),index);
    sem->acquire();


    tp->slot_comand6_Connect_Vx2_10_ext(index);
    sem->acquire();

    tp->slot_comand5_Connect_10MG(0x01,index); //(*QString::number(index).toUtf8().data()-0x30);
    sem->acquire();

    for(int i=0x9B; i > 0; i--)
    {

        //qDebug() << (UINT)i;

        tp->slot_comand7_Set_zatyxanie_10(static_cast<char>(i),index);
        sem->acquire();

        QString VMAX = os->os->getVRMS("CHANnel2");

        qDebug () <<"VMAX = " << VMAX;

        qDebug () <<"VMAX.toDouble() = " << VMAX.toDouble();

        if(VMAX.toDouble() >= 0.3 &&  VMAX.toDouble() <= 0.33 ) //мили вольты
        {
            flag_good_6 = true;
            listBool.append(flag_good_6);

            qDebug () <<"[VMAX.toDouble() >= 0.3 &&  VMAX.toDouble() <= 0.33  ] = " << flag_good_6;

            break;

        }
        else
        {
            flag_good_6 = false;

            if(( VMAX.toDouble() > 0.33 ) || (VMAX.toDouble() == 0.0) )
            {
                listBool.append(flag_good_6);
                break;
            }

        }
    }


    tp->slot_comand8_Connect_10_in(0x01,index);
    sem->acquire();


    tp->slot_comand3_Connect_Vx2_1C(index);
    sem->acquire();

    tp->slot_comand5_Connect_10MG(*QString::number(index).toUtf8().data()-0x30,index);
    sem->acquire();

    //Cигнал для установки напряжения на источнике питания на том блоке к которому подсоединен приемник
    emit setVolt(QString::number(index),"5");
    //Подача питания на приемник
    emit setOutput(QString::number(index),true);
    //Ждем загрузки приемников
    waitMRKLoad();


    // emit signal_startImitator();

    //Пересмотреть нужно убрать запись в бд так как их 2
   // stay = QDateTime::currentDateTime();
    qDebug () << "Проверка 6 часть 2 Начата  ждем";
   // qDebug () << "Дата старта: " << start.toString("dd.MM.yyyy  hh:mm:ss");

    //Запись в БД начала проверки
//    emit sendZaprozToBD("INSERT INTO Result (DateStayStart,IdLink,IdProverki) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','"+IdLink+"','"+QString::number(countProverka)+"')");
    //Семафор для остановки потока проверок для того чтоб подождать пока не найдутся спутники (не выполнится сама проверка)


    if(flag_start_mrk)
    {
        //Сигнал начала запуска таймера для проверки (3.30 минут)
        emit startWork();

        qDebug () << "Start find spytniks 6 proverki : index =  " << index;
        //Семафор для остановки потока проверок для того чтоб подождать пока не найдутся спутники (не выполнится сама проверка)
        sem->acquire();
    }
    else
    {
        Good  = flag_start_mrk;
    }


    //По окончанию работы проверки переводим флаг работы в ложь, для показания того, что приемник прошел проверку и может быть выключен
    job = false;
    qDebug () << "Proverka find spytniks 6  END : index =  " << index;

    //Флаг показывающий прошел ли приемник данную проверку или нет
    flag_good_6 = Good;

    listBool.append(flag_good_6);


    //Если на этапе включения и выставления 10 Мгц от внешнего генератора не прошло
    for(int i=0;i < listBool.count();i++ )
    {
        if(listBool[i] == false)
        {
            flag_good_6 = false; //првоерка не прошла
        }
    }



     //записать один раз в базу результат выполнения 6 проверки
    if(flag_good_6)
    {
        emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                              "Sootv = 'Соотв.'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");
    }
    else
    {
        emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                              "Sootv = 'Не соотв.'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");

    }

    //Сигнал выключения питания приемника по окончанию проверки
    emit setOutput(QString::number(index),false);

    tp->slot_comand8_Connect_10_in(*QString::number(index).toUtf8().data()-0x30,index);
    sem->acquire();

    //Сигнал завершения проверки
    emit signal_EndProverka(countProverka,indexList);

    qDebug () << "Wait all mrk : index = " << index;
    //Остановка потока для того чтоб дождаться остальные приемники для перехода на следующую првоерку

    emit signal_slot_StartProverka_Os(index,indexList);
    sem->acquire();



    if(flag_auto)
    {
        proverka_rabotosposobnosti_Ponijennoe_naprRjenie_NP_ID_7();
    }

}


//Проверка 10 МГЦ
void RelizProverka::proverka_rabotosposobnosti_Ponijennoe_naprRjenie_NP_ID_7()
{
    countProverka = 7;  //№7

    qDebug() << "Start proverka_rabotosposobnosti_Ponijennoe_naprRjenie_NP_ID_7 proverka : index = " << index;


    emit signal_StartProverkaIndex(QString::number(countProverka));

    //Cигнал для установки напряжения на источнике питания на том блоке к которому подсоединен приемник
    emit setVolt(QString::number(index),"5");
    //запускаем приемник
    emit setOutput(QString::number(index),true);
    //Ждем загрузки приемников
    waitMRKLoad();


    //утсновить настройки осцилографа
    os->os->flag_change_seitings = true;
    os->slot_Change_Seitings();

    job = true;

    emit startWork_Os();

    qDebug () << "Start 7 proverka : index = " << index;

    stay = QDateTime::currentDateTime();
    qDebug () << "Proverka 7 Begin  wait : index = " << index;
    qDebug () << "Data start: " << start.toString("dd.MM.yyyy  hh:mm:ss") <<" : index = " << index;
    //Запись в БД начала проверки
    emit sendZaprozToBD("INSERT INTO Result (DateStayStart,IdLink,IdProverki) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','"+IdLink+"','"+QString::number(countProverka)+"')");

    //Семафор для остановки потока проверок для того чтоб подождать пока не найдутся спутники (не выполнится сама проверка)
    qDebug () << "Proverka Begin  7 : index = " << index;
    sem->acquire();
    job = false;

    emit signal_beginOSProv_7(index);
    sem->acquire();


    qDebug () << "Number Np = " << *QString::number(index).toUtf8().data();

    tp->slot_comand1_Connect_Vx2_10(*QString::number(index).toUtf8().data()-0x30,index);
    sem->acquire();


    QString VMAX =  os->os->getVRMS("CHANnel2");

    qDebug () <<"VMAX = " << VMAX << " : index = " << index;

    qDebug () <<"VMAX.toDouble() = " << VMAX.toDouble() << " : index = " << index;

    if(VMAX.toDouble() >= 0.2  && VMAX.toDouble() <= 0.3  ) //мили вольты
    {
        flag_good_7 = true;

        qDebug () <<"[VMAX.toDouble() >= 0.2  && VMAX.toDouble() <= 0.3 ] = " << flag_good_7 << " : index = " << index;

    }
    else
    {
        flag_good_7 = false;

        qDebug () <<"[VMAX.toDouble() >= 0.2  && VMAX.toDouble() <= 0.3 ] = " << flag_good_7 << " : index = " << index;
    }


    if(flag_good_7)
    {
        // emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
        //                                                                                                                             "Sootv = 'Соотв.'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");


        emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                              "Sootv = '"+QString::number(VMAX.toDouble()*1000)+" мВ'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");

    }
    else
    {
        // emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
        //                                                                                                                             "Sootv = 'Не соотв.'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");

        emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                              "Sootv = '"+QString::number(VMAX.toDouble()*100)+" мВ'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");

    }


    qDebug () << "Proverka 7  END : index = " << index;

    emit setOutput(QString::number(index),false);

    emit signal_slot_StartProverka_Os(index,indexList);

    emit signal_EndProverka(countProverka,indexList);
    qDebug () << "Wait all np : index = " << index;
    sem->acquire();

    if(flag_auto)
    {
        proverka_rabotosposobnosti_Ponijennoe_naprRjenie_NP_ID_8();
    }

}

void RelizProverka::proverka_rabotosposobnosti_Ponijennoe_naprRjenie_NP_ID_8()
{
    countProverka = 8;  //№8

    qDebug() << "Start proverka_rabotosposobnosti_Ponijennoe_naprRjenie_NP_ID_8 proverka : index = " << index;

    os->os->flag_change_seitings = false;
    os->slot_Change_Seitings();

    emit signal_StartProverkaIndex(QString::number(countProverka));

    //Cигнал для установки напряжения на источнике питания на том блоке к которому подсоединен приемник
    emit setVolt(QString::number(index),"5");
    //запускаем приемник (падали ток)
    emit setOutput(QString::number(index),true);
    //Ждем загрузки приемников
    waitMRKLoad();


    job = true;


    qDebug () << "Start 8 proverka : index = " << index;

    stay = QDateTime::currentDateTime();
    qDebug () << "Proverka 8 Begin : index = " << index;
    qDebug () << "Data Start: " << start.toString("dd.MM.yyyy  hh:mm:ss")<< " : index = " << index;
    //Запись в БД начала проверки
    emit sendZaprozToBD("INSERT INTO Result (DateStayStart,IdLink,IdProverki) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','"+IdLink+"','"+QString::number(countProverka)+"')");



    if(flag_start_mrk)
    {
        emit startWork_Os();
        //Семафор для остановки потока проверок для того чтоб подождать пока не найдутся спутники (не выполнится сама проверка)
        sem->acquire();
    }
    else
    {
        Good  = flag_start_mrk;
    }




    job = false;

    emit signal_beginOSProv_7(index);
    sem->acquire();


    tp->slot_comand2_Connect_Vx1_1C(*QString::number(index).toUtf8().data()-0x30,index);
    sem->acquire();


    QString Amplituda = os->os->getAmplitude();
    qDebug () <<"Amplituda = " << Amplituda << " : index = " << index;
    qDebug () <<"Amplituda.toDouble() = " << Amplituda.toDouble() << " : index = " << index;

    if(Amplituda.toDouble() >= 2.4 )
    {
        flag_good_8 = true;
    }
    else
    {
        flag_good_8 = false;
    }




    if(flag_good_8)
    {
        // emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
        //                                                                                                                             "Sootv = 'Соотв.'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");

        emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                              "Sootv = '"+QString::number(Amplituda.toDouble())+" В'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");


    }
    else
    {
        // emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
        //                                                                                                                             "Sootv = 'Не соотв.'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");

        emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                              "Sootv = '"+QString::number(Amplituda.toDouble())+" В'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");


    }

    qDebug () << "Proverka 8  End  : index = " << index;

    emit setOutput(QString::number(index),false);

    emit signal_slot_StartProverka_Os(index,indexList);

    emit signal_EndProverka(countProverka,indexList);
    qDebug () << "Wait all np  : index = " << index;

    sem->acquire();



    if(flag_auto)
    {
        proverka_rabotosposobnosti_Ponijennoe_naprRjenie_NP_ID_9();
    }

}

//9.	Проверка напряжения питания МШУ (нет в ПМ)
void RelizProverka::proverka_rabotosposobnosti_Ponijennoe_naprRjenie_NP_ID_9()
{
    countProverka = 9;  //№9

    qDebug() << "Start proverka_rabotosposobnosti_Ponijennoe_naprRjenie_NP_ID_9 proverka : index = " << index;

    os->os->flag_change_seitings_3 = false;
    os->slot_Change_Seitings3();

    emit signal_StartProverkaIndex(QString::number(countProverka));

    //Cигнал для установки напряжения на источнике питания на том блоке к которому подсоединен приемник
    emit setVolt(QString::number(index),"5");
    //запускаем приемник (падали ток)
    emit setOutput(QString::number(index),true);
    //Ждем загрузки приемников
    waitMRKLoad();


    job = true;

    qDebug () << "Start 9 proverka  : index = " << index;

    stay = QDateTime::currentDateTime();
    qDebug () << "Proverka 9 Begin  : index = " << index;
    qDebug () << "Data start: " << start.toString("dd.MM.yyyy  hh:mm:ss") << " : index = " << index;
    //Запись в БД начала проверки
    emit sendZaprozToBD("INSERT INTO Result (DateStayStart,IdLink,IdProverki) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','"+IdLink+"','"+QString::number(countProverka)+"')");



    job = false;

    emit signal_beginOSProv_7(index);
    sem->acquire();

    tp->slot_comand4_Connect_Vx1_MShY(*QString::number(index).toUtf8().data()-0x30,index);
    sem->acquire();


    QString Amplituda = os->os->getVRMS("CHANnel1");
    qDebug () <<"Amplituda = " << Amplituda << " : index = " << index;
    qDebug () <<"Amplituda.toDouble() = " << Amplituda.toDouble() << " : index = " << index;

    if(Amplituda.toDouble() >= 4.75 && Amplituda.toDouble() <= 5.25)
    {
        flag_good_9 = true;
    }
    else
    {
        flag_good_9 = false;
    }


    if(flag_good_9)
    {
        //  emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
        //                                                                                                                              "Sootv = 'Соотв.'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");

        emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                              "Sootv = '"+QString::number(Amplituda.toDouble())+" В'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");

    }
    else
    {
        //  emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
        //                                                                                                                              "Sootv = 'Не соотв.'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");

        emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                              "Sootv = '"+QString::number(Amplituda.toDouble())+" В'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");

    }


    qDebug () << "Proverka 9  End  : index = " << index;

    emit setOutput(QString::number(index),false);

    emit signal_slot_StartProverka_Os(index,indexList);

    emit signal_EndProverka(countProverka,indexList);
    qDebug () << "Wait all np  : index = " << index;
    sem->acquire();


    if(flag_auto)
    {
        proverka_rabotosposobnosti_Ponijennoe_naprRjenie_NP_ID_10();
    }
}

void RelizProverka::proverka_rabotosposobnosti_Ponijennoe_naprRjenie_NP_ID_10()
{

    countProverka = 10;  //№10

    qDebug() << "Start proverka_rabotosposobnosti_Ponijennoe_naprRjenie_NP_ID_10 proverka : index = " << index;


    emit signal_StartProverkaIndex(QString::number(countProverka));

    os->os->flag_change_seitings_3 = true;
    os->slot_Change_Seitings3();

    job = true;

    //Cигнал для установки напряжения на источнике питания на том блоке к которому подсоединен приемник
    emit setVolt(QString::number(index),"5");
    //запускаем приемник (падали ток)
    emit setOutput(QString::number(index),true);
    //Ждем загрузки приемников
    waitMRKLoad();

    emit startWork_Os();

    stay = QDateTime::currentDateTime();

    qDebug () << "Proverka 10 Begin  wait : index = " << index;
    qDebug () << "Data start: " << start.toString("dd.MM.yyyy  hh:mm:ss") << " : index = " << index;
    //Запись в БД начала проверки
    emit sendZaprozToBD("INSERT INTO Result (DateStayStart,IdLink,IdProverki) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','"+IdLink+"','"+QString::number(countProverka)+"')");

    //Семафор для остановки потока проверок для того чтоб подождать пока не найдутся спутники (не выполнится сама проверка)
    sem->acquire();
    job = false;

    qDebug () << "Proverka 10 Start   : index = " << index;

    emit signal_beginOSProv_7(index);
    sem->acquire();


    tp->slot_comand2_Connect_Vx1_1C(*QString::number(index).toUtf8().data()-0x30,index);
    sem->acquire();


    tp->slot_comand3_Connect_Vx2_1C(index);
    sem->acquire();

    os->os->setTIMebaseSCALe("0.0000005");
    this->thread()->sleep(2);


    QString Delay;


    Delay = os->os->getDelay();




    qDebug () <<"Delay = " << Delay;
    qDebug () <<"Delay.toDouble() = " << QString::number(qAbs(Delay.toDouble()),'f',6) << " : index = " << index;


    if(qAbs(Delay.toDouble()) <= 0.000001 ) // 1*qPow(10,(-6))
    {
        flag_good_10 = true;
    }
    else
    {

        os->os->setTIMebaseSCALe("0.005");
        this->thread()->sleep(2);

        Delay = os->os->getDelay();
        qDebug () <<"Delay = " << qAbs(Delay.toDouble());
        qDebug () <<"Delay = " << qAbs(Delay.toDouble()*100);


        flag_good_10 = false;
    }




    if(flag_good_10)
    {
        // emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
        //                                                                                                                             "Sootv = 'Соотв.'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");

        emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                              "Sootv = '"+QString::number(qAbs(Delay.toDouble())*qPow(10,6),'f',3)+" мкс'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");


    }
    else
    {
        // emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
        //                                                                                                                             "Sootv = 'Не соотв.'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");

        emit sendZaprozToBD("UPDATE Result SET DateStayEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"', "
                                                                                                                              "Sootv = '"+QString::number(qAbs(Delay.toDouble())*qPow(10,6),'f',3)+" мкс'  WHERE DateStayStart = '"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"' AND IdLink = '"+IdLink+"'");


    }

    emit setOutput(QString::number(index),false);

    qDebug () << "Proverka 10  End : index = " << index << " Флаг : " << flag_good_10;
    qDebug () << "DateStart = " << start.toString("dd.MM.yyyy  hh:mm:ss") << " : index = " << index;

    qDebug () << "sem->available() = " <<sem->available() << " : index = " << index;

    emit sendZaprozToBD("UPDATE DateStartEnd SET DateEnd = '"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"'"
                                                                                                                            " WHERE DateStart = '"+start.toString("dd.MM.yyyy  hh:mm:ss")+"'");

    emit signal_slot_StartProverka_Os(index,indexList);

    emit signal_EndProverka(countProverka,indexList);
    qDebug () << "Wait all np  : index = " << index;

    qDebug () << "The END  : index = " << index;
}


void RelizProverka::Work(bool auto_test, int proverka)
{
    flag_auto = auto_test;

    qDebug () << "RelizProverka = " << index << " | auto = " << flag_auto;



    if(auto_test)
    {
        emit signal_StartProverka_1();
    }
    else
    {
        switch (proverka)
        {
        case 1:emit signal_StartProverka_1(); break;
        case 2:emit signal_StartProverka_2(); break;
        case 3:emit signal_StartProverka_3(); break;
        case 4:emit signal_StartProverka_4(); break;
        case 5:emit signal_StartProverka_5(); break;
        case 6:
        {
            emit signal_StartProverka_6(); break;
        }
        case 7:emit signal_StartProverka_7(); break;
        case 8:emit signal_StartProverka_8(); break;
        case 9:emit signal_StartProverka_9(); break;
        case 10:emit signal_StartProverka_10(); break;

        }
    }


}

