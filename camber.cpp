#include "camber.h"

Camber::Camber(QObject *parent) : QObject(parent),
    mode("login"),
    camberInform(new CamberInform)
{

    connect(this,&Camber::destroyed,this,&Camber::stopCamberWork,Qt::DirectConnection);

    this->moveToThread(new QThread());
    connect(this->thread(),&QThread::started,this,&Camber::prosecc);
    connect(this,&Camber::signal_finished, this->thread(), &QThread::quit);
    this->thread()->start();

}

Camber::~Camber()
{
    emit signal_finished();
}

void Camber::SendMessageConnect()
{
    qDebug() << "Устройство подключенно";

    QByteArray SendData = client->readAll();

    qDebug () << SendData;

}

void Camber::ReadMessage()
{

    qDebug () << "client";
    QByteArray SendData;
    SendData = client->readAll();

    qDebug () << SendData;
}

void Camber::ReadMessageDevice()
{
    qDebug () << "clientDevice";
    QByteArray SendData;

    SendData = clientDevice->readAll();

    qDebug () << SendData;

}

void Camber::getResponse(QNetworkReply *reply)
{

    if( reply->error() == QNetworkReply::NoError )
    {
        QString data = QString::fromUtf8( reply->readAll() );
        // qDebug() << data ;

        qDebug() << "Ready!";




        if(mode == "login")
        {
            emit signalConnect();
        }

        if(mode == "login_connect")
        {
            flagConnect = true;
            emit connectOk();
            emit signalOpenBoard();
        }

        if(mode == "wh01")
        {
            //запускаем таймер на обновление информации о камере
            if(updateCamberInform->internalTimer->isActive())
            {

            }
            else
            {
               // emit signalStartTiemr();
            }
        }

        if(mode == "status")
        {
            if(updateCamberInform->internalTimer->isActive())
            {
                emit sendData2(data);
            }
        }

        if(mode == "wh01_work")
        {
            emit signalWorkOK();
        }

        if(mode == "wh01_work_ok")
        {
            emit signalStartTiemr();
        }

        if(mode == "wh01_work_stop")
        {
            emit signalWorkStopOK();
        }

        if(mode == "wh01_work_stop_ok")
        {
           // emit signalStartTiemr();
        }



    } else
    {

        if( reply->error() == QNetworkReply::NoError )
        {
            flagConnect = false;
            emit connectOk();
        }


        if(mode == "login_connect" || mode == "login")
        {
            flagConnect = false;
            emit connectOk();
        }

        qDebug() << reply->errorString();

        emit errorCamber(reply->errorString());
    }


    qDebug () <<  reply->error();

    // Мы сами должны освободить память для reply
    // Однако делать это через delete нельзя
    reply->deleteLater();

}

void Camber::connectCamber(QString _ip)
{
    mode.clear();
    mode = "login";
    ip = _ip;
    QNetworkRequest request(QUrl("http://"+ip+"/login.php")); //status.php
    mngr->get(request);
}

void Camber::sendCamberPostAdmin()
{
    mode.clear();
    mode = "login_connect";

    QNetworkRequest request(QUrl("http://"+ip+"/login.php"));

    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/x-www-form-urlencoded"));

    QUrlQuery  params;
    params.addQueryItem("username", "Administrator");
    params.addQueryItem("password", "espec");
    params.addQueryItem("login_button", "Accept the license agreement and login.");

    QUrl lol;

    lol.setQuery(params);


    qDebug () << lol.toEncoded().remove(0,1);

    // Не забудьте назначить ответ в качестве родителя, иначе будет утечка.
    mngr->post(request,lol.toEncoded().remove(0,1));
}

void Camber::openCamberBoard()
{
    mode.clear();
    mode = "wh01";

    QNetworkRequest request2(QUrl("http://"+ip+"/wo01.php"));

    mngr->get(request2);
}

void Camber::startCamberWork()
{
   // emit signalStopTiemr();

    mode.clear();
    mode = "wh01_work";

    QNetworkRequest request(QUrl("http://"+ip+"/wo01.php"));

    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/x-www-form-urlencoded"));

    QUrlQuery  params;
    params.addQueryItem("target", "pgmstart");
    params.addQueryItem("btn_session_clear", "");
    params.addQueryItem("pgmstart", "Start");
    params.addQueryItem("pgm_no", "1");
    params.addQueryItem("step_no", "1");

    QUrl lol;

    lol.setQuery(params);

    // Не забудьте назначить ответ в качестве родителя, иначе будет утечка.
    mngr->post(request,lol.toEncoded().remove(0,1));
}

void Camber::startCamberWorkOk()
{
    mode.clear();
    mode = "wh01_work_ok";

    QNetworkRequest request(QUrl("http://"+ip+"/wo01.php"));
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/x-www-form-urlencoded"));

    QUrlQuery  params;
    params.addQueryItem("target", "");
    params.addQueryItem("btn_session_clear", "");
    params.addQueryItem("pgm_no", "1");
    params.addQueryItem("step_no", "1");

    QUrl lol;

    lol.setQuery(params);

    // Не забудьте назначить ответ в качестве родителя, иначе будет утечка.
    mngr->post(request,lol.toEncoded().remove(0,1));
}

void Camber::stopCamberWork()
{
    emit signalStopTiemr();

    mode.clear();
    mode = "wh01_work_stop";

    QNetworkRequest request(QUrl("http://"+ip+"/wo01.php"));
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/x-www-form-urlencoded"));

    QUrlQuery  params;
    params.addQueryItem("target", "shutdown");
    params.addQueryItem("btn_session_clear", "");
    params.addQueryItem("shutdown", "Stop");

    QUrl lol;

    lol.setQuery(params);

    // Не забудьте назначить ответ в качестве родителя, иначе будет утечка.
    mngr->post(request,lol.toEncoded().remove(0,1));
}

void Camber::stopCamberWorkOk()
{
    //можно дописать код на остановку таймера
    mode.clear();
    mode = "wh01_work_stop_ok";

    QNetworkRequest request(QUrl("http://"+ip+"/wo01.php"));


    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/x-www-form-urlencoded"));

    QUrlQuery  params;
    params.addQueryItem("target", "");
    params.addQueryItem("btn_session_clear", "");

    QUrl lol;

    lol.setQuery(params);

    // Не забудьте назначить ответ в качестве родителя, иначе будет утечка.
    mngr->post(request,lol.toEncoded().remove(0,1));
}

void Camber::setCamberNY()
{
    //camberInform->tempSV = "25";
    emit signalStartTiemr();

    mode.clear();
    QNetworkRequest request(QUrl("http://"+ip+"/wo01.php"));
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/x-www-form-urlencoded"));

    QUrlQuery  params;
    params.addQueryItem("target", "pgmselect");
    params.addQueryItem("btn_session_clear", "");
    params.addQueryItem("pgm_no", "3"); //1
    params.addQueryItem("step_no", "1");

    QUrl lol;

    lol.setQuery(params);

    // Не забудьте назначить ответ в качестве родителя, иначе будет утечка.
    mngr->post(request,lol.toEncoded().remove(0,1));
}

void Camber::setCamberHort()
{
   // camberInform->tempSV = "55";
    emit signalStartTiemr();



    mode.clear();
    QNetworkRequest request(QUrl("http://"+ip+"/wo01.php"));


    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/x-www-form-urlencoded"));

    QUrlQuery  params;
    params.addQueryItem("target", "pgmselect");
    params.addQueryItem("btn_session_clear", "");
    params.addQueryItem("pgm_no", "5"); //2
    params.addQueryItem("step_no", "1");

    QUrl lol;

    lol.setQuery(params);


    qDebug () << lol.toEncoded().remove(0,1);

    // Не забудьте назначить ответ в качестве родителя, иначе будет утечка.
    mngr->post(request,lol.toEncoded().remove(0,1));
}

void Camber::setCamberCold()
{

   // camberInform->tempSV = "-40";
    emit signalStartTiemr();

    //написать код для установки 3 программы (установить ее в камере)
    mode.clear();
    QNetworkRequest request(QUrl("http://"+ip+"/wo01.php"));


    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/x-www-form-urlencoded"));

    QUrlQuery  params;
    params.addQueryItem("target", "pgmselect");
    params.addQueryItem("btn_session_clear", "");
    params.addQueryItem("pgm_no", "4"); //2
    params.addQueryItem("step_no", "1");

    QUrl lol;

    lol.setQuery(params);


    qDebug () << lol.toEncoded().remove(0,1);

    // Не забудьте назначить ответ в качестве родителя, иначе будет утечка.
    mngr->post(request,lol.toEncoded().remove(0,1));
}



void Camber::prosecc()
{
    mngr = new QNetworkAccessManager(this);

    updateCamberInform = new Update(this);



    connect(updateCamberInform->thread(),&QThread::started,updateCamberInform,&Update::process_start_thread);

    connect(this,&Camber::signalStartTiemr,updateCamberInform,&Update::start_m_Time);
    connect(this,&Camber::signalStopTiemr,updateCamberInform,&Update::stop_m_Time,Qt::DirectConnection);


    connect(mngr, SIGNAL(finished(QNetworkReply*)),this, SLOT(getResponse(QNetworkReply*)));
    connect(this,&Camber::signalConnect,this,&Camber::sendCamberPostAdmin,Qt::DirectConnection);
    connect(this,&Camber::signalOpenBoard,this,&Camber::openCamberBoard,Qt::DirectConnection);
    connect(this,&Camber::signalWorkOK,this,&Camber::startCamberWorkOk,Qt::DirectConnection);
    connect(this,&Camber::signalWorkStopOK,this,&Camber::stopCamberWorkOk,Qt::DirectConnection);
    connect(this,&Camber::sendData2,updateCamberInform,&Update::RasparsHTML,Qt::DirectConnection);
}

//Другой класс
/////////////////////////////////////////////////////////////////////////////
Update::Update(Camber* GetCamber,QObject *parent) :
    QObject(parent),
    camber(GetCamber),
    m_Time(QTime(0,0,0,0))
{
    this->moveToThread(new QThread());
    connect(this->thread(),&QThread::started,this,&Update::process_start);
    this->thread()->start();
}

Update::~Update()
{
    internalTimer->stop();
}

void Update::updateInform()
{
//    if(camber->camberInform->tempSV.toInt() > 0 && camber->camberInform->tempPV.toInt() < 0)
//    {
//       camber->camberInform->tempPV =  QString::number(strTempPV.toInt() + 1);
//    }
//    else
//    {

//        if(camber->camberInform->tempSV.toInt() > 0 && camber->camberInform->tempPV.toInt() > 0)
//        {
//            if(camber->camberInform->tempPV.toInt() > camber->camberInform->tempSV.toInt())
//            {
//                camber->camberInform->tempPV =  QString::number(camber->camberInform->tempPV.toInt() - 1);
//            }
//            else
//            {
//              camber->camberInform->tempPV =  QString::number(camber->camberInform->tempPV.toInt() + 1);
//            }

//        }
//        else
//        {
//            if(camber->camberInform->tempPV.toInt() < camber->camberInform->tempSV.toInt())
//            {
//                camber->camberInform->tempPV =  QString::number(camber->camberInform->tempPV.toInt() + 1);
//            }
//            else
//            {
//              camber->camberInform->tempPV =  QString::number(camber->camberInform->tempPV.toInt() - 1);
//            }

//        }
//    }

//     qDebug () << "=============================================";

//     qDebug () << camber->camberInform->tempSV;
//     qDebug () << camber->camberInform->tempPV;

//     camber->sendData(camber->camberInform->current,camber->camberInform->data,camber->camberInform->totalTime,camber->camberInform->stepTimeLeft,camber->camberInform->tempSV,camber->camberInform->tempPV,camber->camberInform->tempHeater,camber->camberInform->refrigeration);



    camber->mode.clear();
    camber->mode = "status";
    QNetworkRequest request2(QUrl("http://"+camber->ip+"/status.php"));

    camber->mngr->get(request2);
}

void Update::process_start_thread()
{
    internalTimer = new QTimer(this);   // Инициализируем таймер

    connect(internalTimer,&QTimer::timeout,this,&Update::updateInform);
}

void Update::start_m_Time()
{
    internalTimer->start(1000); //500
}

void Update::stop_m_Time()
{

    //if(internalTimer->isActive())
   // {
        internalTimer->stop();
   // }
}

void Update::RasparsHTML(QString str)
{
    QString find;

    int j=0;

    while ((j = str.indexOf("<span id=\"monitor-temp-pv-value\">", j)) != -1) {

        ++j;

        for(int i=32; i < 50;i++)
        {
            find += str[j+i];
        }

    }

    camber->camberInform->tempPV = find.split(QLatin1Char('<')).first(); //Записываем в структуру найденную темпиратуру (текущию)

    find.clear();
    j=0;

    while ((j = str.indexOf("<span id=\"date-value\">", j)) != -1) {

        ++j;

        for(int i=21; i < 50;i++)
        {
            find += str[j+i];
        }

    }

    camber->camberInform->data = find.split(QLatin1Char('<')).first(); //Записываем в структуру найденную темпиратуру (текущию)

    find.clear();
    j=0;

    while ((j = str.indexOf("<span id=\"operation-status-value\">", j)) != -1) {

        ++j;

        for(int i=33; i < 100;i++)
        {
            find += str[j+i];
        }

    }

    camber->camberInform->current = find.split(QLatin1Char('<')).first(); //Записываем в структуру режим камеры (текущий)

    find.clear();
    j=0;

    while ((j = str.indexOf("<span id=\"monitor-temp-sv-value\">", j)) != -1) {

        ++j;

        for(int i=32; i < 50;i++)
        {
            find += str[j+i];
        }

    }

    camber->camberInform->tempSV = find.split(QLatin1Char('<')).first(); //Записываем в структуру найденную темпиратуру (установленную)


    find.clear();
    j=0;

    while ((j = str.indexOf("<span id=\"monitor-temp-heater-value\">", j)) != -1) {

        ++j;

        for(int i=36; i < 50;i++)
        {
            find += str[j+i];
        }

    }
    camber->camberInform->tempHeater = find.split(QLatin1Char('<')).first(); //Записываем в структуру найденную величину нагрева

    find.clear();
    j=0;

    while ((j = str.indexOf("monitor-ref-value", j)) != -1) {

        ++j;

        for(int i=18; i < 100;i++)
        {
            find += str[j+i];
        }

    }
    camber->camberInform->refrigeration = find ;//.split(QLatin1Char('<')).first(); //Записываем в структуру найденную величину нагрева

    find.clear();
    j=0;

    while ((j = str.indexOf("<span id=\"program-total-time-value\">", j)) != -1) {

        ++j;

        for(int i=35; i < 60;i++)
        {
            find += str[j+i];
        }

    }

    camber->camberInform->totalTime = find.split(QLatin1Char('<')).first(); //Записываем в структуру найденную вечеличну времени работы от старта


    find.clear();
    j=0;

    while ((j = str.indexOf("<span id=\"program_step_remain_value\">", j)) != -1) {

        ++j;

        for(int i=36; i < 60;i++)
        {
            find += str[j+i];
        }

    }

    camber->camberInform->stepTimeLeft = find.split(QLatin1Char('<')).first(); //Записываем в структуру найденную вечеличну времени работы от старта

    qDebug () << "=============================================";
    qDebug () << camber->camberInform->current;
    qDebug () << camber->camberInform->data;
    qDebug () << camber->camberInform->totalTime;
    qDebug () << camber->camberInform->stepTimeLeft;
    qDebug () << camber->camberInform->tempSV;
    qDebug () << camber->camberInform->tempPV;
    qDebug () << camber->camberInform->tempHeater;
    qDebug () << camber->camberInform->refrigeration;

    camber->sendData(camber->camberInform->current,camber->camberInform->data,camber->camberInform->totalTime,camber->camberInform->stepTimeLeft,camber->camberInform->tempSV,camber->camberInform->tempPV,camber->camberInform->tempHeater,camber->camberInform->refrigeration);

}

void Update::process_start()
{

}
