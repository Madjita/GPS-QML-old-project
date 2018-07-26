#include "gsg.h"
#include <QDateTime>

GSG::GSG(QObject *parent) : QObject(parent),
    ManufacturerDefault("SPECTRACOM"),
    flag_work(false),
    flagStart(false)
{

    this->moveToThread(new QThread()); //Переместили класс GSG в новый поток
    qDebug () << "Помещаем класс  << GSG >> в поток: " << this->thread();

    QObject::connect(this->thread(),&QThread::started,this,&GSG::process_start);
    this->thread()->start();
    qDebug () << "Запускаем поток << gsg >> : " << this->thread();

}





void GSG::process_start()
{
    connected = false;
    NewDataSpytnik.clear();

    //Инициализация таймеров
    QTime midnight(0,0,0);
    qsrand(midnight.secsTo(QTime::currentTime()));

    timer_Imitator_Data = new QTimer();

    QObject::connect(timer_Imitator_Data,&QTimer::timeout,this,&GSG::Work);

    QObject::connect(this,SIGNAL(startTimer(int)),timer_Imitator_Data,SLOT(start(int)));
    QObject::connect(this,&GSG::TimerStop,timer_Imitator_Data,&QTimer::stop);
}






void GSG::endWork()
{
    emit TimerStop();
    flag_work = false;

}






//Функция работы таймера который обновляет информацию в окне Имитатора
void GSG::Work()
{

    if(getCONTrol() == "START\n")
    {
        if(flagStart == false)
        {
            emit signal_QMLStart();
            flagStart = true;
        }

        dataPower.clear();

        //  getScenDataTime();

        getNumberIDSpytnik();

        for(int i=0; i < NewDataSpytnik.split(',').count();i++)
        {

            if(i !=NewDataSpytnik.split(',').count()-1)
            {
                dataPower += getPOWer(NewDataSpytnik.split(',').value(i))+",";
            }
            else
            {
                dataPower += getPOWer(NewDataSpytnik.split(',').value(i));
            }

        }



        if(NewDataSpytnikID != dataPower)
        {


            //  setPOWerALL(40,"ALL");

            NewDataSpytnikID = dataPower;

            if(flag_work)
            {

               // qDebug() << "NewDataSpytnikID =  " << NewDataSpytnikID;
               // qDebug() << "dataPower =  " << dataPower;


                emit UpdateScenPow(NewDataSpytnikID);

                //Можно Убрать
                dataPower.clear();
                NewDataSpytnikID.clear();
            }

        }
    }
}



bool GSG::connectDevice(QString ip)
{

    qDebug () << ip;
    viStatus=viOpenDefaultRM(&defaultRM);

    QString str  = "TCPIP0::"+ip+"::inst0::INSTR";

//    viAddres = (ViRsrc)qPrintable(str);

    // DisConnect();

    viStatus=viOpen(defaultRM, (ViRsrc)qPrintable("TCPIP0::"+ip+"::inst0::INSTR"), VI_NULL, VI_NULL,&vi); // проверено - работает через IP

    if(viStatus<VI_SUCCESS)
    {
        qDebug() << "[Соединение c Имитатором не установленно(ERROR)]";

        name = "Соединение c Имитатором не установленно.";

        connected = false;

        emit connectOk();

        return false;
    }
    else
    {
        connected = true;
        getName();

        qDebug() << "[Соединение c "<< name<<" установленно(ОК)]";

        //        auto listMode = name.split(",");

        //        mode.manufacturer = listMode[0];
        //        mode.model= listMode[1];
        //        mode.serialNumber = listMode[2];
        //        mode.firmwareLevel = listMode[3];
        //        mode.options = listMode[4];


        //        qDebug() << mode.manufacturer <<mode.model << mode.serialNumber;

        emit connectOk();

        return true;


    }
}



void GSG::slot_StartTimer()
{
//    if(timer_Imitator_Data->isActive())
//    {
//            qDebug () << "Уже запущен timer_Imitator_Data";
//    }
//    else
//    {
        emit startTimer(1000);
        flag_work = true;
        flagStart = false;
  //  }

}




//Разъединить соединение
void GSG::DisConnect()
{

    qDebug() << "dshfdhasfkljashdflkhasf timer_Imitator_Data" ;


    viClose( vi );

}


void GSG::getName()
{
    char nameChar[100] = "";

    viQueryf(vi,"*IDN?\t\n","%T",nameChar);

    name = qPrintable(nameChar);

}

/*
 *
 * The scenario must be loaded beforehand usingSOURce:SCENario:LOAD.
 * Calling a START command will first automatically stop any running scenarios.
 * HOLD can be used to pause and resume trajectory movement, not the entire scenario.
 * HOLD is effective when a scenario is running.
 * ARMing a scenario means to hold a scenario before it is started.
 *
 * command : START, STOP, HOLD, WAIT, ARMED or ARMING
 * */
void GSG::setCONTrol(QString command)
{
    viPrintf(vi, "SOURce:SCENario:CONTrol %s\r\n",qPrintable(command));

    // Check errors
    char buff[100] = " ";
    viQueryf(vi,"SYSTem:ERRor?\n","%T",buff);

    //qDebug () << buff;

    if(QString(buff) != "0,\"No error\"\n")
    {
        qDebug () <<"Error setCONTrol(QString command) : " << buff;
    }

}

void GSG::setGenCONTrol(QString command)
{
    viPrintf(vi, "SOUR:ONECHN:CONT %s\r\n",qPrintable(command));

    // Check errors
    char buff[100] = " ";
    viQueryf(vi,"SYSTem:ERRor?\n","%T",buff);

   // qDebug () << buff;

    if(QString(buff) != "0,\"No error\"\n")
    {
        qDebug () <<"Error setCONTrol(QString command) : " << buff;
    }
}

//Parameter
//letter [G, R, E, C, J, I, S, , PG, PR, PE, PC, PJ, PI, UG, UR, UE, UC, UJ, UI]
//decimal [-7-210]
//Example: SOURce:ONECHN:SATid G11
void GSG::setGenSATid(QString Np_liter)
{

    viPrintf(vi, "SOUR:ONECHN:CONT STOP\r\n");

    qDebug () << " Np_liter = " <<  Np_liter;

    viPrintf(vi, "SOURce:ONECHN:SATid %s\r\n",qPrintable(Np_liter));

    // Check errors
    char buff[100] = " ";
    viQueryf(vi,"SYSTem:ERRor?\n","%T",buff);

    qDebug () << buff;

    if(QString(buff) != "0,\"No error\"\n")
    {
        qDebug () <<"Error setCONTrol(QString command) : " << buff;
    }


}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Satellite system GPS/GLONASS/GALILEO, BeiDou/QZSS/IRNSS and modulation are set with
//the SOURce:ONECHN:satID command.
//Parameters
//String GPSL1CA,GPSL1P,GPSL1PY, GPSL2P,GPS L2PY for GPS
//String GLOL1,GLOL2 for GLONASS
//String GALE1,GALE5a,GALE5b for Galileo
//String BDSB1, BDSB2 for BeiDou
//String QZSSL1CA, QZSSL1SAIF, QZSSL2C, QZSSL5 for QZSS
//String IRNSSL5 for IRNSS

void GSG::setGenSIGNALtype(QString type_signal)
{
    viPrintf(vi, "SOURce:ONECHN:SIGNALtype %s\r\n",qPrintable(type_signal));  //GLOL1 and  GPSL1P

    // Check errors
    char buff[100] = " ";
    viQueryf(vi,"SYSTem:ERRor?\n","%T",buff);

   // qDebug () << buff;

    if(QString(buff) != "0,\"No error\"\n")
    {
        qDebug () <<"Error setCONTrol(QString command) : " << buff;
    }

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *
 *Query the current state of scenario execution.
 * Meaning of returned values is the following:
 * START: scenario is started and running
 * STOP: scenario is stopped and thus not running
 * HOLD: scenario is running, but the trajectory is on hold
 * WAIT: scenario delays startup for 2 minutes to allow the simulation to load required data.
 * The start time derived from the NTP server is then aligned to the next full GPS minute.
 * ARMED: scenario is armed, all data loading is done, but scenario is not yet running but waiting for the trigger to start it
 * ARMING: scenario is being loaded to memory after which it is in ARMED statearted.
 *
 * command : START, STOP, HOLD, WAIT, ARMED or ARMING
 * */




QString GSG::getCONTrol()
{
    char bufQuery [100] = "";
    char buff [100] = "";
    // char* bufQuery = new char();

    viQueryf(vi, "SOURce:SCENario:CONTrol?\n","%T",bufQuery);

    // Check errors
    //  char* buff = new char();

    viQueryf(vi,"SYSTem:ERRor?\n","%T",buff);


    if(QString(buff) == "0,\"No error\"\n")
    {
        emit signal_getCONTrol(qPrintable(bufQuery));

        return qPrintable(bufQuery);
    }
    else
    {
        emit signal_getCONTrol(qPrintable(buff));

        return qPrintable(buff);
    }

}



/*
 * String GPSL1CA,GPSL1P,GPSL1PY, GPSL2P,GPSL2PY  for GPS
 * String GLOL1,GLOL2 for GLONASS
 * String GALE1,GALE5a,GALE5b for Galileo
 * String BDSB1, BDSB2 for BeiDou
 * String QZSSL1CA, QZSSL1SAIF, QZSSL2C, QZSSL5 for QZSS
 * String IRNSSL5 for IRNSS
 * */
void GSG::setSIGNALtype(QString SIGNALtype)
{

    viPrintf(vi, "SOURce:ONECHN:SIGNALtype %T\r\n",&SIGNALtype);

    // Check errors
    char buff[100] = "";
    viQueryf(vi,"SYSTem:ERRor?\n","%T",buff);

    if(QString(buff) != "0,\"No error\"\n")
    {
        qDebug () <<"Error setSIGNALtype(QString SIGNALtype) : " << buff;
    }
}

/*
 * Query 1-channel signal type in use. Signal type consists of comma-separated list of the simulated signals.
 *
 * */
QString GSG::getSIGNALtype()
{
    char bufQuery[100] = "";

    viQueryf(vi, "SOURce:ONECHN:SIGNALtype?\n","%T",bufQuery);

    // Check errors
    char buff[100] = "";

    viQueryf(vi,"SYSTem:ERRor?\n","%T",buff);


    if(QString(buff) == "0,\"No error\"\n")
    {
        emit signal_getSIGNALtype(qPrintable(bufQuery));

        return qPrintable(bufQuery);
    }
    else
    {
        emit signal_getSIGNALtype(qPrintable(buff));

        return qPrintable(buff);
    }
}

/*
 * Query signal type of satellite. Signal type consists of comma separated list of frequency
 * bands and codes (CA or P code) for GPS and frequency bands for GLONASS, Galileo, BeiDou, QZSS and IRNSS.
 * */
QString GSG::getSIGNALtype(QString id)
{
    //char* bufSignal = new char();

    char bufSignal[100] = "";


    viQueryf(vi, "SOUR:SCEN:SIGNAL? %s\r\n","%T",qPrintable(id),bufSignal);

    // Check errors
    char buff[100] = "";

    viQueryf(vi,"SYSTem:ERRor?\n","%T",buff);


    if(QString(buff) == "0,\"No error\"\n")
    {
        emit signal_getSIGNALtype(qPrintable(bufSignal));

        return qPrintable(bufSignal);
    }
    else
    {
        emit signal_getSIGNALtype(qPrintable(buff));

        return buff;
    }
}

/*
 * Query the current satellite identifier of channel n.
 * The parameter n can be
 * 1-5 for GSG-52/53,
 * 1-8 for GSG-54,
 * 1-16 for GSG-55/GSG-56
 * 1-32/48/64 for GSG-62/63/64.
 * The returned satellite identifier can be:
 * Gxx for GPS for example G12 Rxx for GLONASS,for example R15
 * Exx for Galileo, for example E01
 * Cxx for BeiDou, for example C11
 * Jxx for QZSS, for example J02
 * Ixx for IRNSS, for example I01
 * Sxxx for SBAS for example S120
 * UG for unmodulated GPS signal
 * UE for unmodulated Galileo signal
 * UC for unmodulated BeiDou signal
 * UJ for unmodulated QZSS signal
 * UI, for unmodulated IRNSS signal
 * URx for unmodulated GLONASS signal.
 * X is the frequency slot from -7 to 6
 * Would the signal be a multipath signal, this is identified by an added character D at the end.
 * The satID is returned with a leading timestamp.
 *
 * */
QString GSG::getSATid(int n)
{
    char bufID[100] = "";

    viQueryf(vi, "SOURce:SCENario:SATid%d?\n","%T",n,bufID);

    // Check errors
    char buff[100] = "";

    viQueryf(vi,"SYSTem:ERRor?\n","%T",buff);


    if(QString(buff) == "0,\"No error\"\n")
    {
        emit signal_getSATid(QString(bufID));

        return QString(bufID);
    }
    else
    {
        emit signal_getSATid(QString(buff));

        return buff;
    }
}

/*
 * Query the current frequency setting ofn when scenario is running.
 * The parameter n can be 1-8 for GSG-54, 1-16 for GSG-55/56 and 1-32/48/64 for GSG-62/63/64.
 * The frequency is returned with a leading timestamp.
 * */
QString GSG::getFREQuency(int n)
{
    char bufID[100] = "";

    viQueryf(vi, "SOURce:SCENario:FREQuency%d?\n","%T",n,bufID);

    // Check errors
    char buff[100] = "";

    viQueryf(vi,"SYSTem:ERRor?\n","%T",buff);


    if(QString(buff) == "0,\"No error\"\n")
    {
        emit signal_getFREQuency(QString(bufID));

        return QString(bufID);
    }
    else
    {
        emit signal_getFREQuency(QString(buff));

        return buff;
    }
}

/*
 *Query the current frequency setting of channel satID when scenario is running.
 *  The frequency is returned with a leading timestamp.
 * */
QString GSG::getFREQuency(QString id)
{
    char bufID[100] = "";

    viQueryf(vi, "SOURce:SCENario:FREQuency? %s\n","%T",qPrintable(id),bufID);

    // Check errors
    char buff[100] = "";

    viQueryf(vi,"SYSTem:ERRor?\n","%T",buff);


    if(QString(buff) == "0,\"No error\"\n")
    {
        qDebug () << bufID;
        return QString(bufID);
    }
    else
    {
        qDebug () <<"Error getFREQuency : " << buff;
        return buff;
    }
}

/*
 *
 * Set the power of channel n when scenario is running.
 * The parameter n can be: 1-5 for GSG-52/53 1-8 for GSG-54 1-16 for GSG-55/56 1-32/48/64 for GSG-62/63/64.
 * The freqband parameter is optional and can be used when only a certain satellite frequency band power is changed.
 * The value ALL in freqband means that the power for all bands is adjusted by the amount indicated via the command.
 *
 * Decimal [-160.0,-65.0] dBm, if freqband is not ALL. With ALL, the limits are [-100,100] dBm. FreqBand [L1, L2, L5, ALL]
 * */
void GSG::setPOWer(int id,int power,QString freqband)
{

    viPrintf(vi, "SOURce:SCENario:POWer%d %d,%s\r\n",id,power,qPrintable(freqband));

    // Check errors
    char buff[100] = "";

    viQueryf(vi,"SYSTem:ERRor?\n","%T",buff);

    if(QString(buff) != "0,\"No error\"\n")
    {
        qDebug () <<"Error setPOWer : " << buff;
    }
}

/*
 *
 * Set the power of satellite satID when scenario is running.
 * Freqband parameter is optional and can be used when only certain frequency band power of satellite is changed.
 * Value ALL in freqband means that power of all bands are adjusted by the amount indicated by the command.
 * */
void GSG::setPOWer(QString id,int power,QString freqband)
{

    viPrintf(vi, "SOURce:SCENario:POWer %s,%d,%s\r\n",qPrintable(id),power,qPrintable(freqband));

    // Check errors
    char buff[100] = "";
    viQueryf(vi,"SYSTem:ERRor?\n","%T",buff);

    if(QString(buff) != "0,\"No error\"\n")
    {
        qDebug () <<"Error setPOWer : " << buff;
    }
}

/*
 * Set the power for a frequency band (all satellites) when scenario is running.
 * Freqband is used to specify the frequency band.
 * The freqband value ALL means that the power for all bands is adjusted by the amount indicated.
 *
 * Decimal [-160.0,-65.0] dBm if freqband is not ALL. For ALL, the limits are [-100,100] dB. FreqBand [L1, L2, L5, ALL]
 * */
void GSG::setPOWerALL(int power,QString freqband)
{

    viPrintf(vi, "SOURce:SCENario:FREQBAND:POWer %d,%s\r\n",power,qPrintable(freqband));

    // Check errors
    //  char* buff = new char();

    char buff[100] = "";

    viQueryf(vi,"SYSTem:ERRor?\n","%T",buff);

    if(QString(buff) != "0,\"No error\"\n")
    {
        qDebug () <<"Error setPOWerALL(int power,QString freqband) : " << buff;
    }
}

/*
 *
 * Query the current power setting of channel n during scenario execution.
 * The parameter n can be: 1-5 for GSG-52/53 1-8 for GSG-54 1-16 for GSG-55/56 1-32/48/64 for GSG-62/63/64.
 * The power is returned with a leading timestamp. Freqband is an optional parameter used to specify for which frequency band the power is returned.
 * If the freqband parameter is omitted, the L1 power is returned.
 * */
QString GSG::getPOWer(int id,QString freqband)
{

    char bufID[100] = "";

    viQueryf(vi, "SOURce:SCENario:POWer%d? %s\n","%T",id,qPrintable(freqband),bufID);

    // Check errors
    char buff[100] = "";

    viQueryf(vi,"SYSTem:ERRor?\n","%T",buff);


    if(QString(buff) == "0,\"No error\"\n")
    {
        emit signal_getPOWer(QString(bufID));

        return QString(bufID);
    }
    else
    {
        emit signal_getPOWer(QString(buff));

        return buff;
    }
}

/*
 *Query the current power setting of the satellite satID during scenario execution.
 * The power is returned with a leading timestamp.
 * Freqband is an optional parameter used to specify the frequency band whose power is returned.
 * If freqband is omitted, the L1 power is returned.
 *
 **/
QString GSG::getPOWer(QString id)
{

    char bufID[100] = "";

    viQueryf(vi, "SOURce:SCENario:POWer? %s\n","%T",qPrintable(id),bufID);

    // Check errors
    char buff[100]="";

    viQueryf(vi,"SYSTem:ERRor?\n","%T",buff);


    if(QString(buff) == "0,\"No error\"\n")
    {
        emit signal_getPOWer(QString(bufID).split(',').last());

        return QString(bufID).split(',').last();
    }
    else
    {
        emit signal_getPOWer(QString(buff));

        return QString(buff);
    }

}

/*
 * Query the satellite’s Space Vehicle model.
 * The parameter n can be: 1-5 for GSG-52/53 1-8 for GSG-54 1-16 for GSG-55/GSG-56 1-32 for GSG-62.
 * The Space Vehicle model can be: Block II, Block IIA, Block IIR, Block IIR-M, Block IIF or Block IIIA for GPS Glonass-M or Glonass-K1 for GLONASS
 * */
QString GSG::getSVmodel(int id)
{
    char bufID[100] = "";

    viQueryf(vi, "SOURce:SCENario:SVmodel%d? %s\n","%T",id,bufID);

    // Check errors
    char buff[100] = "";

    viQueryf(vi,"SYSTem:ERRor?\n","%T",buff);


    if(QString(buff) == "0,\"No error\"\n")
    {
        emit signal_getSVmodel(QString(bufID));

        return QString(bufID);
    }
    else
    {
        emit signal_getSVmodel(QString(buff));

        return buff;
    }
}

/*
 *
 * Query the satellite’s Space Vehicle model.
 * The Space Vehicle model can be: Block II, Block IIA, Block IIR, Block IIR-M, Block IIF or Block IIIA for GPS Glonass-M or Glonass-K1 for GLONASS
 *
 * */
QString GSG::getSVmodel(QString id)
{
    char bufID[100] = "";

    viQueryf(vi, "SOURce:SCENario:SVmodel? %s %s\n","%T",qPrintable(id),bufID);

    // Check errors
    char buff[100] = "";

    viQueryf(vi,"SYSTem:ERRor?\n","%T",buff);


    if(QString(buff) == "0,\"No error\"\n")
    {
        qDebug () << bufID;
        return QString(bufID);
    }
    else
    {
        qDebug () <<"Error getSVmodel(QString id) : " << buff;
        return buff;
    }
}


QString GSG::getPosition()
{
    char bufID[100];

    // viPrintf(vi, "*WAI\r\n");

    viQueryf(vi, "SOURce:SCENario:LOG?\n","%T",bufID);


    // Check errors
    char buff[100];

    viQueryf(vi,"SYSTem:ERRor?\n","%T",buff);


    if(QString(buff) == "0,\"No error\"\n")
    {
        //  qDebug () << bufID;
        return qPrintable(bufID);
    }
    else
    {
        //  qDebug () <<"Error getSVmodel(QString id) : " << buff;
        return qPrintable(buff);
    }
}






void GSG::getScenDataTime()
{
    char bufDataTime[100] = "";
    char bufDataTimeError[100] = "";

    viQueryf(vi, "SOURce:SCENario:DATEtime? UTC\n","%T",&bufDataTime);


    // Check errors
    viQueryf(vi,"SYSTem:ERRor?\n","%T",&bufDataTimeError);

    // qDebug () << QString(qPrintable(bufDataTime)).split("\n").first();

    if(QString(bufDataTimeError) == "0,\"No error\"\n")
    {
        emit UpdateScenDataTime(QString(qPrintable(bufDataTime)).split("\n").first());

        //  return QString(qPrintable(bufID)).split("\n").first();
    }
    else
    {
        emit UpdateScenDataTime(QString(qPrintable(bufDataTime)).split("\n").first());

        // return QString(qPrintable(bufID)).split("\n").first();
    }


}


QString GSG::getNumberIDSpytnik()
{
    char bufID[100]= "";


    viQueryf(vi, "SOURce:SCENario:SVINview?\n","%T",bufID);


    // Check errors
    char buff[100]= "";

    viQueryf(vi,"SYSTem:ERRor?\n","%T",buff);



    if(QString(buff) == "0,\"No error\"\n")
    {

        if(NewDataSpytnik != QString(qPrintable(bufID)))
        {
            NewDataSpytnik.clear();

            NewDataSpytnik = QString(qPrintable(bufID));

            emit UpdateScenNumberIDSpytnik(NewDataSpytnik); // сигнал строки спутников ("G4,G10,R6,R21,G2,G17,G12,R20,R22,R5,R7,R12,G5,G13,G23,G25,R11,R13\n")

        }

        //  qDebug () << bufID;
        return QString(qPrintable(bufID));



    }
    else
    {
        //  qDebug () <<"Error getSVmodel(QString id) : " << buff;
        emit UpdateScenNumberIDSpytnik(QString(qPrintable(buff)));

        return QString(qPrintable(buff));
    }
}

