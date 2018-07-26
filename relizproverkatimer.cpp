#include "relizproverkatimer.h"

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

    this->timer = new QTimer();
    this->timer->moveToThread(new QThread());
    qDebug () << "Помещаем класс  << timer >> в поток: " << this->timer->thread();

    QObject::connect(this->timer->thread(),&QThread::started,this,&relizproverkaTimer::process_start_timer);

    this->timer->thread()->start();
    qDebug () << "Запускаем поток << timer >> : " << this->timer->thread();







    this->timer_liters = new QTimer();
    this->timer_liters->moveToThread(new QThread());
    qDebug () << "Помещаем класс  << timer_liters >> в поток: " << this->timer_liters->thread();

    QObject::connect(this->timer_liters->thread(),&QThread::started,this,&relizproverkaTimer::process_start_timer_liters);

    this->timer_liters->thread()->start();
    qDebug () << "Запускаем поток << timer_liters >> : " << this->timer_liters->thread();


    this->timer_seitings = new QTimer();
    this->timer_seitings->moveToThread(new QThread());
    qDebug () << "Помещаем класс  << timer_seitings >> в поток: " << this->timer_seitings->thread();

    QObject::connect(this->timer_seitings->thread(),&QThread::started,this,&relizproverkaTimer::process_start_timer_seitings);

    this->timer_seitings->thread()->start();
    qDebug () << "Запускаем поток << timer_seitings >> : " << this->timer_seitings->thread();






}

void relizproverkaTimer::process_start_timer()
{
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(time()));
    QObject::connect(this,SIGNAL(startTimer1(int)),timer,SLOT(start(int)));
    QObject::connect(this,&relizproverkaTimer::stopTimer1,timer,&QTimer::stop);

}

void relizproverkaTimer::process_start_timer_liters()
{
    QObject::connect(timer_liters,SIGNAL(timeout()),this,SLOT(time_liters()));
    QObject::connect(this,SIGNAL(startTimer_liters(int)),timer_liters,SLOT(start(int)));
    QObject::connect(this,&relizproverkaTimer::stopTimer_liters,timer_liters,&QTimer::stop);

}

void relizproverkaTimer::process_start_timer_seitings()
{
    QObject::connect(timer_seitings,SIGNAL(timeout()),this,SLOT(time_seitings()));
    QObject::connect(this,SIGNAL(startTimer_seitings(int)),timer_seitings,SLOT(start(int)));
    QObject::connect(this,&relizproverkaTimer::stopTimer_seitings,timer_seitings,&QTimer::stop);
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

        emit signal_Tick(strTimer);

        strTimer.clear();




        if((port->PortNew->CountFindALL >= 4 && rez1->countProverka != 6) || (minut == 3 && sec == 30 && rez1->countProverka == 6)) //изменил
        {
            port->PortNew->flag_start_MRK = false;
            port->PortNew->flag_end_MRK = true;
            port->PortNew->CountFindALL=0;

            port->PortNew->flag = false;

            emit stopTimer1();

            // qDebug ("STOP");
            minut=0;
            sec = 0;


            if(rez1->countProverka == 4)
            {

                // qDebug () <<"Power =" << power;
                // qDebug () <<"Power.toDouble() =" << power.toDouble();

                if(power.toDouble() < 16)
                {
                    rez1->Good = true;
                    //emit signal_GoodQML(true);
                }
                else
                {
                    rez1->Good = false;
                    //emit signal_GoodQML(false);
                }

                rez1->sem->release();

                // qDebug () << rez1->sem->available();
            }
            else
            {

                rez1->Good = true;

               // emit signal_GoodQML(true);

                rez1->sem->release();

                // qDebug () << rez1->sem->available();
            }


        }
        else
        {
            if(minut == 3 && sec == 30)
            {
                rez1->Good = false;
                port->PortNew->flag_start_MRK = false;
                port->PortNew->flag_end_MRK = true;
                port->PortNew->CountFindALL=0;

                port->PortNew->flag = false;




                emit stopTimer1();


                // qDebug ("STOP END TIME");

                minut=0;
                sec = 0;




                minut=0;
                sec = 0;





              //  emit signal_GoodQML(false);


                rez1->sem->release();

                // qDebug () << rez1->sem->available();


            }
        }

    }
    else
    {
        // qDebug ("Ждем запуска приемника");

        emit signal_Tick("Ждем запуска приемника");
    }
}




void relizproverkaTimer::time_liters()
{
    port->PortNew->flag_end_MRK = false;

    if(port->PortNew->flag_start_MRK == true)
    {

        if(sec < 9 && minut == 0 && flag_seitings == true)
        {
            emit signal_GetMrk_OT();
        }

        if(sec < 15 && sec > 10 && minut == 0 && flag_seitings == true)
        {
           emit signal_GetMrk_liters(rez1->liter);
           flag_seitings = false;
           minut=0;
           sec = 0;
        }

//        if(sec < 20 && sec > 15 && minut == 0 && flag_seitings == true)
//        {

//            emit signal_GetMrk_liters_2(rez1->liter);
//            flag_seitings = false;
//            minut=0;
//            sec = 0;
//        }


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

        if(flag_seitings == true )
        {
            emit signal_Tick("Установка литеры");
        }
        else
        {
            emit signal_Tick(strTimer);
        }

        strTimer.clear();



        if(port->PortNew->CountFindALL >= 24) //24
        {
            port->PortNew->flag_start_MRK = false;
            port->PortNew->flag_end_MRK = true;
            port->PortNew->CountFindALL=0;

            port->PortNew->flag = false;

            emit stopTimer_liters();

            // qDebug ("STOP");
            minut=0;
            sec = 0;


            rez1->Good = true;

           // emit signal_GoodQML(true);

            rez1->sem->release();

            // qDebug () << rez1->sem->available();

            flag_seitings = true;



        }
        else
        {
            if(minut == 3 && sec == 30)
            {
                rez1->Good = false;
                port->PortNew->flag_start_MRK = false;
                port->PortNew->flag_end_MRK = true;
                port->PortNew->CountFindALL=0;

                port->PortNew->flag = false;


                flag_seitings = true;


                emit stopTimer_liters();


                // qDebug ("STOP END TIME");

                minut=0;
                sec = 0;


               // emit signal_GoodQML(false);


                rez1->sem->release();

                // qDebug () << rez1->sem->available();


            }
        }

    }
    else
    {
     //   qDebug ("Ждем запуска приемника");

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

        emit signal_Tick(strTimer);

        strTimer.clear();


      //  double Amlityda = gsg->os->os->getAmplitude().toDouble();

        if(minut == 3 && sec == 30 )
        {
            port->PortNew->flag_start_MRK = false;
            port->PortNew->flag_end_MRK = true;
            port->PortNew->CountFindALL=0;

            port->PortNew->flag = false;



            emit stopTimer_seitings();

            // qDebug ("STOP");
            minut=0;
            sec = 0;


            rez1->sem->release();


        }
        else
        {
            if(minut == 10 && sec == 00)
            {
                rez1->Good = false;
                port->PortNew->flag_start_MRK = false;
                port->PortNew->flag_end_MRK = true;
                port->PortNew->CountFindALL=0;

                port->PortNew->flag = false;




                emit stopTimer_seitings();


                // qDebug ("STOP END TIME");

                minut=0;
                sec = 0;




                minut=0;
                sec = 0;





              //  emit signal_GoodQML(false);


                rez1->sem->release();

                // qDebug () << rez1->sem->available();


            }
        }

    }
    else
    {
        // qDebug ("Ждем запуска приемника");

        emit signal_Tick("Ждем запуска приемника");
    }

}


void relizproverkaTimer::Work()
{
    // qDebug () << "relizproverkaTimer";

    if(timer->isActive())
    {
        qDebug () << "Уже запущен timer";
    }
    else
    {
        emit startTimer1(1000);
    }


}

void relizproverkaTimer::Work_liters()
{
    emit startTimer_liters(1000);
}

void relizproverkaTimer::Work_Os()
{
    emit startTimer_seitings(1000);
}

void relizproverkaTimer::slot_Power(QString powerGet)
{
    power = powerGet;

}


void relizproverkaTimer::slot_EndBlock()
{
     qDebug () << "slot_EndBlock rez1->sem->release() = " << rez1->sem->available();

    if(rez1->Good)
    {
        rez1->sem->release();
    }

    qDebug () << "slot_EndBlock rez1->sem->release() = " << rez1->sem->available();

}

void relizproverkaTimer::slot_EndBlock_5_proverka()
{

   qDebug () << "slot_EndBlock_5_proverka rez1->sem->release() = " << rez1->sem->available() << " and " << rez1->Name;

   rez1->sem->release();

    qDebug () << "slot_EndBlock_5_proverka rez1->sem->release() = " << rez1->sem->available();

}


