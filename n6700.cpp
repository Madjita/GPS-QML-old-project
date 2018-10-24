#include "n6700.h"

#include <QDateTime>

#include <QDateTime>

#define errorMeasure 0//0.035

N6700::N6700(QObject *parent) : QObject(parent),
    flag_work(false)

{

    this->moveToThread(new QThread()); //Переместили класс N6700 в новый поток
    QObject::connect(this->thread(),&QThread::started,this,&N6700::process_start);
    connect(this,&N6700::signal_finished, this->thread(), &QThread::quit);
    this->thread()->start();


    qRegisterMetaType<QVector<QString>>();

}

N6700::~N6700()
{
    if(connected)
    {
        setOutput("all",false);
        viClose(vi);
    }

    if(timer_IstP1_Measure->isActive())
        emit TimerStop();


    delete timer_IstP1_Measure;

    emit signal_finished();
}


void N6700::process_start()
{
    connected = false;

    //Инициализация таймеров
    timer_IstP1_Measure = new QTimer();

    QObject::connect(timer_IstP1_Measure,&QTimer::timeout,this,&N6700::Work);

    QObject::connect(this,SIGNAL(startTimer(int)),timer_IstP1_Measure,SLOT(start(int)));
    QObject::connect(this,&N6700::TimerStop,timer_IstP1_Measure,&QTimer::stop);
}



void N6700::slot_StartTimer()
{
    emit startTimer(1000);
    flag_work = true;
}



bool N6700::Connect(QString ip)
{

    viStatus=viOpenDefaultRM(&defaultRM);

    QString str  = "TCPIP0::"+ip+"::inst0::INSTR";

    viStatus=viOpen(defaultRM,   const_cast<ViRsrc>(qPrintable("TCPIP0::"+ip+"::inst0::INSTR")), VI_NULL, VI_NULL,&vi); // проверено - работает через IP

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

        setVolt("all","5");
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

    viQueryf(vi,const_cast<ViString>("*IDN?\t\n"),const_cast<ViString>("%T"),nameChar);

    name = qPrintable(nameChar);
}

void N6700::setOutput(QString canal, bool OnOff)
{
    mut.lock();

    if(canal =="ALL" || canal =="all")
    {
        switch (static_cast<int>(OnOff)) {
        case true:
            viPrintf(vi, const_cast<ViString>("OUTPut:STATe 1,(@1)\r\n"));
            viPrintf(vi, const_cast<ViString>("OUTPut:STATe 1,(@2)\r\n"));
            viPrintf(vi, const_cast<ViString>("OUTPut:STATe 1,(@3)\r\n"));
            viPrintf(vi, const_cast<ViString>("OUTPut:STATe 1,(@4)\r\n"));
            break;
        case false:
            viPrintf(vi, const_cast<ViString>("OUTPut:STATe 0,(@1)\r\n"));
            viPrintf(vi, const_cast<ViString>("OUTPut:STATe 0,(@2)\r\n"));
            viPrintf(vi, const_cast<ViString>("OUTPut:STATe 0,(@3)\r\n"));
            viPrintf(vi, const_cast<ViString>("OUTPut:STATe 0,(@4)\r\n"));
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
            switch (static_cast<int>(OnOff)) {
            case true:
                viPrintf(vi, const_cast<ViString>("OUTPut:STATe 1,(@1)\r\n"));
                break;
            case false:
                viPrintf(vi, const_cast<ViString>("OUTPut:STATe 0,(@1)\r\n"));
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
            switch (static_cast<int>(OnOff)) {
            case true:
                viPrintf(vi, const_cast<ViString>("OUTPut:STATe 1,(@2)\r\n"));
                break;
            case false:
                viPrintf(vi, const_cast<ViString>("OUTPut:STATe 0,(@2)\r\n"));
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
            switch (static_cast<int>(OnOff)) {
            case true:
                viPrintf(vi, const_cast<ViString>("OUTPut:STATe 1,(@3)\r\n"));
                break;
            case false:
                viPrintf(vi, const_cast<ViString>("OUTPut:STATe 0,(@3)\r\n"));
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
            switch (static_cast<int>(OnOff)) {
            case true:
                viPrintf(vi, const_cast<ViString>("OUTPut:STATe 1,(@4)\r\n"));
                break;
            case false:
                viPrintf(vi, const_cast<ViString>("OUTPut:STATe 0,(@4)\r\n"));
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

    mut.unlock();
}


void N6700::setVolt(QString canal, QString V)
{
    if(canal =="ALL" || canal =="all")
    {
        viPrintf(vi, const_cast<ViString>("VOLTage:LEVel %s,(@1)\r\n"),qPrintable(V));
        viPrintf(vi, const_cast<ViString>("VOLTage:LEVel %s,(@2)\r\n"),qPrintable(V));
        viPrintf(vi, const_cast<ViString>("VOLTage:LEVel %s,(@3)\r\n"),qPrintable(V));
        viPrintf(vi, const_cast<ViString>("VOLTage:LEVel %s,(@4)\r\n"),qPrintable(V));

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
            viPrintf(vi, const_cast<ViString>("VOLTage:LEVel %s,(@1)\r\n"),qPrintable(V));

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
            viPrintf(vi, const_cast<ViString>("VOLTage:LEVel %s,(@2)\r\n"),qPrintable(V));

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
            viPrintf(vi, const_cast<ViString>("VOLTage:LEVel %s,(@3)\r\n"),qPrintable(V));

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
            viPrintf(vi, const_cast<ViString>("VOLTage:LEVel %s,(@4)\r\n"),qPrintable(V));

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
        viPrintf(vi, const_cast<ViString>("CURRent:LEVel %s,(@1)\r\n"),qPrintable(I));
        viPrintf(vi, const_cast<ViString>("CURRent:LEVel %s,(@2)\r\n"),qPrintable(I));
        viPrintf(vi, const_cast<ViString>("CURRent:LEVel %s,(@3)\r\n"),qPrintable(I));
        viPrintf(vi, const_cast<ViString>("CURRent:LEVel %s,(@4)\r\n"),qPrintable(I));

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
            viPrintf(vi, const_cast<ViString>("CURRent:LEVel %s,(@1)\r\n"),qPrintable(I));

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
            viPrintf(vi, const_cast<ViString>("CURRent:LEVel %s,(@2)\r\n"),qPrintable(I));

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
            viPrintf(vi, const_cast<ViString>("CURRent:LEVel %s,(@3)\r\n"),qPrintable(I));

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
            viPrintf(vi, const_cast<ViString>("CURRent:LEVel %s,(@4)\r\n"),qPrintable(I));

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
        viQueryf(vi,const_cast<ViString>("OUTPut:STATe? (@1)\t\n"),const_cast<ViString>("%T"),Char);
        break;
    }
    case 2:
    {
        viQueryf(vi,const_cast<ViString>("OUTPut:STATe? (@2)\t\n"),const_cast<ViString>("%T"),Char);
        break;
    }
    case 3:
    {
        viQueryf(vi,const_cast<ViString>("OUTPut:STATe? (@3)\t\n"),const_cast<ViString>("%T"),Char);
        break;
    }
    case 4:
    {
        viQueryf(vi,const_cast<ViString>("OUTPut:STATe? (@4)\t\n"),const_cast<ViString>("%T"),Char);
        break;
    }
    }
    return qPrintable(Char);
}




QString N6700::getEror()
{
    // Check errors
    viQueryf(vi,const_cast<ViString>("SYSTem:ERRor?\n"),const_cast<ViString>("%T"),buffEror);

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
        viQueryf(vi,const_cast<ViString>("VOLTage:LEVel? (@1)\t\n"),const_cast<ViString>("%T"),Char);
        getEror();
        vector.append(qPrintable(Char));
        memset (Char, 0, sizeof(Char));
        viQueryf(vi,const_cast<ViString>("VOLTage:LEVel? (@2)\t\n"),const_cast<ViString>("%T"),Char);
        getEror();
        vector.append(qPrintable(Char));
        memset (Char, 0, sizeof(Char));
        viQueryf(vi,const_cast<ViString>("VOLTage:LEVel? (@3)\t\n"),const_cast<ViString>("%T"),Char);
        getEror();
        vector.append(qPrintable(Char));
        memset (Char, 0, sizeof(Char));
        viQueryf(vi,const_cast<ViString>("VOLTage:LEVel? (@4)\t\n"),const_cast<ViString>("%T"),Char);
        getEror();
        vector.append(qPrintable(Char));
        memset (Char, 0, sizeof(Char));

        return vector;
    }
    else
    {
        switch (canal.toInt())
        {
        case 1:
        {
            viQueryf(vi,const_cast<ViString>("VOLTage:LEVel? (@1)\t\n"),const_cast<ViString>("%T"),Char);
            getEror();
            vector.append(qPrintable(Char));
            vector.append(nullptr);
            vector.append(nullptr);
            vector.append(nullptr);
            memset (Char, 0, sizeof(Char));
        }
        case 2:
        {
            viQueryf(vi,const_cast<ViString>("VOLTage:LEVel? (@2)\t\n"),const_cast<ViString>("%T"),Char);
            getEror();
            vector.append(nullptr);
            vector.append(qPrintable(Char));
            vector.append(nullptr);
            vector.append(nullptr);
            memset (Char, 0, sizeof(Char));
        }
        case 3:
        {
            viQueryf(vi,const_cast<ViString>("VOLTage:LEVel? (@3)\t\n"),const_cast<ViString>("%T"),Char);
            getEror();
            vector.append(nullptr);
            vector.append(nullptr);
            vector.append(qPrintable(Char));
            vector.append(nullptr);
            memset (Char, 0, sizeof(Char));
        }
        case 4:
        {
            viQueryf(vi,const_cast<ViString>("VOLTage:LEVel? (@4)\t\n"),const_cast<ViString>("%T"),Char);
            getEror();
            vector.append(nullptr);
            vector.append(nullptr);
            vector.append(nullptr);
            vector.append(qPrintable(Char));
            memset (Char, 0, sizeof(Char));
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






void N6700::getMeasureVoltALL()
{
    char MeasureVoltALL[100];

    VectorMeasureVoltALL.clear();

    viQueryf(vi, const_cast<ViString>("MEASure:VOLTage? (@1)\n"),const_cast<ViString>("%T"),MeasureVoltALL);
    getEror();
    VectorMeasureVoltALL.append(qPrintable(MeasureVoltALL));
    memset (MeasureVoltALL, 0, sizeof(MeasureVoltALL));
    viQueryf(vi,const_cast<ViString>("MEASure:VOLTage? (@2)\t\n"),const_cast<ViString>("%T"),MeasureVoltALL);
    getEror();
    VectorMeasureVoltALL.append(qPrintable(MeasureVoltALL));
    memset (MeasureVoltALL, 0, sizeof(MeasureVoltALL));
    viQueryf(vi,const_cast<ViString>("MEASure:VOLTage? (@3)\t\n"),const_cast<ViString>("%T"),MeasureVoltALL);
    getEror();
    VectorMeasureVoltALL.append(qPrintable(MeasureVoltALL));
    memset (MeasureVoltALL, 0, sizeof(MeasureVoltALL));
    viQueryf(vi,const_cast<ViString>("MEASure:VOLTage? (@4)\t\n"),const_cast<ViString>("%T"),MeasureVoltALL);
    getEror();
    VectorMeasureVoltALL.append(qPrintable(MeasureVoltALL));

    memset (MeasureVoltALL, 0, sizeof(MeasureVoltALL));

    //qDebug () << VectorMeasureVoltALL;


    v_1 = QString::number(VectorMeasureVoltALL[0].split("\n").first().toDouble()-
            errorMeasure,'\0',3);
    v_2 = QString::number(VectorMeasureVoltALL[1].split("\n").first().toDouble()-errorMeasure,'\0',3);
    v_3 = QString::number(VectorMeasureVoltALL[2].split("\n").first().toDouble()-errorMeasure,'\0',3);
    v_4 = QString::number(VectorMeasureVoltALL[3].split("\n").first().toDouble()-errorMeasure,'\0',3);

//    qDebug() <<VectorMeasureVoltALL[0].split("\n").first() << "D = " << VectorMeasureVoltALL[0].split("\n").first().toDouble();
//    qDebug() <<VectorMeasureVoltALL[1].split("\n").first()<< "D = " << VectorMeasureVoltALL[1].split("\n").first().toDouble();
//    qDebug() <<VectorMeasureVoltALL[2].split("\n").first()<< "D = " << VectorMeasureVoltALL[2].split("\n").first().toDouble();
//    qDebug() <<VectorMeasureVoltALL[3].split("\n").first()<< "D = " << VectorMeasureVoltALL[3].split("\n").first().toDouble();



    emit getMeasureVoltSignal(VectorMeasureVoltALL);


}




void N6700::getMeasureCURRentALL()
{
    char MeasureCURRentALL[100];

    VectorMeasureCURRentALL.clear();

    viQueryf(vi, const_cast<ViString>("MEASure:CURRent? (@1)\n"),const_cast<ViString>("%T"),MeasureCURRentALL);
    getEror();
    VectorMeasureCURRentALL.append(qPrintable(MeasureCURRentALL));
    memset (MeasureCURRentALL, 0, sizeof(MeasureCURRentALL));
    viQueryf(vi,const_cast<ViString>("MEASure:CURRent? (@2)\t\n"),const_cast<ViString>("%T"),MeasureCURRentALL);
    getEror();
    VectorMeasureCURRentALL.append(qPrintable(MeasureCURRentALL));
    memset (MeasureCURRentALL, 0, sizeof(MeasureCURRentALL));
    viQueryf(vi,const_cast<ViString>("MEASure:CURRent? (@3)\t\n"),const_cast<ViString>("%T"),MeasureCURRentALL);
    getEror();
    VectorMeasureCURRentALL.append(qPrintable(MeasureCURRentALL));
    memset (MeasureCURRentALL, 0, sizeof(MeasureCURRentALL));
    viQueryf(vi,const_cast<ViString>("MEASure:CURRent? (@4)\t\n"),const_cast<ViString>("%T"),MeasureCURRentALL);
    getEror();
    VectorMeasureCURRentALL.append(qPrintable(MeasureCURRentALL));

    memset (MeasureCURRentALL, 0, sizeof(MeasureCURRentALL));


    i_1 = QString::number(VectorMeasureCURRentALL[0].split("\n").first().toDouble(),'\0',3);
    i_2 = QString::number(VectorMeasureCURRentALL[1].split("\n").first().toDouble(),'\0',3);
    i_3 = QString::number(VectorMeasureCURRentALL[2].split("\n").first().toDouble(),'\0',3);
    i_4 = QString::number(VectorMeasureCURRentALL[3].split("\n").first().toDouble(),'\0',3);


    emit getMeasureCURRentSignal(VectorMeasureCURRentALL);


}

QVector<QString> N6700::getMeasureCURRentALL_find()
{
    mut.lock();

    char MeasureCURRentALL[100];

    VectorMeasureCURRentALL.clear();

    viQueryf(vi, const_cast<ViString>("MEASure:CURRent? (@1)\n"),const_cast<ViString>("%T"),MeasureCURRentALL);
    //getEror();
    VectorMeasureCURRentALL.append(qPrintable(MeasureCURRentALL));
    memset (MeasureCURRentALL, 0, sizeof(MeasureCURRentALL));
    viQueryf(vi,const_cast<ViString>("MEASure:CURRent? (@2)\t\n"),const_cast<ViString>("%T"),MeasureCURRentALL);
    // getEror();
    VectorMeasureCURRentALL.append(qPrintable(MeasureCURRentALL));
    memset (MeasureCURRentALL, 0, sizeof(MeasureCURRentALL));
    viQueryf(vi,const_cast<ViString>("MEASure:CURRent? (@3)\t\n"),const_cast<ViString>("%T"),MeasureCURRentALL);
    // getEror();
    VectorMeasureCURRentALL.append(qPrintable(MeasureCURRentALL));
    memset (MeasureCURRentALL, 0, sizeof(MeasureCURRentALL));
    viQueryf(vi,const_cast<ViString>("MEASure:CURRent? (@4)\t\n"),const_cast<ViString>("%T"),MeasureCURRentALL);
    // getEror();
    VectorMeasureCURRentALL.append(qPrintable(MeasureCURRentALL));

    memset (MeasureCURRentALL, 0, sizeof(MeasureCURRentALL));


    i_1 = QString::number(VectorMeasureCURRentALL[0].split("\n").first().toDouble(),'\0',3);
    i_2 = QString::number(VectorMeasureCURRentALL[1].split("\n").first().toDouble(),'\0',3);
    i_3 = QString::number(VectorMeasureCURRentALL[2].split("\n").first().toDouble(),'\0',3);
    i_4 = QString::number(VectorMeasureCURRentALL[3].split("\n").first().toDouble(),'\0',3);

    mut.unlock();

    return VectorMeasureCURRentALL;


}

QVector<QString> N6700::getMeasureVolt(QString canal)
{
    QVector<QString> vector;

    char Char[100];

    if(canal =="ALL" || canal =="all")
    {
        viQueryf(vi,const_cast<ViString>("MEASure:VOLTage? (@1)\t\n"),const_cast<ViString>("%T"),Char);
        getEror();
        vector.append(qPrintable(Char));
        memset (Char, 0, sizeof(Char));
        viQueryf(vi,const_cast<ViString>("MEASure:VOLTage? (@2)\t\n"),const_cast<ViString>("%T"),Char);
        getEror();
        vector.append(qPrintable(Char));
        memset (Char, 0, sizeof(Char));
        viQueryf(vi,const_cast<ViString>("MEASure:VOLTage? (@3)\t\n"),const_cast<ViString>("%T"),Char);
        getEror();
        vector.append(qPrintable(Char));
        memset (Char, 0, sizeof(Char));
        viQueryf(vi,const_cast<ViString>("MEASure:VOLTage? (@4)\t\n"),const_cast<ViString>("%T"),Char);
        getEror();
        vector.append(qPrintable(Char));
        memset (Char, 0, sizeof(Char));

        return vector;
    }
    else
    {

    }
}
