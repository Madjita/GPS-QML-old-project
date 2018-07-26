#include "n6700.h"

#include <windows.h>
#include <QDateTime>

#include "N6700.h"
#include <QDateTime>


N6700::N6700(QObject *parent) : QObject(parent),
    flag_work(false)

{

    this->moveToThread(new QThread()); //Переместили класс N6700 в новый поток
    qDebug () << "Помещаем класс  << N6700 >> в поток: " << this->thread();

    QObject::connect(this->thread(),&QThread::started,this,&N6700::process_start);
    this->thread()->start();
    qDebug () << "Запускаем поток << N6700 >> : " << this->thread();

    qRegisterMetaType<QVector<QString>>();

}


void N6700::process_start()
{
    connected = false;

    //Инициализация таймеров
    QTime midnight(0,0,0);
    qsrand(midnight.secsTo(QTime::currentTime()));

    timer_IstP1_Measure = new QTimer();

    QObject::connect(timer_IstP1_Measure,&QTimer::timeout,this,&N6700::Work);

    QObject::connect(this,SIGNAL(startTimer(int)),timer_IstP1_Measure,SLOT(start(int)));
    QObject::connect(this,&N6700::TimerStop,timer_IstP1_Measure,&QTimer::stop);
}



void N6700::slot_StartTimer()
{
//    if(timer_IstP1_Measure->isActive())
//    {
//        qDebug () << "Уже запущен таймер источника мпитания";
//    }
//    else
//    {
        emit startTimer(1000);
        flag_work = true;

        qDebug () << "Hellow";
  //  }

}



bool N6700::Connect(QString ip)
{

    viStatus=viOpenDefaultRM(&defaultRM);

    QString str  = "TCPIP0::"+ip+"::inst0::INSTR";

    viStatus=viOpen(defaultRM, (ViRsrc)qPrintable("TCPIP0::"+ip+"::inst0::INSTR"), VI_NULL, VI_NULL,&vi); // проверено - работает через IP

    if(viStatus<VI_SUCCESS)
    {
        qDebug() << "[Соединение c Источником питания №1 не установленно(ERROR)]";

        name = "Соединение c Источником питания не установленно.";

        connected = false;

        emit connectOk();

        return false;
    }
    else
    {
        connected = true;
        getName();

        qDebug() << "[Соединение c "<< name<<" установленно(ОК)]";



        setVolt("all","5"); // Установка напряжения на все каналы источника питания 5 V
        setCurrent("all","3");
        setOutput("all",false);

        emit connectOk();

        return true;
    }
}



void N6700::startProverka()
{
    if(connected)
    {
        timer_IstP1_Measure->blockSignals(false);

        timer_IstP1_Measure->start(1000);

        setVolt("all","1");
        setOutput("all",true);
    }
}
//Разъединить соединение
void N6700::DisConnect()
{

    emit TimerStop();

    //  timer_IstP1_Measure->blockSignals(true);

    setOutput("all",false);



    viClose( vi );

}



void N6700::getName()
{
    char nameChar[100];

    viQueryf(vi,"*IDN?\t\n","%T",nameChar);

    name = qPrintable(nameChar);
}

void N6700::setOutput(QString canal, bool OnOff)
{
    if(canal =="ALL" || canal =="all")
    {
        switch (OnOff) {
        case true:
            viPrintf(vi, "OUTPut:STATe 1,(@1)\r\n");
            viPrintf(vi, "OUTPut:STATe 1,(@2)\r\n");
            viPrintf(vi, "OUTPut:STATe 1,(@3)\r\n");
            viPrintf(vi, "OUTPut:STATe 1,(@4)\r\n");
            break;
        case false:
            viPrintf(vi, "OUTPut:STATe 0,(@1)\r\n");
            viPrintf(vi, "OUTPut:STATe 0,(@2)\r\n");
            viPrintf(vi, "OUTPut:STATe 0,(@3)\r\n");
            viPrintf(vi, "OUTPut:STATe 0,(@4)\r\n");
            break;
        }


        //        // Check errors
        //        char* buff = new char();
        //        viQueryf(vi,"SYSTem:ERRor?\n","%T",buff);

        //        if(QString(buff) != "0,\"No error\"\n")
        //        {
        //            qDebug () <<"Error setOutput(QString canal, bool OnOff) canal: "<<canal<<" and "<<QString::number(OnOff)<<":  " << buff;
        //        }

    }
    else
    {

        switch (canal.toInt())
        {
        case 1:
        {
            switch (OnOff) {
            case true:
                viPrintf(vi, "OUTPut:STATe 1,(@1)\r\n");
                break;
            case false:
                viPrintf(vi, "OUTPut:STATe 0,(@1)\r\n");
                break;
            }

            //            // Check errors
            //            char* buff = new char();
            //            viQueryf(vi,"SYSTem:ERRor?\n","%T",buff);

            //            if(QString(buff) != "0,\"No error\"\n")
            //            {
            //                qDebug () <<"Error setOutput(QString canal, bool OnOff) canal: "<<canal<<":  " << buff;
            //            }
            break;
        }
        case 2:
        {
            switch (OnOff) {
            case true:
                viPrintf(vi, "OUTPut:STATe 1,(@2)\r\n");
                break;
            case false:
                viPrintf(vi, "OUTPut:STATe 0,(@2)\r\n");
                break;
            }

            //            // Check errors
            //            char* buff = new char();
            //            viQueryf(vi,"SYSTem:ERRor?\n","%T",buff);

            //            if(QString(buff) != "0,\"No error\"\n")
            //            {
            //                qDebug () <<"Error setOutput(QString canal, bool OnOff) canal: "<<canal<<":  " << buff;
            //            }
            break;
        }
        case 3:
        {
            switch (OnOff) {
            case true:
                viPrintf(vi, "OUTPut:STATe 1,(@3)\r\n");
                break;
            case false:
                viPrintf(vi, "OUTPut:STATe 0,(@3)\r\n");
                break;
            }

            //            // Check errors
            //            char* buff = new char();
            //            viQueryf(vi,"SYSTem:ERRor?\n","%T",buff);

            //            if(QString(buff) != "0,\"No error\"\n")
            //            {
            //                qDebug () <<"Error setOutput(QString canal, bool OnOff) canal: "<<canal<<":  " << buff;
            //            }
            break;
        }
        case 4:
        {
            switch (OnOff) {
            case true:
                viPrintf(vi, "OUTPut:STATe 1,(@4)\r\n");
                break;
            case false:
                viPrintf(vi, "OUTPut:STATe 0,(@4)\r\n");
                break;
            }

            //            // Check errors
            //            char* buff = new char();
            //            viQueryf(vi,"SYSTem:ERRor?\n","%T",buff);

            //            if(QString(buff) != "0,\"No error\"\n")
            //            {
            //                qDebug () <<"Error setOutput(QString canal, bool OnOff) canal: "<<canal<<":  " << buff;
            //            }
            break;
        }
        }
    }
}


void N6700::setVolt(QString canal, QString V)
{
    if(canal =="ALL" || canal =="all")
    {
        viPrintf(vi, "VOLTage:LEVel %s,(@1)\r\n",qPrintable(V));
        viPrintf(vi, "VOLTage:LEVel %s,(@2)\r\n",qPrintable(V));
        viPrintf(vi, "VOLTage:LEVel %s,(@3)\r\n",qPrintable(V));
        viPrintf(vi, "VOLTage:LEVel %s,(@4)\r\n",qPrintable(V));

        //        // Check errors
        //        char* buff = new char();
        //        viQueryf(vi,"SYSTem:ERRor?\n","%T",buff);

        //        if(QString(buff) != "0,\"No error\"\n")
        //        {
        //            qDebug () <<"Error setOutput(QString canal, bool OnOff) canal: "<<canal<<" and V="<<qPrintable(V)<<":  " << buff;
        //        }
    }
    else
    {
        switch (canal.toInt())
        {
        case 1:
        {
            viPrintf(vi, "VOLTage:LEVel %s,(@1)\r\n",qPrintable(V));

            //            // Check errors
            //            char* buff = new char();
            //            viQueryf(vi,"SYSTem:ERRor?\n","%T",buff);

            //            if(QString(buff) != "0,\"No error\"\n")
            //            {
            //                qDebug () <<"Error setOutput(QString canal, bool OnOff) canal: "<<canal<<" and V="<<qPrintable(V)<<":  " << buff;
            //            }
        }
        case 2:
        {
            viPrintf(vi, "VOLTage:LEVel %s,(@2)\r\n",qPrintable(V));

            //            // Check errors
            //            char* buff = new char();
            //            viQueryf(vi,"SYSTem:ERRor?\n","%T",buff);

            //            if(QString(buff) != "0,\"No error\"\n")
            //            {
            //                qDebug () <<"Error setOutput(QString canal, bool OnOff) canal: "<<canal<<" and V="<<qPrintable(V)<<":  " << buff;
            //            }
        }
        case 3:
        {
            viPrintf(vi, "VOLTage:LEVel %s,(@3)\r\n",qPrintable(V));

            //            // Check errors
            //            char* buff = new char();
            //            viQueryf(vi,"SYSTem:ERRor?\n","%T",buff);

            //            if(QString(buff) != "0,\"No error\"\n")
            //            {
            //                qDebug () <<"Error setOutput(QString canal, bool OnOff) canal: "<<canal<<" and V="<<qPrintable(V)<<":  " << buff;
            //            }
        }
        case 4:
        {
            viPrintf(vi, "VOLTage:LEVel %s,(@4)\r\n",qPrintable(V));

            //            // Check errors
            //            char* buff = new char();
            //            viQueryf(vi,"SYSTem:ERRor?\n","%T",buff);

            //            if(QString(buff) != "0,\"No error\"\n")
            //            {
            //                qDebug () <<"Error setOutput(QString canal, bool OnOff) canal: "<<canal<<" and V="<<qPrintable(V)<<":  " << buff;
            //            }
        }
        }
    }
}

void N6700::setCurrent(QString canal, QString I)
{
    if(canal =="ALL" || canal =="all")
    {
        viPrintf(vi, "CURRent:LEVel %s,(@1)\r\n",qPrintable(I));
        viPrintf(vi, "CURRent:LEVel %s,(@2)\r\n",qPrintable(I));
        viPrintf(vi, "CURRent:LEVel %s,(@3)\r\n",qPrintable(I));
        viPrintf(vi, "CURRent:LEVel %s,(@4)\r\n",qPrintable(I));

        //        // Check errors
        //        char* buff = new char();
        //        viQueryf(vi,"SYSTem:ERRor?\n","%T",buff);

        //        if(QString(buff) != "0,\"No error\"\n")
        //        {
        //            qDebug () <<"Error setOutput(QString canal, bool OnOff) canal: "<<canal<<" and V="<<qPrintable(V)<<":  " << buff;
        //        }
    }
    else
    {
        switch (canal.toInt())
        {
        case 1:
        {
            viPrintf(vi, "CURRent:LEVel %s,(@1)\r\n",qPrintable(I));

            //            // Check errors
            //            char* buff = new char();
            //            viQueryf(vi,"SYSTem:ERRor?\n","%T",buff);

            //            if(QString(buff) != "0,\"No error\"\n")
            //            {
            //                qDebug () <<"Error setOutput(QString canal, bool OnOff) canal: "<<canal<<" and V="<<qPrintable(V)<<":  " << buff;
            //            }
        }
        case 2:
        {
            viPrintf(vi, "CURRent:LEVel %s,(@2)\r\n",qPrintable(I));

            //            // Check errors
            //            char* buff = new char();
            //            viQueryf(vi,"SYSTem:ERRor?\n","%T",buff);

            //            if(QString(buff) != "0,\"No error\"\n")
            //            {
            //                qDebug () <<"Error setOutput(QString canal, bool OnOff) canal: "<<canal<<" and V="<<qPrintable(V)<<":  " << buff;
            //            }
        }
        case 3:
        {
            viPrintf(vi, "CURRent:LEVel %s,(@3)\r\n",qPrintable(I));

            //            // Check errors
            //            char* buff = new char();
            //            viQueryf(vi,"SYSTem:ERRor?\n","%T",buff);

            //            if(QString(buff) != "0,\"No error\"\n")
            //            {
            //                qDebug () <<"Error setOutput(QString canal, bool OnOff) canal: "<<canal<<" and V="<<qPrintable(V)<<":  " << buff;
            //            }
        }
        case 4:
        {
            viPrintf(vi, "CURRent:LEVel %s,(@4)\r\n",qPrintable(I));

            //            // Check errors
            //            char* buff = new char();
            //            viQueryf(vi,"SYSTem:ERRor?\n","%T",buff);

            //            if(QString(buff) != "0,\"No error\"\n")
            //            {
            //                qDebug () <<"Error setOutput(QString canal, bool OnOff) canal: "<<canal<<" and V="<<qPrintable(V)<<":  " << buff;
            //            }
        }
        }
    }
}

QString N6700::getOutput(QString canal)
{
    char Char[100];


    switch (canal.toInt())
    {
    case 1:
        {
            viQueryf(vi,"OUTPut:STATe? (@1)\t\n","%T",Char);
            break;
        }
    case 2:
        {
            viQueryf(vi,"OUTPut:STATe? (@2)\t\n","%T",Char);
            break;
        }
    case 3:
        {
            viQueryf(vi,"OUTPut:STATe? (@3)\t\n","%T",Char);
            break;
        }
    case 4:
        {
            viQueryf(vi,"OUTPut:STATe? (@4)\t\n","%T",Char);
            break;
        }
    }
     return qPrintable(Char);
}


char buffEror[100];

QString N6700::getEror()
{
    // Check errors
    viQueryf(vi,"SYSTem:ERRor?\n","%T",buffEror);

    if(QString(buffEror) == "0,\"No error\"\n")
    {
        //  qDebug () << bufID;
        return qPrintable(buffEror);
    }
    else
    {
        //  qDebug () <<"Error getSVmodel(QString id) : " << buff;
        return qPrintable(buffEror);
    }
}
QVector<QString> N6700::getSetVolt(QString canal)
{
    QVector<QString> vector;

    char Char[100];

    if(canal =="ALL" || canal =="all")
    {
        viQueryf(vi,"VOLTage:LEVel? (@1)\t\n","%T",Char);
        getEror();
        vector.append(qPrintable(Char));
        Char[100] = NULL;
        viQueryf(vi,"VOLTage:LEVel? (@2)\t\n","%T",Char);
        getEror();
        vector.append(qPrintable(Char));
        Char[100] = NULL;
        viQueryf(vi,"VOLTage:LEVel? (@3)\t\n","%T",Char);
        getEror();
        vector.append(qPrintable(Char));
        Char[100] = NULL;
        viQueryf(vi,"VOLTage:LEVel? (@4)\t\n","%T",Char);
        getEror();
        vector.append(qPrintable(Char));
        Char[100] = NULL;

        return vector;
    }
    else
    {
        switch (canal.toInt())
        {
        case 1:
        {
            viQueryf(vi,"VOLTage:LEVel? (@1)\t\n","%T",Char);
            getEror();
            vector.append(qPrintable(Char));
            vector.append(NULL);
            vector.append(NULL);
            vector.append(NULL);
            Char[100] = NULL;
        }
        case 2:
        {
            viQueryf(vi,"VOLTage:LEVel? (@2)\t\n","%T",Char);
            getEror();
            vector.append(NULL);
            vector.append(qPrintable(Char));
            vector.append(NULL);
            vector.append(NULL);
            Char[100] = NULL;
        }
        case 3:
        {
            viQueryf(vi,"VOLTage:LEVel? (@3)\t\n","%T",Char);
            getEror();
            vector.append(NULL);
            vector.append(NULL);
            vector.append(qPrintable(Char));
            vector.append(NULL);
            Char[100] = NULL;
        }
        case 4:
        {
            viQueryf(vi,"VOLTage:LEVel? (@4)\t\n","%T",Char);
            getEror();
            vector.append(NULL);
            vector.append(NULL);
            vector.append(NULL);
            vector.append(qPrintable(Char));
            Char[100] = NULL;
        }
        }
    }

}

void N6700::endWork()
{
    emit TimerStop();


    flag_work = false;

}


void N6700::Work()
{

    getMeasureCURRentALL();
    getMeasureVoltALL();

}


QVector<QString> VectorMeasureVoltALL;



void N6700::getMeasureVoltALL()
{
    char MeasureVoltALL[100];

    VectorMeasureVoltALL.clear();

    viQueryf(vi, "MEASure:VOLTage? (@1)\n","%T",MeasureVoltALL);
    getEror();
    VectorMeasureVoltALL.append(qPrintable(MeasureVoltALL));
    MeasureVoltALL[100] = NULL;
    viQueryf(vi,"MEASure:VOLTage? (@2)\t\n","%T",MeasureVoltALL);
    getEror();
    VectorMeasureVoltALL.append(qPrintable(MeasureVoltALL));
    MeasureVoltALL[100] = NULL;
    viQueryf(vi,"MEASure:VOLTage? (@3)\t\n","%T",MeasureVoltALL);
    getEror();
    VectorMeasureVoltALL.append(qPrintable(MeasureVoltALL));
    MeasureVoltALL[100] = NULL;
    viQueryf(vi,"MEASure:VOLTage? (@4)\t\n","%T",MeasureVoltALL);
    getEror();
    VectorMeasureVoltALL.append(qPrintable(MeasureVoltALL));

    MeasureVoltALL[100] = NULL;

    //qDebug () << VectorMeasureVoltALL;

    emit getMeasureVoltSignal(VectorMeasureVoltALL);


}

QVector<QString> VectorMeasureCURRentALL;


void N6700::getMeasureCURRentALL()
{
    char MeasureCURRentALL[100];

    VectorMeasureCURRentALL.clear();

    viQueryf(vi, "MEASure:CURRent? (@1)\n","%T",MeasureCURRentALL);
    getEror();
    VectorMeasureCURRentALL.append(qPrintable(MeasureCURRentALL));
    MeasureCURRentALL[100] = NULL;
    viQueryf(vi,"MEASure:CURRent? (@2)\t\n","%T",MeasureCURRentALL);
    getEror();
    VectorMeasureCURRentALL.append(qPrintable(MeasureCURRentALL));
    MeasureCURRentALL[100] = NULL;
    viQueryf(vi,"MEASure:CURRent? (@3)\t\n","%T",MeasureCURRentALL);
    getEror();
    VectorMeasureCURRentALL.append(qPrintable(MeasureCURRentALL));
    MeasureCURRentALL[100] = NULL;
    viQueryf(vi,"MEASure:CURRent? (@4)\t\n","%T",MeasureCURRentALL);
    getEror();
    VectorMeasureCURRentALL.append(qPrintable(MeasureCURRentALL));

    MeasureCURRentALL[100] = NULL;


    emit getMeasureCURRentSignal(VectorMeasureCURRentALL);

  //  qDebug() << VectorMeasureCURRentALL;

}

QVector<QString> N6700::getMeasureVolt(QString canal)
{
    QVector<QString> vector;

    char Char[100];

    if(canal =="ALL" || canal =="all")
    {
        viQueryf(vi,"MEASure:VOLTage? (@1)\t\n","%T",Char);
        getEror();
        vector.append(qPrintable(Char));
        Char[100] = NULL;
        viQueryf(vi,"MEASure:VOLTage? (@2)\t\n","%T",Char);
        getEror();
        vector.append(qPrintable(Char));
        Char[100] = NULL;
        viQueryf(vi,"MEASure:VOLTage? (@3)\t\n","%T",Char);
        getEror();
        vector.append(qPrintable(Char));
        Char[100] = NULL;
        viQueryf(vi,"MEASure:VOLTage? (@4)\t\n","%T",Char);
        getEror();
        vector.append(qPrintable(Char));
        Char[100] = NULL;

        return vector;
    }
    else
    {

    }
}
