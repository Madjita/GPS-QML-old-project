#include "algorithm.h"

Algorithm::Algorithm(QObject *parent) :
    QObject(parent),
    autoProverka(false),
    kol_Auto_Start(0),
    kol_cignalov(0),
    autoMode_NY_cold_hart(0)

{
    this->moveToThread(new QThread());
    connect(this->thread(),&QThread::started,this,&Algorithm::process_start);
    connect(this,&Algorithm::signal_finished, this->thread(), &QThread::quit);
    this->thread()->start();

}

Algorithm::~Algorithm()
{

    emit signal_finished();
}

void Algorithm::setDevice(CamberModel* _camber,gsgModel* _gsg,N6700Model* _n6700_1,N6700Model* _n6700_2,OsuilografModel* _os,tp8Model* _tp,PortModel* _port1,PortModel* _port2,PortModel* _port3,PortModel* _port4,PortModel* _port5,PortModel* _port6,PortModel* _port7,PortModel* _port8)
{
    camber = _camber;
    gsg = _gsg;
    n6700_1 = _n6700_1;
    n6700_2 = _n6700_2;
    tp = _tp;
    os = _os;

    portList.clear();
    portList.append(_port1);
    portList.append(_port2);
    portList.append(_port3);
    portList.append(_port4);
    portList.append(_port5);
    portList.append(_port6);
    portList.append(_port7);
    portList.append(_port8);

}

void Algorithm::slot_StartProverka()
{
    qDebug() << "START Work  autoProverka = " << autoProverka;
    qDebug() << "START Work  namberProverkaStart = " << namberProverkaStart << " namberModeCamber = " << namberModeCamber;
    qDebug() << "START Work sem.available() = " <<  sem.available();



     QString z = "SELECT * FROM Etap Where [Etap] = '"+ m_modeStart+"'";

     auto sql =  BD->zaprosQueryModel(z);

     //Сохраняем ID Etap
     idEtap = sql->data(sql->index(0,0), Qt::EditRole).toString();

     qDebug() << z << " idEtap : " << idEtap;



     z =  "SELECT * FROM User WHERE [FIO] = '"+m_fio+"'";
     sql =  BD->zaprosQueryModel(z);

     //Сохраняем ID FIO
     idUser = sql->data(sql->index(0,0), Qt::EditRole).toString();

     qDebug() << z << " idUser : " << idUser;

     if(idUser.count() == 0)
     {
        z = "INSERT INTO User (FIO) VALUES('"+m_fio+"')";
        sql =  BD->zaprosQueryModel(z);

        z =  "SELECT * FROM User WHERE [FIO] = '"+m_fio+"'";
        sql =  BD->zaprosQueryModel(z);

        //Сохраняем ID FIO
        idUser = sql->data(sql->index(0,0), Qt::EditRole).toString();

        qDebug() << z << " idUser : " << idUser;
     }











    //ZZZ
//    if(kol_Auto_Start == 0)
//    {

//        for(int i=0;i < m_ListProverkaItem->count();i++)
//        {
//            delete  m_ListProverkaItem->value(i);
//        }

//        emit qml_text("Ждем сброса ТПНП-101...");

//        tp->slot_clear();
//        sem.acquire();

//        emit qml_text("Определение количества приемников...");
//        //инициализация проверок
//        initializate_proverka();
//        qDebug() << "END initializate_proverka ";
//    }
//    else
//    {
//        emit qml_text("Ждем сброса ТПНП-101...");
//        tp->slot_clear();
//        sem.acquire();
//    }

//    emit setPPS(1);
//    emit qml_text("Загрузка имитатора на имитацию сигнала...");

//    if(namberProverkaStart != 5)
//    {
//        emit setSIGNALtype("START");
//    }

//    emit signal_StartGSG();
//    qDebug() << "Wait GSG start";
//    //Ждем загрузки имитатора
//    sem.acquire();
//    qDebug() << "Wait GSG start-GOOD";

    emit qml_text("Имитатор запущен...");




    if(m_modeStart == "После технической тренировки в НУ" || m_modeStart == "Проверка неприрывной работы")
    {
        flagWork_Dont_PSIandRegylirovka = true; //завершение на автоматизированной проверки НУ
    }
    else
    {
        flagWork_Dont_PSIandRegylirovka = false; //завершение на автоматизированной проверки НУ
    }


    if(autoProverka == true)
    {

        //Установили НУ, потом установим Холод, после установим тепло.
        switch (kol_Auto_Start)
        {
        case 0: autoMode_NY_cold_hart = 1;camber->slot_SetNY();qDebug()<< "Set NY";  break; //Устанавливаем НУ
        case 1: autoMode_NY_cold_hart = 2;camber->slot_SetCold();qDebug()<< "Set -40";  break; //Устанавливаем -40
        case 2: autoMode_NY_cold_hart = 3;camber->slot_SetNY();qDebug()<< "Set NY before -40";  break; //Устанавливаем НУ после -40
        case 3: autoMode_NY_cold_hart = 4;camber->slot_SetHord();qDebug()<< "Set +55";  break; //Устанавливаем +55
        case 4: autoMode_NY_cold_hart = 5;camber->slot_SetNY();qDebug()<< "Set NY before +55";  break; //Устанавливаем НУ после +55
        case 5: {
            // autoMode_NY_cold_hart = 1;kol_Auto_Start =0; //Устанавливаем НУ
            qDebug()<< "Set Default";

            for(int i=0;i < listProverkaItem.count();i++)
            {
                listProverkaItem[i]->relizProverka->flag_auto = false;
            }
        }

        }

        //подсчёт количества стартов
        kol_Auto_Start++;

        namberProverkaStart = 1;
        namberModeCamber = autoMode_NY_cold_hart;
        camber->slot_startCamberWork();  //Затем запустить камеру.

    }
    else
    {
        //открыть окно для выбора проверки для одиночного режима

        for(int i=0;i < listProverkaItem.count();i++)
        {
            listProverkaItem[i]->relizProverka->flag_auto = autoProverka;
        }

        slot_StartOneProverka(namberProverkaStart,namberModeCamber);
    }

    //Ждем настройки камеры и выдержка вермени в камере.
    sem.acquire();

    QSqlQueryModel* SQL_Etap;

    QDateTime start;
    start = QDateTime::currentDateTime();

    //записываем дату старта проверки
    BD->zaprosQueryModel("INSERT INTO DateStartEnd (DateStart) VALUES('"+start.toString("dd.MM.yyyy  hh:mm:ss")+"')");

    SQL_Etap =  BD->zaprosQueryModel("SELECT DateStart,Id FROM DateStartEnd WHERE DateStart LIKE '"+start.toString("dd.MM.yyyy  hh:mm:ss")+"'");

    //Сохраняем ID начала заупска проверки
    idData = SQL_Etap->data(SQL_Etap->index(0,1), Qt::EditRole).toString();

    idLink.clear();


    for(int i=0;i< listProverkaItem.count();i++)
    {
        BD->zaprosQueryModel("INSERT INTO Link_Result (IdUser,IdEtap,IdDate,IdSerial,IdTemp) VALUES('"+idUser+"','"+idEtap+"','"+idData+"','"+idSerial.value(i)+"','"+QString::number(autoMode_NY_cold_hart)+"')");

        SQL_Etap = BD->zaprosQueryModel("SELECT Id FROM Link_Result WHERE IdDate LIKE '"+idData+"' AND IdSerial LIKE '"+idSerial.value(i)+"' AND IdUser LIKE '"+idUser+"' AND IdEtap LIKE '"+idEtap+"' AND IdTemp LIKE '"+QString::number(autoMode_NY_cold_hart)+"'");
        //Записываем ID для результатов именно этого юсера этапа времени и прибора
        idLink.append(SQL_Etap->data(SQL_Etap->index(0,0), Qt::EditRole).toString());


        listProverkaItem[i]->relizProverka->SetIdLink(idLink.last());
        listProverkaItem[i]->relizProverka->start = start;
        listProverkaItem[i]->relizProverka->autoMode_NY_cold_hart = autoMode_NY_cold_hart;


    }


  //  emit signal_clearView();

//ZZZ
   // emit startTimer();

  //  emit signal_StartProverka(autoProverka, namberProverkaStart);


    //удалить после првоерки камеры
    if(autoProverka == false)
    {
        emit signal_Finish();
        return;
    }
    else
    {

        qDebug() << "LOL 3 signal_startColdHard = " << autoMode_NY_cold_hart;

        emit signal_clearView();

        //autoMode_NY_cold_hart = 1; //временно убрать когда появится камера

        if(flagWork_Dont_PSIandRegylirovka)
        {
            autoMode_NY_cold_hart = 5;
        }

        switch (autoMode_NY_cold_hart)
        {
            //запустить камеру для НУ
            case 0: emit signal_NextCamberTemprature();break; //Устанавливаем НУ

                //запустить камеру для охлаждения
            case 1:  emit signal_NextCamberTemprature();break; //Устанавливаем -40
                //запустить камеру для НУ
            case 2: emit signal_NextCamberTemprature();break; //Устанавливаем НУ после -40
                //запустить камеру для нагрева
            case 3: emit signal_NextCamberTemprature(); break; //Устанавливаем +55
                //запустить камеру для НУ
            case 4: emit signal_NextCamberTemprature(); break; //Устанавливаем НУ после +50

            case 5:
            {
                emit signal_Finish();

                emit signal_stopCamberWorkProverka();
               // camber->slot_stopCamberWork();

                autoMode_NY_cold_hart = 1;
                kol_Auto_Start =0; //Устанавливаем НУ

                qDebug() << "!!!!!!!!!!!!!!!!!!!!!!!! END !!!!!!!!!!!!!!!!!!!!!!!!";


            }
        }

    }


}

void Algorithm::slot_StartOneProverka(int indexProverka, int modeCamber)
{
    namberProverkaStart = indexProverka;
    namberModeCamber = modeCamber;

    switch (modeCamber) {
        case 2:
        {
            camber->slot_SetCold();
            break;
        }
        case 4:
        {
            camber->slot_SetHord();
            break;
        }
        default:
        {
            camber->slot_SetNY();
            break;
        }
    }

    camber->slot_startCamberWork();

    //ZZZ
  //  camber->slot_StartTimer();  //если есть камера то убрать

}

void Algorithm::get_beginOSProv(int index)
{
    kol_cignalov++;

    if(kol_cignalov != Count_NP)
    {
        qDebug () << "Proverka 6 -> kol_cignalov != Count_NP : " << kol_cignalov << " index :" << index;
        return;
    }

    kol_cignalov = 0;

    emit setSIGNALtype("STOP");
    emit setSIGNALtype("START");

    emit signal_StartGSG();

    qDebug() << "Wait GSG start Proverka 6";
    //Ждем загрузки имитатора
    sem.acquire();
    qDebug() << "Wait GSG start-GOOD Proverka 6";


    //запустили 1 проверку
    listProverkaItem[0]->relizProverka->sem->release();


}

void Algorithm::get_beginOSProv_7(int index)
{
    kol_cignalov++;

    if(kol_cignalov != Count_NP)
    {
        qDebug () << "Proverka 7,8,9,10 -> kol_cignalov != Count_NP : " << kol_cignalov << "\t index:" <<index;
        return;
    }

    qDebug () << "Good Proverka 7,8,9,10 -> kol_cignalov == Count_NP : " << kol_cignalov<< "\t index:" <<index;

    kol_cignalov = 0;

    listProverkaItem[0]->tp->tp->index = listProverkaItem[0]->relizProverka->index;



    qDebug() << "tp->tp->index = " << listProverkaItem[0]->tp->tp->index;
    //запустили 1 проверку
    listProverkaItem[0]->relizProverka->sem->release();

    qDebug() << "tp->tp->index = " << listProverkaItem[0]->tp->tp->index;
}

void Algorithm::slot_comand_ok(int comand, int index)
{
    qDebug() << "TP8 send comand = "<< comand << " is Good on mrk : " << index;

    // listProverkaItem[index-1]->relizProverka->sem->release();


    for(int i=0;i < listProverkaItem.count();i++)
    {
        if(listProverkaItem[i]->relizProverka->index == index)
        {
            qDebug() << "listProverkaItem["<<index<<"]->relizProverka->sem->release(); = " << listProverkaItem[i]->relizProverka->sem->available();
            listProverkaItem[i]->relizProverka->sem->release();
             qDebug() << "listProverkaItem["<<index<<"]->relizProverka->sem->release(); = " << listProverkaItem[i]->relizProverka->sem->available();
            return;
        }
    }

//    foreach (proverkaItem* item, listProverkaItem)
//    {
//        if(item->relizProverka->index == index)
//        {
//            item->relizProverka->sem->release();
//            return;
//        }
//    }
}

void Algorithm::signal_slot_StartProverka_Os(int index, int position)
{
    qDebug() << "signal_slot_StartProverka_Os " << index;

    if(listProverkaItem.indexOf(listProverkaItem[position]) != (listProverkaItem.count()-1))
        listProverkaItem[position+1]->relizProverka->sem->release();
    else
        qDebug() << "END signal_slot_StartProverka_Os END " << index;

}

void Algorithm::slot_start_GSG()
{


}


void Algorithm::process_start()
{
    //Брать данные из GUI
    idUser = "2";
    idEtap = "1";
    m_modeStart = "Регулировка";

    BD = new BData();
    m_ListProverkaItem = new QList<QObject*>();

    connect(this,&Algorithm::sendZapros,BD,&BData::getZapros);
    connect(this,&Algorithm::getZapros,BD,&BData::zaprosQueryModel);


    BD->connectBD();
}

void Algorithm::initializate_proverka()
{
    qDebug() <<"Start initializate_proverka";



    QStringList list;
    QVector<QString> listFind_I;

    // нужно запустить поиск количества приемников.

    if(n6700_1 != nullptr)
    {

        if(n6700_1->getConnect() == true)
        {
            if(n6700_1->n6700->timer_IstP1_Measure->isActive())
            {
                n6700_1->slot_StopTimer();
            }

            n6700_1->n6700->setVolt("ALL","5");
            n6700_1->n6700->setOutput("ALL",true);
            this->thread()->sleep(2);

            listFind_I = n6700_1->n6700->getMeasureCURRentALL_find();

            for(int i=0 ; i < listFind_I.count();i++)
            {
                qDebug () << listFind_I.value(i).toDouble();

                if(listFind_I.value(i).toDouble() > 0.01)
                {
                    list.append(QString::number(i+1));
                }
            }
        }
    }

    if(n6700_2 != nullptr)
    {

        if(n6700_2->n6700->connected == true)
        {

            if(n6700_2->n6700->timer_IstP1_Measure->isActive())
            {
                n6700_2->slot_StopTimer();
            }

            n6700_2->n6700->setVolt("ALL","5");
            n6700_2->n6700->setOutput("ALL",true);
            this->thread()->sleep(2);

            listFind_I =  n6700_2->n6700->getMeasureCURRentALL_find();


            for(int i=0 ; i < listFind_I.count();i++)
            {
                if(listFind_I.value(i).toDouble() > 0.1)
                {
                    list.append(QString::number(i+5));
                }
            }
        }
    }

    if(list.count() == 0)
    {
        QMessageBox::warning(nullptr,"Ошибка","Приемники находящиеся в пульте не потребляют питание.");
        emit signal_Finish();
    }

    qDebug () << list;

    list_CountNP = list;

    for (int i=0;i < list.count();i++)
    {
        int mrkPosition = list.value(i).toInt()-1;

        portList[mrkPosition]->PortNew->flag_waitloadingMRK = true;
        portList[mrkPosition]->PortNew->flag_end_MRK = false;
        portList[mrkPosition]->slot_Work();
    }

    qDebug() << "Wait Start MRK";
    //Ждем загрузки приемников
    sem.acquire(list.count());

    for (int i=0;i < list.count();i++)
    {
        int mrkPosition = list.value(i).toInt()-1;
        portList[mrkPosition]->PortNew->flag_getName = true;
        portList[mrkPosition]->PortNew->GetMrk_Name();
    }

    qDebug() << "Get Name MRK";
    //Ждем получения Имени приемников
    sem.acquire(list.count());

    if((n6700_1 != nullptr))
    {
        if(n6700_1->n6700->connected == true)
        {
            n6700_1->n6700->setOutput("ALL",false);
        }
    }

    if((n6700_2 != nullptr))
    {
        if(n6700_2->n6700->connected == true)
        {
            n6700_2->n6700->setOutput("ALL",false);
        }
    }

    if(m_ListProverkaItem->count() > 0)
    {
        m_ListProverkaItem->clear();
    }

    listNameMRK.clear();

    for (int i=0;i < list.count();i++)
    {
        int mrkPosition = list.value(i).toInt()-1;
        listNameMRK.append(portList[mrkPosition]->PortNew->nameMRK);

        auto nameMRK = QString(portList[mrkPosition]->PortNew->nameMRK);

        if(nameMRK == "")
        {
            portList[mrkPosition]->PortNew->nameMRK.clear();
            portList[mrkPosition]->PortNew->nameMRK.append(list.value(i));
        }
    }

    qDebug() << listNameMRK;

    //Поиск id имен МРК в базе данных
    find_NameMRK_On_BD();


    /////////////////////////////////////////////////////////////////////

    listProverkaItem.clear();



    Count_NP = list_CountNP.count(); // количество найденных приемников в пульте ТП-НП101



    for(int i=0; i < list_CountNP.count();i++)
    {
        int mrkPosition = list_CountNP.value(i).toInt()-1;
        proverkaItem* proverka = nullptr;

        if(mrkPosition+1 <= 4)
        {
            proverka = new proverkaItem(mrkPosition+1,listProverkaItem.count(),n6700_1,gsg,os,tp,portList[mrkPosition],false, false,listNameMRK.value(i));
        }
        else
        {
            proverka = new proverkaItem(mrkPosition+1,listProverkaItem.count(),n6700_2,gsg,os,tp,portList[mrkPosition],false, false,listNameMRK.value(i));
        }

        proverka->moveToThread(mainThread);

        //Присвоение ID найденных устройств в БДы
        proverka->relizProverka->Id = idSerial.value(i);

        m_ListProverkaItem->append(proverka);
        listProverkaItem.append(proverka);


        //////connect
        connect(this,&Algorithm::signal_StartProverka,listProverkaItem[i]->relizProverka,&RelizProverka::Work);

        connect(listProverkaItem[i]->relizProverka,&RelizProverka::sendZaprozToBD,this,&Algorithm::get_zaprozBD);
        connect(listProverkaItem[i]->relizProverka,&RelizProverka::signal_EndProverka,this,&Algorithm::slot_endProverka);
        connect(listProverkaItem[i]->relizProverka,&RelizProverka::signal_startGen,this,&Algorithm::slot_start_GEN);

        connect(this,&Algorithm::signal_StartProverka_5,listProverkaItem[i]->relizProverkaTimer,&relizproverkaTimer::slot_EndBlock_5_proverka);

        connect(listProverkaItem[i]->relizProverka->port,&PortModel::mrkLoadProverkaChanged,listProverkaItem[i]->relizProverkaTimer,&relizproverkaTimer::slot_EndBlock2);


        connect(listProverkaItem[i]->relizProverka,&RelizProverka::signal_startImitator,this,&Algorithm::slot_start_GSG);


        //подключение сигналов для 6 проверки для последовательного включения
        connect(listProverkaItem[i]->relizProverka,&RelizProverka::signal_beginOSProv,this,&Algorithm::get_beginOSProv);

        connect(listProverkaItem[i]->relizProverka,&RelizProverka::signal_beginOSProv_7,this,&Algorithm::get_beginOSProv_7);

        connect(listProverkaItem[i]->relizProverka,&RelizProverka::signal_slot_StartProverka_Os,this,&Algorithm::signal_slot_StartProverka_Os);

    }



    qDebug() << "listProverkaItem =  " << listProverkaItem.count();


    emit listProverkaItemChanged(m_ListProverkaItem);


}

void Algorithm::find_NameMRK_On_BD()
{
    QSqlQueryModel* SQL_serial = nullptr;

    idSerial.clear();

    for(int i=0;i < list_CountNP.count();i++)
    {
        auto nameMRK = QString(listNameMRK[i]);

        if(nameMRK != "")
        {
            SQL_serial = BD->zaprosQueryModel("SELECT *"
                                              " FROM Serial"
                                              " WHERE Serial LIKE '"+QString(listNameMRK[i].toHex())+"'");

            qDebug() << SQL_serial->rowCount();

            if(SQL_serial->rowCount() > 0)
            {
                auto id = SQL_serial->data(SQL_serial->index(0,0), Qt::EditRole).toString();
                idSerial.append(id);
            }
            else
            {
                BD->zaprosQueryModel("INSERT INTO Serial (Serial) VALUES('"+QString(listNameMRK[i].toHex())+"')");

                SQL_serial = BD->zaprosQueryModel("SELECT *"
                                                  " FROM Serial"
                                                  " WHERE Serial LIKE '"+QString(listNameMRK[i].toHex())+"'");

                auto id = SQL_serial->data(SQL_serial->index(0,0), Qt::EditRole).toString();
                idSerial.append(id);
            }
        }
        else
        {
            SQL_serial = BD->zaprosQueryModel("SELECT * FROM Serial WHERE Serial LIKE 'ЛЖ000"+list_CountNP.value(i)+"'");


            auto id = SQL_serial->data(SQL_serial->index(0,0), Qt::EditRole).toString();
            idSerial.append(id);
        }
    }

    //    BD->DiscConnect();

    //    delete BD;

    //    BD = nullptr;

    qDebug() <<"id_Serial = " <<  idSerial;


}

void Algorithm::get_zaprozBD(QString data)
{

    emit sendZapros(data);

    // BD->zaprosQueryModel("INSERT INTO Result (Sootv) VALUES('1')");

    // BD->zaprosQueryModel(data);
}

//Слот для запуска имитатора генератора для 5 првоерки.
void Algorithm::slot_start_GEN()
{
    kol_cignalov++;

    if(kol_cignalov != Count_NP)
    {
        qDebug () << "Proverka 5 -> kol_cignalov != Count_NP : " << kol_cignalov;

        return;
    }

    kolStart = 0;

    qDebug () << "Start Proverka 5";

    for(int i=0;i <Count_NP;i++)
    {

        if(listProverkaItem[i]->relizProverka->liter  <= 36)
        {
            listProverkaItem[i]->relizProverka->liter  = 26;
        }
        else
        {
            listProverkaItem[i]->relizProverka->liter  = 150;//26; //130
        }

        listProverkaItem[i]->relizProverka->liter -= 1;

        qDebug () << "listProverkaItem["<<i<<"]->relizProverka->liter = " << listProverkaItem[i]->relizProverka->liter;

        Set_Number_Spytnik(listProverkaItem[i]);

    }

    emit signal_Gen_setCONTrol("START");

    qDebug() << "Wait GSG liter start";
    //Ждем загрузки имитатора
    sem.acquire();
    qDebug() << "Wait GSG liter start-GOOD";

    kol_cignalov = 0;
    start = 0;

    emit signal_StartProverka_5();

}

void Algorithm::waitTpQML()
{
    for(int i=0;i <Count_NP;i++)
    {
        listProverkaItem[i]->relizProverkaTimer->signal_Tick("Ждем сброса ТПНП-101...");
    }
}

void Algorithm::slot_endProverka(int index,int position)
{
    qDebug () << "slot_EndProverka = " << index << " : index " << position;

    listProverkaItem[position]->relizProverka->port->PortNew->CountFindGPS=0;
    listProverkaItem[position]->relizProverka->port->PortNew->CountFindGLONASS=0;
    listProverkaItem[position]->relizProverka->port->PortNew->CountFindALL=0;

    start++;

    qDebug() << "!!!!!!!!!start : " << start;

    switch (index)
    {
    case 1:
        if(listProverkaItem[position]->relizProverka->job == false)
        {

            if(listProverkaItem[position]->relizProverka->flag_good_1)
            {
                qDebug ()<<"Proverka 1 is GOOD : index =\t" << listProverkaItem[position]->index;
            }
            else
            {
                qDebug ()<<"Proverka 1 is BEAD : index =\t" << listProverkaItem[position]->index;
            }

            emit qml_good(listProverkaItem[position]->relizProverka->flag_good_1,QString::number(index),QString::number(listProverkaItem[position]->index));
        }

        if(start == Count_NP)
        {

            qDebug ()<<"ALL Proverka 1 is FINISH : index =\t" << listProverkaItem[position]->index<<" :   Camber \t" <<  autoMode_NY_cold_hart;
            start=0;


            if(autoProverka == false)
            {

                endOneProverka(index);

                for(int i=0;i <Count_NP;i++)
                {
                    listProverkaItem[i]->relizProverka->job  = true;
                    listProverkaItem[i]->relizProverka->sem->release();
                }

                emit signal_Finish();

                return;
            }
            else
            {
                waitTpQML();
                tp->slot_clear();
                sem.acquire(); //ждем пока пульт не сбросит
                kol_cignalov = 0;

                for(int i=0;i <Count_NP;i++)
                {
                    listProverkaItem[i]->relizProverka->job  = true;
                    listProverkaItem[i]->relizProverka->sem->release();
                }

            }

        }
        else
        {
            //  emit signal_BadQML(proverka1->relizProverka->countProverka);
        }

        break;
    case 2:

        if(listProverkaItem[position]->relizProverka->job == false)
        {

            if(listProverkaItem[position]->relizProverka->flag_good_2)
            {
                qDebug ()<<"Proverka 2 is GOOD : index =\t" << listProverkaItem[position]->index;

            }
            else
            {
                qDebug ()<<"Proverka 2 is BEAD : index =\t" << listProverkaItem[position]->index;
            }

               emit qml_good(listProverkaItem[position]->relizProverka->flag_good_2,QString::number(index),QString::number(listProverkaItem[position]->index));
        }

        if(start == Count_NP)
        {

            qDebug ()<<"ALL Proverka 2 is FINISH :   Camber \t" <<  autoMode_NY_cold_hart;
            start=0;

            if(autoProverka == false)
            {

                emit endOneProverka(index);

                for(int i=0;i <Count_NP;i++)
                {
                    listProverkaItem[i]->relizProverka->job  = true;
                    listProverkaItem[i]->relizProverka->sem->release();
                }

                emit signal_Finish();

                return;
            }
            else
            {
                waitTpQML();
                tp->slot_clear();
                sem.acquire(); //ждем пока пульт не сбросит
                kol_cignalov = 0;

                for(int i=0;i <Count_NP;i++)
                {
                    listProverkaItem[i]->relizProverka->job  = true;
                    listProverkaItem[i]->relizProverka->sem->release();
                }

            }
        }

        break;
    case 3:

        if(listProverkaItem[position]->relizProverka->job == false)
        {

            if(listProverkaItem[position]->relizProverka->flag_good_3)
            {
                qDebug ()<<"Proverka 3 is GOOD : index =\t" << listProverkaItem[position]->index;

            }
            else
            {
                qDebug ()<<"Proverka 3 is BEAD : index =\t" << listProverkaItem[position]->index;
            }

               emit qml_good(listProverkaItem[position]->relizProverka->flag_good_3,QString::number(index),QString::number(listProverkaItem[position]->index));
        }

        if(start == Count_NP)
        {

            qDebug ()<<"ALL Proverka 3 is FINISH :   Camber \t" <<  autoMode_NY_cold_hart;
            start = 0;

            if(autoProverka == false)
            {

                emit endOneProverka(index);

                for(int i=0;i <Count_NP;i++)
                {
                    listProverkaItem[i]->relizProverka->job  = true;
                    listProverkaItem[i]->relizProverka->sem->release();
                }

                emit signal_Finish();

                return;
            }
            else
            {
                waitTpQML();
                tp->slot_clear();
                sem.acquire(); //ждем пока пульт не сбросит
                kol_cignalov = 0;

                for(int i=0;i <Count_NP;i++)
                {
                    listProverkaItem[i]->relizProverka->job  = true;
                    listProverkaItem[i]->relizProverka->sem->release();
                }

            }
        }

        break;
    case 4:

        if(listProverkaItem[position]->relizProverka->job == false)
        {

            if(listProverkaItem[position]->relizProverka->flag_good_4)
            {
                qDebug ()<<"Proverka 4 is GOOD : index =\t" << listProverkaItem[position]->index;

            }
            else
            {
                qDebug ()<<"Proverka 4 is BEAD : index =\t" << listProverkaItem[position]->index;
            }

               emit qml_good(listProverkaItem[position]->relizProverka->flag_good_4,QString::number(index),QString::number(listProverkaItem[position]->index));
        }


        if(start == Count_NP)
        {

            qDebug ()<<"ALL Proverka 4 is FINISH :   Camber \t" <<  autoMode_NY_cold_hart;
            start = 0;

            if(autoProverka == false)
            {
                emit endOneProverka(index);

                for(int i=0;i <Count_NP;i++)
                {
                    listProverkaItem[i]->relizProverka->job  = true;
                    listProverkaItem[i]->relizProverka->sem->release();
                }

                emit signal_Finish();

                return;
            }
            else
            {
                tp->slot_clear();
                sem.acquire(); //ждем пока пульт не сбросит
                kol_cignalov = 0;

                for(int i=0;i <Count_NP;i++)
                {
                    listProverkaItem[i]->relizProverka->job  = true;
                    listProverkaItem[i]->relizProverka->sem->release();
                }

                // emit signal_StartProverka_5();
            }
        }

        break;
    case 5:

        if(listProverkaItem[position]->relizProverka->job == false)
        {

            if(listProverkaItem[position]->relizProverka->list_flag_good_5.value(listProverkaItem[position]->relizProverka->proverka5_part-1))
            {
                qDebug ()<<"Proverka 5 liter is: "<< listProverkaItem[position]->relizProverka->liter<<" is GOOD";

            }
            else
            {
                qDebug ()<<"Proverka 5 liter is: "<< listProverkaItem[position]->relizProverka->liter<<" is BEAD";

            }

               emit qml_good(listProverkaItem[position]->relizProverka->list_flag_good_5.value(listProverkaItem[position]->relizProverka->proverka5_part-1),QString::number(index),QString::number(listProverkaItem[position]->index));

        }

        if(start == Count_NP)
        {

            qDebug ()<<"ALL Proverka 5 is FINISH :   Camber \t" <<  autoMode_NY_cold_hart;
            start = 0;

            emit setSIGNALtype("STOP");

            kolStart = 0;

            for(int i=0;i <Count_NP;i++)
            {

                listProverkaItem[i]->relizProverka->job = true;

                if(listProverkaItem[i]->relizProverka->proverka5_part ==36)
                {


                    if(autoProverka == false)
                    {
                        emit endOneProverka(index);

                        for(int i=0;i <Count_NP;i++)
                        {
                            listProverkaItem[i]->relizProverka->job  = true;
                            listProverkaItem[i]->relizProverka->sem->release();
                        }

                        emit signal_Finish();

                        return;
                    }
                    else
                    {
                        waitTpQML();

                        qDebug() <<"tp->slot_clear() sem.available() = "<< sem.available();
                        tp->slot_clear();
                        sem.acquire();
                        qDebug() <<"tp->slot_clear() sem.available() = "<< sem.available();


                        kol_cignalov = 0;
                        kolStart = 0;

                        for(int j=0;j <Count_NP;j++)
                        {
                            qDebug() <<"listProverkaItem[j]->relizProverka->sem->  sem.available() = "<< listProverkaItem[j]->relizProverka->sem->available();

                            listProverkaItem[j]->relizProverka->job  = true;
                            listProverkaItem[j]->relizProverka->sem->release();
                        }
                        return;
                    }
                }

                //на мой взгляд лишнее условие
                if(listProverkaItem[i]->relizProverka->liter < 38)
                {
                    Set_Number_Spytnik(listProverkaItem[i]);
                }
                else
                {
                    if(listProverkaItem[i]->relizProverka->liter == 38)
                    {
                        Set_Number_Spytnik(listProverkaItem[i]);
                    }
                    else
                    {
                        Set_Number_Spytnik(listProverkaItem[i]);
                    }
                }
            }

            for(int i=0;i <Count_NP;i++)
            {
              listProverkaItem[i]->relizProverkaTimer->signal_Tick("Загрузка имитатора");
              listProverkaItem[i]->relizProverka->signal_StartProverkaIndex(QString::number(listProverkaItem[i]->relizProverka->countProverka));
            }


            if(autoProverka == false)
            {
                // endOneProverka();

                emit signal_Gen_setCONTrol("START");
                sem.acquire();
                emit signal_StartProverka_5();
            }
            else
            {
                emit signal_Gen_setCONTrol("START");
                sem.acquire();
                emit signal_StartProverka_5();
            }

        }
        else
        {
            //  emit signal_BadQML(proverka1->relizProverka->countProverka);
            break;
        }
        break;


        ///////////////////////

    case 6:
        if(listProverkaItem[position]->relizProverka->job == false)
        {

            if(listProverkaItem[position]->relizProverka->flag_good_6)
            {
                qDebug ()<<"Proverka 6 is GOOD : index =\t" << listProverkaItem[position]->index;
            }
            else
            {
                qDebug ()<<"Proverka 6 is BEAD : index =\t" << listProverkaItem[position]->index;
            }

               emit qml_good(listProverkaItem[position]->relizProverka->flag_good_6,QString::number(index),QString::number(listProverkaItem[position]->index));

        }

        if(listProverkaItem[position]->relizProverka->flag_300MGH_6Proverka == false)
        {
            --start;
            qDebug ()<<"start == "<< start;
            listProverkaItem[position]->relizProverka->sem->release();
            return;
        }


        if(start == Count_NP)
        {
            qDebug ()<<"ALL Proverka 6 is FINISH :   Camber \t" <<  autoMode_NY_cold_hart;
            start=0;

            // emit setSIGNALtype("STOP");


            if(autoProverka == false)
            {
                emit endOneProverka(index);

                for(int i=0;i <Count_NP;i++)
                {
                    listProverkaItem[i]->relizProverka->job  = true;
                    listProverkaItem[i]->relizProverka->sem->release();
                }

                emit signal_Finish();

                return;
            }
            else
            {
                waitTpQML();
                qDebug() <<"tp->slot_clear() sem.available() = "<< sem.available();
                tp->slot_clear();
                sem.acquire();
                qDebug() <<"tp->slot_clear() sem.available() = "<< sem.available();

                kol_cignalov = 0;

                for(int i=0;i <Count_NP;i++)
                {
                    listProverkaItem[i]->relizProverka->job  = true;
                    listProverkaItem[i]->relizProverka->sem->release();
                }
            }

        }
        else
        {
            //  emit signal_BadQML(proverka1->relizProverka->countProverka);
        }
        break;

        //////////////////////////
    case 7:

        if(listProverkaItem[position]->relizProverka->job == false)
        {

            if(listProverkaItem[position]->relizProverka->flag_good_7)
            {
                qDebug ()<<"Proverka 7 is GOOD : index =\t" << listProverkaItem[position]->index;
            }
            else
            {
                qDebug ()<<"Proverka 7 is BEAD : index =\t" << listProverkaItem[position]->index;
            }

               emit qml_good(listProverkaItem[position]->relizProverka->flag_good_7,QString::number(index),QString::number(listProverkaItem[position]->index));

        }

        if(start == Count_NP)
        {

            qDebug ()<<"ALL Proverka 7 is FINISH :   Camber \t" <<  autoMode_NY_cold_hart;
            start=0;


            if(autoProverka == false)
            {
                emit endOneProverka(index);

                for(int i=0;i <Count_NP;i++)
                {
                    listProverkaItem[i]->relizProverka->job  = true;
                    listProverkaItem[i]->relizProverka->sem->release();
                }

                emit signal_Finish();

                return;
            }
            else
            {
                waitTpQML();

                qDebug() <<"tp->slot_clear() sem.available() = "<< sem.available();
                tp->slot_clear();
                sem.acquire();
                qDebug() <<"tp->slot_clear() sem.available() = "<< sem.available();

                kol_cignalov = 0;

                for(int i=0;i <Count_NP;i++)
                {
                    listProverkaItem[i]->relizProverka->job  = true;
                    listProverkaItem[i]->relizProverka->sem->release();
                }
            }

        }
        else
        {
            //  emit signal_BadQML(proverka1->relizProverka->countProverka);
        }
        break;

        ////////////////////////////////

    case 8:

        if(listProverkaItem[position]->relizProverka->job == false)
        {

            if(listProverkaItem[position]->relizProverka->flag_good_8)
            {
                qDebug ()<<"Proverka 8 is GOOD : index =\t" << listProverkaItem[position]->index;
            }
            else
            {
                qDebug ()<<"Proverka 8 is BEAD : index =\t" << listProverkaItem[position]->index;
            }

               emit qml_good(listProverkaItem[position]->relizProverka->flag_good_8,QString::number(index),QString::number(listProverkaItem[position]->index));

        }

        if(start == Count_NP)
        {

            qDebug ()<<"ALL Proverka 8 is FINISH :   Camber \t" <<  autoMode_NY_cold_hart;
            start =0;

            //   emit setSIGNALtype("STOP");



            if(autoProverka == false)
            {
                emit endOneProverka(index);

                for(int i=0;i <Count_NP;i++)
                {
                    listProverkaItem[i]->relizProverka->job  = true;
                    listProverkaItem[i]->relizProverka->sem->release();
                }

                emit signal_Finish();

                return;

            }
            else
            {
                waitTpQML();

                qDebug() <<"tp->slot_clear() sem.available() = "<< sem.available();
                tp->slot_clear();
                sem.acquire();
                qDebug() <<"tp->slot_clear() sem.available() = "<< sem.available();

                kol_cignalov = 0;

                for(int i=0;i <Count_NP;i++)
                {
                    listProverkaItem[i]->relizProverka->job  = true;
                    listProverkaItem[i]->relizProverka->sem->release();
                }
            }

        }
        else
        {
            //  emit signal_BadQML(proverka1->relizProverka->countProverka);
        }
        break;

        /////////////////
    case 9:
        if(listProverkaItem[position]->relizProverka->job == false)
        {

            if(listProverkaItem[position]->relizProverka->flag_good_9)
            {
                qDebug ()<<"Proverka 9 is GOOD : index =\t" << listProverkaItem[position]->index;
            }
            else
            {
                qDebug ()<<"Proverka 9 is BEAD : index =\t" << listProverkaItem[position]->index;
            }

               emit qml_good(listProverkaItem[position]->relizProverka->flag_good_9,QString::number(index),QString::number(listProverkaItem[position]->index));

        }

        if(start == Count_NP)
        {
            qDebug ()<<"ALL Proverka 9 is FINISH :   Camber \t" <<  autoMode_NY_cold_hart;
            start = 0;
            //  emit setSIGNALtype("STOP");



            if(autoProverka == false)
            {
                emit endOneProverka(index);

                for(int i=0;i <Count_NP;i++)
                {
                    listProverkaItem[i]->relizProverka->job  = true;
                    listProverkaItem[i]->relizProverka->sem->release();
                }

                emit signal_Finish();

                return;
            }
            else
            {
                waitTpQML();

                qDebug() <<"tp->slot_clear() sem.available() = "<< sem.available();
                tp->slot_clear();
                sem.acquire();
                qDebug() <<"tp->slot_clear() sem.available() = "<< sem.available();

                kol_cignalov = 0;

                for(int i=0;i <Count_NP;i++)
                {
                    listProverkaItem[i]->relizProverka->job  = true;
                    listProverkaItem[i]->relizProverka->sem->release();
                }
            }

        }
        else
        {
            //  emit signal_BadQML(proverka1->relizProverka->countProverka);
        }
        break;

        /////////////////
    case 10:

        if(listProverkaItem[position]->relizProverka->job == false)
        {

            if(listProverkaItem[position]->relizProverka->flag_good_10)
            {
                qDebug ()<<"Proverka 10 is GOOD : index =\t" << listProverkaItem[position]->index;
            }
            else
            {
                qDebug ()<<"Proverka 10 is BEAD : index =\t" << listProverkaItem[position]->index;
            }

               emit qml_good(listProverkaItem[position]->relizProverka->flag_good_10,QString::number(index),QString::number(listProverkaItem[position]->index));

        }

        if(start == Count_NP)
        {

            qDebug ()<<"ALL Proverka 10 is FINISH :   Camber \t" <<  autoMode_NY_cold_hart;
            start =0;

            emit setSIGNALtype("STOP");



            if(autoProverka == false)
            {
                emit endOneProverka(index);

                for(int i=0;i <Count_NP;i++)
                {
                    listProverkaItem[i]->relizProverka->job  = true;
                    listProverkaItem[i]->relizProverka->sem->release();
                }

                emit signal_Finish();

                return;
            }
            else
            {
                waitTpQML();

                qDebug() <<"tp->slot_clear() sem.available() = "<< sem.available();
                tp->slot_clear();
                sem.acquire();
                qDebug() <<"tp->slot_clear() sem.available() = "<< sem.available();

                kol_cignalov = 0;

                //                for(int i=0;i <Count_NP;i++)
                //                {
                //                    listProverkaItem[i]->relizProverka->job  = true;
                //                    listProverkaItem[i]->relizProverka->sem->release();
                //                }

                qDebug() << "LOL 3 signal_startColdHard = " << autoMode_NY_cold_hart; //1
                // emit signal_startColdHard();

                //WARNING
                //Дописать код обнуления информации на странице

                emit signal_clearView();

                //autoMode_NY_cold_hart = 1; //временно убрать когда появится камера

                if(flagWork_Dont_PSIandRegylirovka)
                {
                    autoMode_NY_cold_hart = 5;
                }





                switch (autoMode_NY_cold_hart)
                {
                //запустить камеру для НУ
                case 0: emit signal_NextCamberTemprature();break; //Устанавливаем НУ

                    //запустить камеру для охлаждения
                case 1:  emit signal_NextCamberTemprature();break; //Устанавливаем -40
                    //запустить камеру для НУ
                case 2: emit signal_NextCamberTemprature(); break; //Устанавливаем НУ после -40
                    //запустить камеру для нагрева
                case 3: emit signal_NextCamberTemprature(); break; //Устанавливаем +50
                    //запустить камеру для НУ
                case 4: emit signal_NextCamberTemprature(); break; //Устанавливаем НУ после +50

                case 5:
                {
                    emit signal_Finish();

                    emit signal_stopCamberWorkProverka();

                    autoMode_NY_cold_hart = 1;
                    kol_Auto_Start =0; //Устанавливаем НУ


                    for(int i=0; i < listProverkaItem.count();i++)
                    {
                        listProverkaItem[i]->relizProverka->flag_auto = false;
                    }

                    qDebug() << "!!!!!!!!!!!!!!!!!!!!!!!! END !!!!!!!!!!!!!!!!!!!!!!!!";

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


void Algorithm::Set_Number_Spytnik(proverkaItem* flag)
{

    if(kolStart != Count_NP)
    {
        kolStart++;
    }

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



    for(int i=0;i < listProverkaItem.count();i++)
    {
        listProverkaItem[i]->relizProverka->stay_liter = stay;
    }


    qDebug () << " Set spytnik  = " << liters;

    switch (liters) {
    case 25:
        if(kolStart == Count_NP)
        {
            emit signal_Gen_setSATid("R10"); //-7 liter +


            for(int i=0;i < listProverkaItem.count();i++)
            {
                BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-7','"+listProverkaItem[i]->relizProverka->IdLink+"')");
            }
        }

        break;
    case 26:
        if(kolStart == Count_NP)
        {
            emit signal_Gen_setSATid("R18"); //-6 liter +

            for(int i=0;i < listProverkaItem.count();i++)
            {
                BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-6','"+listProverkaItem[i]->relizProverka->IdLink+"')");
            }
        }


        break;
    case 27:
        if(kolStart == Count_NP)
        {
            emit signal_Gen_setSATid("R14"); //-5 liter +

            for(int i=0;i < listProverkaItem.count();i++)
            {
                BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-5','"+listProverkaItem[i]->relizProverka->IdLink+"')");
            }
        }


        break;
    case 28:
        if(kolStart == Count_NP)
        {
            emit signal_Gen_setSATid("R6");  //-4 liter +


            for(int i=0;i < listProverkaItem.count();i++)
            {
                BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-4','"+listProverkaItem[i]->relizProverka->IdLink+"')");
            }
        }


        break;
    case 29:
        if(kolStart == Count_NP)
        {
            emit signal_Gen_setSATid("R22"); //-3 liter +

            for(int i=0;i < listProverkaItem.count();i++)
            {
                BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-3','"+listProverkaItem[i]->relizProverka->IdLink+"')");
            }
        }

        break;
    case 30:
        if(kolStart == Count_NP)
        {
            emit signal_Gen_setSATid("R13");  //-2 liter +

            for(int i=0;i < listProverkaItem.count();i++)
            {
                BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-2','"+listProverkaItem[i]->relizProverka->IdLink+"')");
            }
        }


        break;
    case 31:
        if(kolStart == Count_NP)
        {
            emit signal_Gen_setSATid("R12"); //-1 liter +

            for(int i=0;i < listProverkaItem.count();i++)
            {
                BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','-1','"+listProverkaItem[i]->relizProverka->IdLink+"')");
            }
        }


        break;
    case 32:
        if(kolStart == Count_NP)
        {
            emit signal_Gen_setSATid("R11"); //0 liter +

            for(int i=0;i < listProverkaItem.count();i++)
            {
                BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','0','"+listProverkaItem[i]->relizProverka->IdLink+"')");
            }
        }

        break;
    case 33:
        if(kolStart == Count_NP)
        {
            emit signal_Gen_setSATid("R5"); //1 liter +

            for(int i=0;i < listProverkaItem.count();i++)
            {
                BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','1','"+listProverkaItem[i]->relizProverka->IdLink+"')");
            }
        }


        break;
    case 34:
        if(kolStart == Count_NP)
        {
            emit signal_Gen_setSATid("R20"); //2 liter +

            for(int i=0;i < listProverkaItem.count();i++)
            {
                BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','2','"+listProverkaItem[i]->relizProverka->IdLink+"')");
            }
        }

        break;
    case 35:
        if(kolStart == Count_NP)
        {
            emit signal_Gen_setSATid("R19"); //3 liter +

            for(int i=0;i < listProverkaItem.count();i++)
            {
                BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','3','"+listProverkaItem[i]->relizProverka->IdLink+"')");
            }
        }


        break;
    case 36:
        if(kolStart == Count_NP)
        {
            emit signal_Gen_setSATid("R17"); //4 liter +

            for(int i=0;i < listProverkaItem.count();i++)
            {
                BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','4','"+listProverkaItem[i]->relizProverka->IdLink+"')");
            }
        }


        break;
    case 37:
        if(kolStart == Count_NP)
        {
            emit signal_Gen_setSATid("R7"); //5 liter +

            for(int i=0;i < listProverkaItem.count();i++)
            {
                BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','5','"+listProverkaItem[i]->relizProverka->IdLink+"')");
            }
        }


        break;
    case 38:
        if(kolStart == Count_NP)
        {
            emit signal_Gen_setSATid("R8"); //6 liter +

            for(int i=0;i < listProverkaItem.count();i++)
            {
                BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','6','"+listProverkaItem[i]->relizProverka->IdLink+"')");
            }
        }


        break;
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    case 129:
        if(kolStart == Count_NP)
        {
            emit signal_Gen_setSATid("G1"); //G1 liter +

            for(int i=0;i < listProverkaItem.count();i++)
            {
                BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G1','"+listProverkaItem[i]->relizProverka->IdLink+"')");
            }
        }


        break;
    case 130:
        if(kolStart == Count_NP)
        {
            emit signal_Gen_setSATid("G2"); //G2 liter +

            for(int i=0;i < listProverkaItem.count();i++)
            {
                BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G2','"+listProverkaItem[i]->relizProverka->IdLink+"')");
            }
        }


        break;
    case 131:
        if(kolStart == Count_NP)
        {
            emit signal_Gen_setSATid("G3"); //G3 liter +

            for(int i=0;i < listProverkaItem.count();i++)
            {
                BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G3','"+listProverkaItem[i]->relizProverka->IdLink+"')");
            }
        }


        break;
    case 132:
        if(kolStart == Count_NP)
        {
            emit signal_Gen_setSATid("G4"); //G4 liter +

            for(int i=0;i < listProverkaItem.count();i++)
            {
                BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G4','"+listProverkaItem[i]->relizProverka->IdLink+"')");
            }
        }


        break;
    case 133:
        if(kolStart == Count_NP)
        {
            emit signal_Gen_setSATid("G5"); //G5 liter +

            for(int i=0;i < listProverkaItem.count();i++)
            {
                BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G5','"+listProverkaItem[i]->relizProverka->IdLink+"')");
            }
        }


        break;
    case 134:
        if(kolStart == Count_NP)
        {
            emit signal_Gen_setSATid("G6"); //G6 liter +

            for(int i=0;i < listProverkaItem.count();i++)
            {
                BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G6','"+listProverkaItem[i]->relizProverka->IdLink+"')");
            }
        }


        break;
    case 135:
        if(kolStart == Count_NP)
        {
            emit signal_Gen_setSATid("G7"); //G7 liter +

            for(int i=0;i < listProverkaItem.count();i++)
            {
                BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G7','"+listProverkaItem[i]->relizProverka->IdLink+"')");
            }
        }


        break;
    case 136:
        if(kolStart == Count_NP)
        {
            emit signal_Gen_setSATid("G8"); //G8 liter +

            for(int i=0;i < listProverkaItem.count();i++)
            {
                BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G8','"+listProverkaItem[i]->relizProverka->IdLink+"')");
            }
        }


        break;
    case 137:
        if(kolStart == Count_NP)
        {
            emit signal_Gen_setSATid("G9"); //G9 liter +

            for(int i=0;i < listProverkaItem.count();i++)
            {
                BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G9','"+listProverkaItem[i]->relizProverka->IdLink+"')");
            }
        }


        break;
    case 138:
        if(kolStart == Count_NP)
        {
            emit signal_Gen_setSATid("G10"); //G10 liter +

            for(int i=0;i < listProverkaItem.count();i++)
            {
                BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G10','"+listProverkaItem[i]->relizProverka->IdLink+"')");
            }
        }


        break;
    case 139:
        if(kolStart == Count_NP)
        {
            emit signal_Gen_setSATid("G11"); //G11 liter +

            for(int i=0;i < listProverkaItem.count();i++)
            {
                BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G11','"+listProverkaItem[i]->relizProverka->IdLink+"')");
            }
        }


        break;
    case 140:
        if(kolStart == Count_NP)
        {
            emit signal_Gen_setSATid("G12"); //G12 liter +

            for(int i=0;i < listProverkaItem.count();i++)
            {
                BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G12','"+listProverkaItem[i]->relizProverka->IdLink+"')");
            }
        }


        break;
    case 141:
        if(kolStart == Count_NP)
        {
            emit signal_Gen_setSATid("G13"); //G13 liter +

            for(int i=0;i < listProverkaItem.count();i++)
            {
                BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G13','"+listProverkaItem[i]->relizProverka->IdLink+"')");
            }
        }


        break;
    case 142:
        if(kolStart == Count_NP)
        {
            emit signal_Gen_setSATid("G14"); //G14 liter +

            for(int i=0;i < listProverkaItem.count();i++)
            {
                BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G14','"+listProverkaItem[i]->relizProverka->IdLink+"')");
            }
        }


        break;
    case 143:
        if(kolStart == Count_NP)
        {
            emit signal_Gen_setSATid("G15"); //G15 liter +

            for(int i=0;i < listProverkaItem.count();i++)
            {
                BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G15','"+listProverkaItem[i]->relizProverka->IdLink+"')");
            }
        }


        break;
    case 144:
        if(kolStart == Count_NP)
        {
            emit signal_Gen_setSATid("G16"); //G16 liter +

            for(int i=0;i < listProverkaItem.count();i++)
            {
                BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G16','"+listProverkaItem[i]->relizProverka->IdLink+"')");
            }
        }


        break;
    case 145:
        if(kolStart == Count_NP)
        {
            emit signal_Gen_setSATid("G17"); //G17 liter +

            for(int i=0;i < listProverkaItem.count();i++)
            {
                BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G17','"+listProverkaItem[i]->relizProverka->IdLink+"')");
            }
        }


        break;
    case 146:
        if(kolStart == Count_NP)
        {
            emit signal_Gen_setSATid("G18"); //G18 liter +

            for(int i=0;i < listProverkaItem.count();i++)
            {
                BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G18','"+listProverkaItem[i]->relizProverka->IdLink+"')");
            }
        }


        break;
    case 147:
        if(kolStart == Count_NP)
        {
            emit signal_Gen_setSATid("G19"); //G19 liter +

            for(int i=0;i < listProverkaItem.count();i++)
            {
                BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G19','"+listProverkaItem[i]->relizProverka->IdLink+"')");
            }
        }


        break;
    case 148:
        if(kolStart == Count_NP)
        {
            emit signal_Gen_setSATid("G20"); //G20 liter +

            for(int i=0;i < listProverkaItem.count();i++)
            {
                BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G20','"+listProverkaItem[i]->relizProverka->IdLink+"')");
            }
        }

        break;
    case 149:
        if(kolStart == Count_NP)
        {
            emit signal_Gen_setSATid("G21"); //G21 liter +

            for(int i=0;i < listProverkaItem.count();i++)
            {
                BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G21','"+listProverkaItem[i]->relizProverka->IdLink+"')");
            }
        }


        break;
    case 150:
        if(kolStart == Count_NP)
        {
            emit signal_Gen_setSATid("G22"); //G22 liter +

            for(int i=0;i < listProverkaItem.count();i++)
            {
                BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G22','"+listProverkaItem[i]->relizProverka->IdLink+"')");
            }
        }


        break;
    case 151:
        if(kolStart == Count_NP)
        {
            emit signal_Gen_setSATid("G23"); //G23 liter +

            for(int i=0;i < listProverkaItem.count();i++)
            {
                BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G23','"+listProverkaItem[i]->relizProverka->IdLink+"')");
            }
        }


        break;
    case 152:
        if(kolStart == Count_NP)
        {

            emit signal_Gen_setSATid("G24"); //G24 liter +

            for(int i=0;i < listProverkaItem.count();i++)
            {
                BD->zaprosQueryModel("INSERT INTO Liter (Date,NP,IdLink) VALUES('"+stay.toString("dd.MM.yyyy  hh:mm:ss")+"','G24','"+listProverkaItem[i]->relizProverka->IdLink+"')");
            }
        }

        break;
    default:
        break;
    }
}
