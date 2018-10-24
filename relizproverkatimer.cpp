#include "relizproverkatimer.h"

/*!
    \brief Конструктор класса таймеров слежения спутников в проверках.
    \param[in] int indexGet Индекс прибора


    Конструктор класса таймеров слежения спутников в проверках.\n

    \author Sergey Smoglyuk
    \version 1.0
    \date Август 2018 года
*/
relizproverkaTimer::relizproverkaTimer(int indexGet,N6700Model* n6700Get,
                                       gsgModel* gsgModelGet,PortModel* portGet,RelizProverka* rez,QObject *parent) :
    QObject(parent),
    index(indexGet),
    gsg(gsgModelGet),
    n6700(n6700Get),
    port(portGet),
    rez1(rez),
    flag_seitings(true)

{

    this->moveToThread(new QThread()); //Переместили класс GSG в новый поток
    connect(this, &relizproverkaTimer::signal_finished, this->thread(), &QThread::quit);
    connect(this->thread(),&QThread::started,this,&relizproverkaTimer::process_start);
    this->thread()->start();

}

relizproverkaTimer::~relizproverkaTimer()
{
    qDebug() << "DELETE relizproverkaTimer";

    emit signal_finished();
}

void relizproverkaTimer::process_start()
{

    this->timer = new QTimer();
    //    this->timer->moveToThread(new QThread());
    //    connect(this->timer->thread(),&QThread::started,this,&relizproverkaTimer::process_start_timer);
    //    this->timer->thread()->start();

    process_start_timer();



    this->timer_liters = new QTimer();
    //    this->timer_liters->moveToThread(new QThread());
    //    connect(this->timer_liters->thread(),&QThread::started,this,&relizproverkaTimer::process_start_timer_liters);
    //    this->timer_liters->thread()->start();

    process_start_timer_liters();


    this->timer_seitings = new QTimer();
    //    this->timer_seitings->moveToThread(new QThread());
    //    connect(this->timer_seitings->thread(),&QThread::started,this,&relizproverkaTimer::process_start_timer_seitings);
    //    this->timer_seitings->thread()->start();

    process_start_timer_seitings();

    connect(this,&relizproverkaTimer::signal_finished,this,&relizproverkaTimer::stopTimer1);
    connect(this,&relizproverkaTimer::signal_finished,this,&relizproverkaTimer::stopTimer_liters);
    connect(this,&relizproverkaTimer::signal_finished,this,&relizproverkaTimer::stopTimer_seitings);
}

void relizproverkaTimer::process_start_timer()
{
    connect(timer,&QTimer::timeout,this,&relizproverkaTimer::time);
    connect(this,SIGNAL(startTimer1(int)),timer,SLOT(start(int)));
    connect(this,&relizproverkaTimer::stopTimer1,timer,&QTimer::stop);

}

void relizproverkaTimer::process_start_timer_liters()
{
    connect(timer_liters,&QTimer::timeout,this,&relizproverkaTimer::time_liters);
    connect(this,SIGNAL(startTimer_liters(int)),timer_liters,SLOT(start(int)));
    connect(this,&relizproverkaTimer::stopTimer_liters,timer_liters,&QTimer::stop);

}

void relizproverkaTimer::process_start_timer_seitings()
{
    connect(timer_seitings,&QTimer::timeout,this,&relizproverkaTimer::time_seitings);
    connect(this,SIGNAL(startTimer_seitings(int)),timer_seitings,SLOT(start(int)));
    connect(this,&relizproverkaTimer::stopTimer_seitings,timer_seitings,&QTimer::stop);
}



void relizproverkaTimer::time()
{

    port->PortNew->flag_end_MRK = false;

    if(port->PortNew->flag_start_MRK == true)
    {

        sec++;



        if(sec > 59 )
        {
            sec = 0;
            minut++;
        }


        //Код для обратного таймера
        //                sec--;

        //                if(sec < 0 )
        //                {
        //                    sec = 60;
        //                    minut--;
        //                }

        //emit signal_TimerQML(minut,sec);
        // qDebug ("Find GPS/GLONASS:   Time elapsed: %d:%d", minut,sec);

        //         if(minut == 0 && sec == 10)
        //         {
        //                port->PortNew->CountFindALL = 5;
        //         }


        QString strTimer;

        if(sec > 9 )
        {
            strTimer += QString::number(minut) + ":" + QString::number(sec);
        }
        else
        {
            strTimer += QString::number(minut) + ":0" + QString::number(sec);
        }

        qDebug() << port->CountFindALL << " ) relizproverkaTimer::time() = " << strTimer;

        emit signal_Tick(strTimer);

        strTimer.clear();




        //        if((port->PortNew->CountFindALL >= 4 && rez1->countProverka != 6) || (minut == 3 && sec == 30 && rez1->countProverka == 6))
        if(port->CountFindALL >= 4)
        {
            emit stopTimer1();
            timer->stop();

            port->PortNew->flag_start_MRK = false;
            port->PortNew->flag_end_MRK = true;
            port->CountFindALL=0;

            port->PortNew->flag = false;
            port->PortNew->CountFindALL=0;

            //rez1->sem->release();

            qDebug() << "FIND 4 spytnik : index = " << rez1->index;


            minut=0;
            sec = 0;


            if(rez1->countProverka == 4)
            {

                if(power.toDouble() < 16)
                {
                    rez1->Good = true;
                }
                else
                {
                    rez1->Good = false;
                }

                rez1->sem->release();

            }
            else
            {

                rez1->Good = true;
                rez1->sem->release();

            }

        }
        else
        {
            if(minut == 3 && sec == 30)
            {
                emit stopTimer1();
                timer->stop();

                qDebug() << "BEAD proverka 3:30 min : index = " << rez1->index;

                rez1->Good = false;
                port->PortNew->flag_start_MRK = false;
                port->PortNew->flag_end_MRK = true;
                port->PortNew->CountFindALL=0;

                port->PortNew->flag = false;

                minut=0;
                sec = 0;

                //Для проверки мощности
                if(rez1->countProverka == 4)
                {

                    if(power.toDouble() < 16)
                    {
                        rez1->Good = true;
                    }
                    else
                    {
                        rez1->Good = false;
                    }

                    rez1->sem->release();

                }
                else
                {

                    rez1->sem->release();

                }

            }
        }

    }
    else
    {

        emit signal_Tick("Ждем запуска приемника");
    }
}




void relizproverkaTimer::time_liters()
{
    port->PortNew->flag_end_MRK = false;

    if(port->PortNew->flag_start_MRK == true)
    {

        if(sec < 10 && minut == 0 && flag_seitings == true)
        {
            emit signal_GetMrk_OT();
            qDebug() << rez1->index << "Count send signal_GetMrk_OT : " << sec;
        }


        if(sec > 10 && sec <= 20 && minut == 0 && flag_seitings == true)
        {
            emit signal_GetMrk_liters(rez1->liter);

            qDebug() << rez1->index << "Count send signal_GetMrk_liters "<<rez1->liter<<" : " << sec-3;

        }

        if(sec > 20 && minut == 0 && flag_seitings == true)
        {
            flag_seitings = false;
            minut=0;
            sec = 0;
        }

        sec++;

        if(sec > 59 )
        {
            sec = 0;
            minut++;
        }


        //Код для обратного таймера
        //                sec--;

        //                if(sec < 0 )
        //                {
        //                    sec = 60;
        //                    minut--;
        //                }


        // qDebug ("Find GPS/GLONASS:   Time elapsed: %d:%d", minut,sec);


        QString strTimer;

        if(sec > 9 )
        {
            strTimer += QString::number(minut) + ":" + QString::number(sec);
        }
        else
        {
            strTimer += QString::number(minut) + ":0" + QString::number(sec);
        }

        qDebug() << " relizproverkaTimer::time_liters() = " << strTimer;

        if(flag_seitings == true )
        {
            emit signal_Tick("Установка литеры");
            strTimer.clear();
            return;
        }
        else
        {
            emit signal_Tick(findLiter(rez1->liter) + strTimer);
        }

        strTimer.clear();

        if(port->CountFindALL >= 24) //24
        {
            emit stopTimer_liters();
            timer_liters->stop();
            qDebug() << "FIND 24 : index " << rez1->index;

            port->PortNew->flag_start_MRK = false;
            port->PortNew->flag_end_MRK = true;
            port->CountFindALL=0;

            port->PortNew->flag = false;


            minut=0;
            sec = 0;

            rez1->Good = true;

            rez1->sem->release();

            flag_seitings = true;
        }
        else
        {
            //if(minut == 3 && sec == 30)
            if(minut == 0 && sec == 2)
            {
                emit stopTimer_liters();
                timer_liters->stop();
                qDebug() << "END timer : index " << rez1->index;

                rez1->Good = false;
                port->PortNew->flag_start_MRK = false;
                port->PortNew->flag_end_MRK = true;
                port->CountFindALL=0;

                port->PortNew->flag = false;

                flag_seitings = true;


                minut=0;
                sec = 0;
                rez1->sem->release();
            }
        }

    }
    else
    {
        emit signal_Tick("Ждем запуска приемника");
    }
}

void relizproverkaTimer::time_seitings()
{

    port->PortNew->flag_end_MRK = false;

    if(port->PortNew->flag_start_MRK == true)
    {

        sec++;

        if(sec > 59 )
        {
            sec = 0;
            minut++;
        }


        //Код для обратного таймера
        //                sec--;

        //                if(sec < 0 )
        //                {
        //                    sec = 60;
        //                    minut--;
        //                }

        //emit signal_TimerQML(minut,sec);
        // qDebug ("Find GPS/GLONASS:   Time elapsed: %d:%d", minut,sec);

        //         if(minut == 0 && sec == 10)
        //         {
        //                port->PortNew->CountFindALL = 5;
        //         }


        QString strTimer;

        if(sec > 9 )
        {
            strTimer += QString::number(minut) + ":" + QString::number(sec);
        }
        else
        {
            strTimer += QString::number(minut) + ":0" + QString::number(sec);
        }

        qDebug() << " relizproverkaTimer::time_seitings() = " << strTimer;

        emit signal_Tick(strTimer);

        strTimer.clear();


        if( port->CountFindALL >= 4 && rez1->countProverka != 10)
        {
            emit stopTimer_seitings();
            timer_seitings->stop();

            port->PortNew->flag_start_MRK = false;
            port->PortNew->flag_end_MRK = true;
            port->CountFindALL=0;

            port->PortNew->flag = false;

            minut=0;
            sec = 0;
            rez1->sem->release();

        }
        else
        {
            if(minut == 0 && sec == 1 && rez1->countProverka != 10) // 3 30
            {
                emit stopTimer_seitings();
                timer_seitings->stop();

                rez1->Good = false;
                port->PortNew->flag_start_MRK = false;
                port->PortNew->flag_end_MRK = true;
                port->CountFindALL=0;

                port->PortNew->flag = false;

                minut=0;
                sec = 0;
                rez1->sem->release();
            }
            else
            {
                if(minut == 5 && sec == 00 && rez1->countProverka == 10)
                {
                    emit stopTimer_seitings();
                    timer_seitings->stop();

                    rez1->Good = false;
                    port->PortNew->flag_start_MRK = false;
                    port->PortNew->flag_end_MRK = true;
                    port->CountFindALL=0;

                    port->PortNew->flag = false;

                    minut=0;
                    sec = 0;
                    rez1->sem->release();
                }
            }

        }

    }
    else
    {
        emit signal_Tick("Ждем запуска приемника");
    }

}


void relizproverkaTimer::Work()
{
    /* qDebug () << "NP = " <<  this->index;

    if(timer->isActive())
    {
        qDebug () << "!! Timer is start !!";
    }
    else
    {
    */
    emit startTimer1(1000);
    //  }


}

void relizproverkaTimer::Work_liters()
{
    /*  if(timer_liters->isActive())
    {
        qDebug () << "!! timer_liters is start !!";
    }
    else
    {
    */
    emit startTimer_liters(1000);
    // }
}

void relizproverkaTimer::Work_Os()
{
    /*   if(timer_seitings->isActive())
    {
        qDebug () << "!! timer_seitings is start !!";
    }
    else
    {
    */
    emit startTimer_seitings(1000);
    // }
}

void relizproverkaTimer::slot_Power(QString powerGet)
{
    power = powerGet;

}


void relizproverkaTimer::slot_EndBlock()
{
    qDebug () << "Proverka Open block wait = " << rez1->Name;

    rez1->sem->release();

}

void relizproverkaTimer::slot_EndBlock2(bool ok)
{
    qDebug () << ok << " : Proverka Open block wait = " << rez1->Name ;
    rez1->flag_start_mrk = ok;
    rez1->sem->release();
}

void relizproverkaTimer::slot_EndBlock_5_proverka()
{
    qDebug () << "5 proverka Open block wait = " << rez1->Name;

    rez1->sem->release();
}

void relizproverkaTimer::slot_EndBlock_proverka()
{
    qDebug () << "slot_EndBlock_proverka = " << rez1->Name;
    if(rez1->sem->available() > 1)
    {
        rez1->sem->release();
    }
}

void relizproverkaTimer::slot_stopProverka_error_10Mhz()
{
    emit stopTimer1();
    emit stopTimer_liters();

    flag_seitings = false;

    rez1->Good = false;
    port->PortNew->flag_start_MRK = false;
    port->PortNew->flag_end_MRK = true;
    port->PortNew->CountFindALL=0;
    port->PortNew->flag = false;


    rez1->sem->release();
}

QString relizproverkaTimer::findLiter(int liters)
{
    switch (liters) {
    case 25:
        return "-7 (R10) ";
    case 26:
        return "-6 (R18) ";
    case 27:
        return "-5 (R14) ";
    case 28:
        return "-4 (R6) ";
    case 29:
        return "-3 (R22) ";
    case 30:
        return "-2 (R13) ";
    case 31:
        return "-1 (R12) ";
    case 32:
        return "0 (R11) ";
    case 33:
        return "1 (R5) ";
    case 34:
        return "2 (R20) ";
    case 35:
        return "3 (R19) ";
    case 36:
        return "4 (R17) ";
    case 37:
        return "5 (R7) ";
    case 38:
        return "6 (R8) ";
    case 129:
        return "G1 ";
    case 130:
        return "G2 ";
    case 131:
        return "G3 ";
    case 132:
       return "G4 ";
    case 133:
       return "G5 ";
    case 134:
       return "G6 ";
    case 135:
        return "G7 ";
    case 136:
        return "G8 ";
    case 137:
       return "G9 ";
    case 138:
        return "G10 ";
    case 139:
       return "G11 ";
    case 140:
       return "G12 ";
    case 141:
       return "G13 ";
    case 142:
        return "G14 ";
    case 143:
       return "G15 ";
    case 144:
        return "G16 ";
    case 145:
        return "G17 ";
    case 146:
       return "G18 ";
    case 147:
       return "G19 ";
    case 148:
        return "G20 ";
    case 149:
        return "G21 ";
    case 150:
       return "G22 ";
    case 151:
       return "G23 ";
    case 152:
       return "G24 ";
    default:
        break;
    }

    return "";
}


