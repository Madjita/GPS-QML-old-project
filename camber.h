#ifndef CAMBER_H
#define CAMBER_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QMessageBox>

#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QHttpMultiPart>
#include <QUrl>
#include <QUrlQuery>


#include <QTimer>
#include <QThread>

class Update ;


struct CamberInform
{
    QString tempPV; //темпиратура измеряемая
    QString tempSV; // темпиратура которая должна установиться

    QString data;  //дата
    QString current; // состояние камеры

    QString totalTime; // время начала
    QString stepTimeLeft; // время через сколько закончится

    QString tempHeater;  // охлаждение
    QString refrigeration; // темпиратура нагревателя

};


class Camber : public QObject
{
    Q_OBJECT
public:
     explicit Camber(QObject *parent = nullptr);
        ~Camber();

    Update* updateCamberInform;
    QString mode;
    CamberInform* camberInform;
    bool flagConnect;
    QString ip;

signals:
    void signalConnect(); //Отправить ПОСТ запрос для авторизации
    void signalOpenBoard(); //Отправить ПОСТ запрос для открытия страницы управления
    void signalStartTiemr();
    void signalStopTiemr();
    void sendData(QString current,QString dataTime,QString totalTime,QString stepTimeLeft,QString tempSV,QString tempPV,QString tempHeater,QString refrigeration);
    void sendData2(QString);


    void signalWorkOK();
    void signalWorkStopOK();

    void errorCamber(QString); // Ошибка информация

    void connectOk();
    void signal_finished();

public  slots:
    //Слот для чтение данных с подключенным устроойством
     void SendMessageConnect();
      void ReadMessage();
      void ReadMessageDevice();

      /////////////////////////////////////////////
      void getResponse(QNetworkReply *reply);

      void connectCamber(QString ip); // Открыть login
      void sendCamberPostAdmin(); // Отправить ПОСТ запрос для авторизации
      void openCamberBoard(); // Открыть Панель управления

      void startCamberWork(); //Запустить работу камеры
      void startCamberWorkOk(); //Подтвердить запуск работы камеры
      void stopCamberWork(); //Остановить работу камеры
      void stopCamberWorkOk(); //Подтвердить остановку работы камеры

      void setCamberNY(); // Установить программу для камеры НУ (+27 градусов)
      void setCamberHort(); // Установить программу для камеры Нагрев (+60 градусов)
      void setCamberCold(); // Установить программу для камеры Охлаждение (-70 градусов)



      void prosecc();

public:
    QTcpSocket *client;
    QTcpSocket *clientDevice;
    QHostAddress s_address;
    quint16 s_port;
    QByteArray dataConnect;


    ///////////
    QNetworkAccessManager *mngr;

};
#endif // CAMBER_H


class Update : public QObject
{
    Q_OBJECT
public:
    explicit Update(Camber* GetCamber,QObject *parent = 0);
       ~Update();

     QTimer      *internalTimer;         // Таймер, по которому будет изменяться время
     QTime       m_Time;           // Текущее время таймера
     Camber* camber;


     //
     QString strTempSV; //установленная
     QString strTempPV; // текущая

public slots:
     void updateInform();
     void process_start_thread();
     void start_m_Time();
     void stop_m_Time();

     void RasparsHTML(QString);

     void process_start();


};


