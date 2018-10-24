#include "osuilograf.h"

#include "osuilograf.h"
#include <QDateTime>

Osuilograf::Osuilograf(QObject *parent) : QObject(parent),
    ManufacturerDefault("SPECTRACOM"),
    flag_work(false),
    flag_change_seitings(true),
    flag_change_seitings_3(false)

{


    this->moveToThread(new QThread()); //Переместили класс Osuilograf в новый поток
    connect(this->thread(),&QThread::started,this,&Osuilograf::process_start);
    connect(this,&Osuilograf::signal_finished, this->thread(), &QThread::quit);
    this->thread()->start();

}

Osuilograf::~Osuilograf()
{
    if(connected)
    {
        viClose(vi);
    }

    emit signal_finished();

    emit TimerStop();
    delete timer_Imitator_Data;

}



void Osuilograf::process_start()
{
    connected = false;
    NewDataSpytnik.clear();

    //Инициализация таймеров

    timer_Imitator_Data = new QTimer();

    QObject::connect(timer_Imitator_Data,&QTimer::timeout,this,&Osuilograf::Work);

    QObject::connect(this,SIGNAL(startTimer(int)),timer_Imitator_Data,SLOT(start(int)));
    QObject::connect(this,&Osuilograf::TimerStop,timer_Imitator_Data,&QTimer::stop);


    timer_Imitator_Data_Amplituda = new QTimer();

    QObject::connect(timer_Imitator_Data_Amplituda,&QTimer::timeout,this,&Osuilograf::Work_Amplituda);

    QObject::connect(this,SIGNAL(startTimer_Amplituda(int)),timer_Imitator_Data_Amplituda,SLOT(start(int)));
    QObject::connect(this,&Osuilograf::TimerStop,timer_Imitator_Data_Amplituda,&QTimer::stop);
}






void Osuilograf::endWork()
{
    emit TimerStop();
    flag_work = false;

}




//Функция работы таймера который обновляет информацию в окне Имитатора
void Osuilograf::Work()
{

    getDelay();
}


void Osuilograf::Work_Amplituda()
{
    auto Amplituda = getAmplitude().toDouble();

    if(Amplituda > 0 && Amplituda < 10 )
    {
        qDebug () << "GOOOD";
    }

}



bool Osuilograf::connectDevice(QString ip)
{
    viStatus=viOpenDefaultRM(&defaultRM);

    QString str  = "TCPIP0::"+ip+"::inst0::INSTR";

    viStatus=viOpen(defaultRM, const_cast<ViRsrc>(qPrintable("TCPIP0::"+ip+"::inst0::INSTR")), VI_NULL, VI_NULL,&vi); // проверено - работает через IP

    if(viStatus<VI_SUCCESS)
    {
        qDebug() << "[Соединение c Осцилографом не установленно(ERROR)]";

        name = "Соединение c Осцилографом не установленно.";

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

        setStartSeitings();

        return true;


    }
}



void Osuilograf::slot_StartTimer()
{

    emit startTimer(1000);

    emit startTimer_Amplituda(1000);

    flag_work = true;

}




//Разъединить соединение
void Osuilograf::DisConnect()
{

    qDebug() << "dshfdhasfkljashdflkhasf timer_Imitator_Data" ;


    viClose( vi );

}


void Osuilograf::getName()
{
    char nameChar[100] = "";

    viQueryf(vi,const_cast<ViString>("*IDN?\t\n"),const_cast<ViString>("%T"),nameChar);

    name = qPrintable(nameChar);

}

// Установка импеданса в 1 либо во 2 канал
//:CHANnel<n>:IMPedance <impedance>
//<impedance> ::= {ONEMeg | FIFTy}
void Osuilograf::setIMPedance(QString command , int control = 0 )
{
    switch (control)
    {
    case 1:
        viPrintf(vi, const_cast<ViString>(":CHANnel1:IMPedance %s\r\n"),qPrintable(command));
        break;
    case 2:
        viPrintf(vi, const_cast<ViString>(":CHANnel2:IMPedance %s\r\n"),qPrintable(command));
        break;
    default:
        viPrintf(vi, const_cast<ViString>(":CHANnel1:IMPedance %s\r\n"),qPrintable(command));
        viPrintf(vi, const_cast<ViString>(":CHANnel2:IMPedance %s\r\n"),qPrintable(command));
        break;
    }

}

void Osuilograf::setSCALe(QString command, int control = 0)
{
    switch (control)
    {
    case 1:
        viPrintf(vi, const_cast<ViString>(":CHANnel1:SCALe %s\r\n"),qPrintable(command));
        break;
    case 2:
        viPrintf(vi, const_cast<ViString>(":CHANnel2:SCALe %s\r\n"),qPrintable(command));
        break;
    default:
        viPrintf(vi, const_cast<ViString>(":CHANnel1:SCALe %s\r\n"),qPrintable(command));
        viPrintf(vi, const_cast<ViString>(":CHANnel2:SCALe %s\r\n"),qPrintable(command));
        break;
    }
}

void Osuilograf::setRANGe(QString command, int control = 0)
{
    switch (control)
    {
    case 1:
        viPrintf(vi, const_cast<ViString>(":CHANnel1:RANGe %s\r\n"),qPrintable(command));
        break;
    case 2:
        viPrintf(vi, const_cast<ViString>(":CHANnel2:RANGe %s\r\n"),qPrintable(command));
        break;
    default:
        viPrintf(vi, const_cast<ViString>(":CHANnel1:RANGe %s\r\n"),qPrintable(command));
        viPrintf(vi, const_cast<ViString>(":CHANnel2:RANGe %s\r\n"),qPrintable(command));
        break;
    }
}

void Osuilograf::setTIMebaseSCALe(QString command)
{
    viPrintf(vi, const_cast<ViString>(":TIMebase:SCALe %s\r\n"),qPrintable(command));
}

//{LEFT | CENTer | RIGHt | CUSTom}

void Osuilograf::setTIMebaseREFerence(QString command)
{
    viPrintf(vi, const_cast<ViString>(":TIMebase:REFerence  %s\r\n"),qPrintable(command));
}

void Osuilograf::setTRIGgerEDGESOURce(QString command)
{
    viPrintf(vi, const_cast<ViString>(":TRIGger:EDGE:SOURce  %s\r\n"),qPrintable(command));
}

//{POSitive | NEGative | EITHer | ALTernate}
void Osuilograf::setTRIGgerEDGESLOPe(QString command)
{
    viPrintf(vi, const_cast<ViString>(":TRIGger:EDGE:SLOPe  %s\r\n"),qPrintable(command));
}

void Osuilograf::setTRIGgerEDGELEVel(QString command)
{
    viPrintf(vi, const_cast<ViString>(":TRIGger:EDGE:LEVel  %s\r\n"),qPrintable(command));
}

void Osuilograf::setDelay(QString canal1 , QString canal2)
{

    viPrintf(vi, const_cast<ViString>(":MEASure:DELay  CHANnel%s,CHANnel%s\r\n"),qPrintable(canal1),qPrintable(canal2));

}

void Osuilograf::setAmplitude(QString canal)
{
    viPrintf(vi, const_cast<ViString>(":MEASure:VAMPlitude  CHANnel%s\r\n"),qPrintable(canal));


    //Считывание значение амплитуды.
//    char bufQuery [100] = "";
//    char buff [100] = "";

//    viQueryf(vi, ":MEASure:VAMPlitude? CHANnel1\n","%T",bufQuery);

//    qDebug () <<":MEASure:VAMPlitude  == " << qPrintable(bufQuery);

}

void Osuilograf::setVMAX(QString canal)
{
     viPrintf(vi, const_cast<ViString>(":MEASure:VMAX  CHANnel%s\r\n"),qPrintable(canal));
}

//NORM or AUTO
void Osuilograf::setTRIGgerSWEep(QString command)
{
   viPrintf(vi, const_cast<ViString>(":TRIGger:SWEep  %s\r\n"),qPrintable(command));
}


//насйтрока Осцилографа для измерения секундной метки
void Osuilograf::setStartSeitings()
{
    setTRIGgerSWEep("AUTO");
    setIMPedance("FIFTy");
    //setIMPedance("AUTO");

    setSCALe("2",1); // 2 V
    setSCALe("0.2",2); // 200mV
    setRANGe("16",1); // 2 V
    setRANGe("4",2); // 1V
    setTIMebaseSCALe("0.0000001"); // 1 микс
    setTIMebaseREFerence("CENTer");

    setTRIGgerEDGESOURce("CHANnel2");
    setTRIGgerEDGESLOPe("POSitive");
    setTRIGgerEDGELEVel("0.03"); // 30mV
    setDelay("1","2"); // Задержка между каналом 1 и каналом 2

    setAmplitude("0.2");

    setVMAX("1");

    setVMAX("2");
}


void Osuilograf::setStartSeitings2()
{
    if(flag_change_seitings == false)
    {
        setTRIGgerEDGESOURce("CHANnel1");
        setTRIGgerSWEep("NORM"); //NORM
        setTRIGgerEDGELEVel("0.2"); // 200mV
        setSCALe("1",1);
        setIMPedance("FIFTy");
    }
    else
    {
        setTRIGgerEDGESOURce("CHANnel2");
        setTRIGgerEDGELEVel("0.03"); //0.2
        setSCALe("0.2",2);
        //setIMPedance("AUTO");
        setTRIGgerSWEep("AUTO"); //NORM
        setIMPedance("FIFTy");//ONEMeg

    }
}


void Osuilograf::setStartSeitings3()
{
    if(flag_change_seitings_3 == false)
    {
        setTRIGgerEDGESOURce("CHANnel1");
        setTRIGgerEDGELEVel("0.2"); //6
        setIMPedance("ONEMeg");
        setTRIGgerSWEep("AUTO");
    }
    else
    {
        setTRIGgerEDGESOURce("CHANnel1");
        setTRIGgerEDGELEVel("0.5"); //0.5
        setSCALe("2",1);
        setSCALe("2",2);

        setTIMebaseSCALe("0.0000005"); // 1 микс
       // setTIMebaseSCALe("0.005"); // 50

        setTRIGgerSWEep("NORM");
        setIMPedance("FIFTy");
    }
}

QString Osuilograf::getDelay()
{
    char bufQuery [100];
    char buff [100];
    memset(bufQuery,0,sizeof(bufQuery));
    memset(buff,0,sizeof(buff));

    viQueryf(vi, const_cast<ViString>(":MEASure:DELay? CHANnel1,CHANnel2\n"),const_cast<ViString>("%T"),bufQuery);


    return qPrintable(bufQuery);

//    viQueryf(vi,"SYSTem:ERRor?\n","%T",buff);


//    qDebug () <<"Osuilograf delay = " <<  bufQuery;

//    if(QString(buff) == "0,\"No error\"\n")
//    {
//        emit signal_getCONTrol(qPrintable(bufQuery));

//        return qPrintable(bufQuery);
//    }
//    else
//    {
//        emit signal_getCONTrol(qPrintable(buff));

//        return qPrintable(buff);
//    }


}

QString Osuilograf::getAmplitude()
{

    //Считывание значение амплитуды.

    char bufQuery [100];
    char buff [100];
    memset(bufQuery,0,sizeof(bufQuery));
    memset(buff,0,sizeof(buff));

    viQueryf(vi, const_cast<ViString>(":MEASure:VAMPlitude? CHANnel1\n"),const_cast<ViString>("%T"),bufQuery);


   // viQueryf(vi,"SYSTem:ERRor?\n","%T",buff);


    qDebug () <<"Osuilograf VAMPlitude = " <<  bufQuery;

  //  if(QString(buff) == "+0,\"No error\"\n")
  //  {
   //     emit signal_getCONTrol(qPrintable(bufQuery));

        return qPrintable(bufQuery);
  //  }
  //  else
  //  {
  //      emit signal_getCONTrol(qPrintable(buff));

  //      return qPrintable(buff);
   // }

}

QString Osuilograf::getVMAX(QString canal)
{
    //Считывание значение амплитуды.

    char bufQuery [100];
    char buff [100];
    memset(bufQuery,0,sizeof(bufQuery));
    memset(buff,0,sizeof(buff));

    if(canal == "CHANnel2")
    {
        viQueryf(vi, const_cast<ViString>(":MEASure:VMAX? CHANnel2\n"),const_cast<ViString>("%T"),bufQuery);
    }
    else
    {
        viQueryf(vi, const_cast<ViString>(":MEASure:VMAX? CHANnel1\n"),const_cast<ViString>("%T"),bufQuery);
    }


   // viQueryf(vi,"SYSTem:ERRor?\n","%T",buff);


    qDebug () <<"Osuilograf VMAX = " <<  bufQuery;

    qDebug () <<"Osuilograf VMAX = " <<  qPrintable(bufQuery);

  //  if(QString(buff) == "+0,\"No error\"\n")
  //  {
   //     emit signal_getCONTrol(qPrintable(bufQuery));

        return qPrintable(bufQuery);
  //  }
  //  else
  //  {
  //      emit signal_getCONTrol(qPrintable(buff));

  //      return qPrintable(buff);
   // }
}

QString Osuilograf::getVRMS(QString canal)
{
    //Считывание значение амплитуды.

    char bufQuery [100];
    char buff [100];
    memset(bufQuery,0,sizeof(bufQuery));
    memset(buff,0,sizeof(buff));


    if(canal == "CHANnel2")
    {
        viQueryf(vi, const_cast<ViString>(":MEASure:VRMS? CHANnel2\n"),const_cast<ViString>("%T"),bufQuery);
    }
    else
    {
        viQueryf(vi, const_cast<ViString>(":MEASure:VRMS? CHANnel1\n"),const_cast<ViString>("%T"),bufQuery);
    }


   // viQueryf(vi,"SYSTem:ERRor?\n","%T",buff);


    qDebug () <<"Osuilograf VRMS = " <<  bufQuery;

    qDebug () <<"Osuilograf VRMS = " <<  qPrintable(bufQuery);

  //  if(QString(buff) == "+0,\"No error\"\n")
  //  {
   //     emit signal_getCONTrol(qPrintable(bufQuery));

        return qPrintable(bufQuery);
  //  }
  //  else
  //  {
  //      emit signal_getCONTrol(qPrintable(buff));

  //      return qPrintable(buff);
   // }
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
void Osuilograf::setCONTrol(QString command)
{
    viPrintf(vi, const_cast<ViString>("SOURce:SCENario:CONTrol %s\r\n"),qPrintable(command));

    // Check errors
    char buff[100] = " ";
    viQueryf(vi,const_cast<ViString>("SYSTem:ERRor?\n"),const_cast<ViString>("%T"),buff);

    qDebug () << buff;

    if(QString(buff) != "0,\"No error\"\n")
    {
        qDebug () <<"Error setCONTrol(QString command) : " << buff;
    }

}

void Osuilograf::setGenCONTrol(QString command)
{
    viPrintf(vi, const_cast<ViString>("SOUR:ONECHN:CONT %s\r\n"),qPrintable(command));

    // Check errors
    char buff[100] = " ";
    viQueryf(vi,const_cast<ViString>("SYSTem:ERRor?\n"),const_cast<ViString>("%T"),buff);

    qDebug () << buff;

    if(QString(buff) != "0,\"No error\"\n")
    {
        qDebug () <<"Error setCONTrol(QString command) : " << buff;
    }
}

//Parameter
//letter [G, R, E, C, J, I, S, , PG, PR, PE, PC, PJ, PI, UG, UR, UE, UC, UJ, UI]
//decimal [-7-210]
//Example: SOURce:ONECHN:SATid G11
void Osuilograf::setGenSATid(QString Np_liter)
{

    viPrintf(vi, const_cast<ViString>("SOUR:ONECHN:CONT STOP\r\n"));

    qDebug () << " Np_liter = " <<  Np_liter;

    viPrintf(vi, const_cast<ViString>("SOURce:ONECHN:SATid %s\r\n"),qPrintable(Np_liter));

    // Check errors
    char buff[100] = " ";
    viQueryf(vi,const_cast<ViString>("SYSTem:ERRor?\n"),const_cast<ViString>("%T"),buff);

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

void Osuilograf::setGenSIGNALtype(QString type_signal)
{
    viPrintf(vi, const_cast<ViString>("SOURce:ONECHN:SIGNALtype %s\r\n"),qPrintable(type_signal));  //GLOL1 and  GPSL1P

    // Check errors
    char buff[100] = " ";
    viQueryf(vi,const_cast<ViString>("SYSTem:ERRor?\n"),const_cast<ViString>("%T"),buff);

    qDebug () << buff;

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




QString Osuilograf::getCONTrol()
{
    char bufQuery [100] = "";
    char buff [100] = "";
    // char* bufQuery = new char();

    viQueryf(vi, const_cast<ViString>("SOURce:SCENario:CONTrol?\n"),const_cast<ViString>("%T"),bufQuery);

    // Check errors
    //  char* buff = new char();

    viQueryf(vi,const_cast<ViString>("SYSTem:ERRor?\n"),const_cast<ViString>("%T"),buff);


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
void Osuilograf::setSIGNALtype(QString SIGNALtype)
{

    viPrintf(vi, const_cast<ViString>("SOURce:ONECHN:SIGNALtype %T\r\n"),&SIGNALtype);

    // Check errors
    char* buff = new char();
    viQueryf(vi,const_cast<ViString>("SYSTem:ERRor?\n"),const_cast<ViString>("%T"),buff);

    if(QString(buff) != "0,\"No error\"\n")
    {
        qDebug () <<"Error setSIGNALtype(QString SIGNALtype) : " << buff;
    }
}

/*
   * Query 1-channel signal type in use. Signal type consists of comma-separated list of the simulated signals.
   *
   * */
QString Osuilograf::getSIGNALtype()
{
    char* bufQuery = new char();

    viQueryf(vi, const_cast<ViString>("SOURce:ONECHN:SIGNALtype?\n"),const_cast<ViString>("%T"),bufQuery);

    // Check errors
    char* buff = new char();

    viQueryf(vi,const_cast<ViString>("SYSTem:ERRor?\n"),const_cast<ViString>("%T"),buff);


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
QString Osuilograf::getSIGNALtype(QString id)
{
    char* bufSignal = new char();


    viQueryf(vi, const_cast<ViString>("SOUR:SCEN:SIGNAL? %s\r\n"),const_cast<ViString>("%T"),qPrintable(id),bufSignal);

    // Check errors
    char* buff = new char();

    viQueryf(vi,const_cast<ViString>("SYSTem:ERRor?\n"),const_cast<ViString>("%T"),buff);


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
   * 1-5 for Osuilograf-52/53,
   * 1-8 for Osuilograf-54,
   * 1-16 for Osuilograf-55/Osuilograf-56
   * 1-32/48/64 for Osuilograf-62/63/64.
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
QString Osuilograf::getSATid(int n)
{
    char* bufID = new char();

    viQueryf(vi, const_cast<ViString>("SOURce:SCENario:SATid%d?\n"),const_cast<ViString>("%T"),n,bufID);

    // Check errors
    char* buff = new char();

    viQueryf(vi,const_cast<ViString>("SYSTem:ERRor?\n"),const_cast<ViString>("%T"),buff);


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
   * The parameter n can be 1-8 for Osuilograf-54, 1-16 for Osuilograf-55/56 and 1-32/48/64 for Osuilograf-62/63/64.
   * The frequency is returned with a leading timestamp.
   * */
QString Osuilograf::getFREQuency(int n)
{
    char* bufID = new char();

    viQueryf(vi, const_cast<ViString>("SOURce:SCENario:FREQuency%d?\n"),const_cast<ViString>("%T"),n,bufID);

    // Check errors
    char* buff = new char();

    viQueryf(vi,const_cast<ViString>("SYSTem:ERRor?\n"),const_cast<ViString>("%T"),buff);


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
QString Osuilograf::getFREQuency(QString id)
{
    char* bufID = new char();

    viQueryf(vi, const_cast<ViString>("SOURce:SCENario:FREQuency? %s\n"),const_cast<ViString>("%T"),qPrintable(id),bufID);

    // Check errors
    char* buff = new char();

    viQueryf(vi,const_cast<ViString>("SYSTem:ERRor?\n"),const_cast<ViString>("%T"),buff);


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
   * The parameter n can be: 1-5 for Osuilograf-52/53 1-8 for Osuilograf-54 1-16 for Osuilograf-55/56 1-32/48/64 for Osuilograf-62/63/64.
   * The freqband parameter is optional and can be used when only a certain satellite frequency band power is changed.
   * The value ALL in freqband means that the power for all bands is adjusted by the amount indicated via the command.
   *
   * Decimal [-160.0,-65.0] dBm, if freqband is not ALL. With ALL, the limits are [-100,100] dBm. FreqBand [L1, L2, L5, ALL]
   * */
void Osuilograf::setPOWer(int id,int power,QString freqband)
{

    viPrintf(vi, const_cast<ViString>("SOURce:SCENario:POWer%d %d,%s\r\n"),id,power,qPrintable(freqband));

    // Check errors
    char* buff = new char();
    viQueryf(vi,const_cast<ViString>("SYSTem:ERRor?\n"),const_cast<ViString>("%T"),buff);

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
void Osuilograf::setPOWer(QString id,int power,QString freqband)
{

    viPrintf(vi, const_cast<ViString>("SOURce:SCENario:POWer %s,%d,%s\r\n"),qPrintable(id),power,qPrintable(freqband));

    // Check errors
    char* buff = new char();
    viQueryf(vi,const_cast<ViString>("SYSTem:ERRor?\n"),const_cast<ViString>("%T"),buff);

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
void Osuilograf::setPOWerALL(int power,QString freqband)
{

    viPrintf(vi, const_cast<ViString>("SOURce:SCENario:FREQBAND:POWer %d,%s\r\n"),power,qPrintable(freqband));

    // Check errors
    //  char* buff = new char();

    char buff[100] = " ";

    viQueryf(vi,const_cast<ViString>("SYSTem:ERRor?\n"),const_cast<ViString>("%T"),buff);

    if(QString(buff) != "0,\"No error\"\n")
    {
        qDebug () <<"Error setPOWerALL(int power,QString freqband) : " << buff;
    }
}

/*
   *
   * Query the current power setting of channel n during scenario execution.
   * The parameter n can be: 1-5 for Osuilograf-52/53 1-8 for Osuilograf-54 1-16 for Osuilograf-55/56 1-32/48/64 for Osuilograf-62/63/64.
   * The power is returned with a leading timestamp. Freqband is an optional parameter used to specify for which frequency band the power is returned.
   * If the freqband parameter is omitted, the L1 power is returned.
   * */
QString Osuilograf::getPOWer(int id,QString freqband)
{

    char* bufID = new char();

    viQueryf(vi, const_cast<ViString>("SOURce:SCENario:POWer%d? %s\n"),const_cast<ViString>("%T"),id,qPrintable(freqband),bufID);

    // Check errors
    char* buff = new char();

    viQueryf(vi,const_cast<ViString>("SYSTem:ERRor?\n"),const_cast<ViString>("%T"),buff);


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
QString Osuilograf::getPOWer(QString id)
{

    char bufID[100] = "";

    viQueryf(vi, const_cast<ViString>("SOURce:SCENario:POWer? %s\n"),const_cast<ViString>("%T"),qPrintable(id),bufID);

    // Check errors
    char buff[100]="";

    viQueryf(vi,const_cast<ViString>("SYSTem:ERRor?\n"),const_cast<ViString>("%T"),buff);


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
   * The parameter n can be: 1-5 for Osuilograf-52/53 1-8 for Osuilograf-54 1-16 for Osuilograf-55/Osuilograf-56 1-32 for Osuilograf-62.
   * The Space Vehicle model can be: Block II, Block IIA, Block IIR, Block IIR-M, Block IIF or Block IIIA for GPS Glonass-M or Glonass-K1 for GLONASS
   * */
QString Osuilograf::getSVmodel(int id)
{
    char* bufID = new char();

    viQueryf(vi, const_cast<ViString>("SOURce:SCENario:SVmodel%d? %s\n"),const_cast<ViString>("%T"),id,bufID);

    // Check errors
    char* buff = new char();

    viQueryf(vi,const_cast<ViString>("SYSTem:ERRor?\n"),const_cast<ViString>("%T"),buff);


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
QString Osuilograf::getSVmodel(QString id)
{
    char* bufID = new char();

    viQueryf(vi, const_cast<ViString>("SOURce:SCENario:SVmodel? %s %s\n"),const_cast<ViString>("%T"),qPrintable(id),bufID);

    // Check errors
    char* buff = new char();

    viQueryf(vi,const_cast<ViString>("SYSTem:ERRor?\n"),const_cast<ViString>("%T"),buff);


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


QString Osuilograf::getPosition()
{
    char bufID[100];

    // viPrintf(vi, "*WAI\r\n");

    viQueryf(vi, const_cast<ViString>("SOURce:SCENario:LOG?\n"),const_cast<ViString>("%T"),bufID);


    // Check errors
    char buff[100];

    viQueryf(vi,const_cast<ViString>("SYSTem:ERRor?\n"),const_cast<ViString>("%T"),buff);


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






void Osuilograf::getScenDataTime()
{
    char bufDataTime[100] = "";
    char bufDataTimeError[100] = "";

    viQueryf(vi, const_cast<ViString>("SOURce:SCENario:DATEtime? UTC\n"),const_cast<ViString>("%T"),&bufDataTime);


    // Check errors
    viQueryf(vi,const_cast<ViString>("SYSTem:ERRor?\n"),const_cast<ViString>("%T"),&bufDataTimeError);

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


QString Osuilograf::getNumberIDSpytnik()
{
    char bufID[100]= "";


    viQueryf(vi, const_cast<ViString>("SOURce:SCENario:SVINview?\n"),const_cast<ViString>("%T"),bufID);


    // Check errors
    char buff[100]= "";

    viQueryf(vi,const_cast<ViString>("SYSTem:ERRor?\n"),const_cast<ViString>("%T"),buff);



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

