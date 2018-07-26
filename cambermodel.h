#ifndef CAMBERMODEL_H
#define CAMBERMODEL_H

#include <QObject>
#include "camber.h"



class CamberModel : public QObject
{
    Q_OBJECT
public:
    explicit CamberModel(QObject *parent = 0);
    ~CamberModel();

    Camber* camber;

signals:
   void signal_startCamberWork(); //Запустить работу камеры
   void signal_stopCamberWork(); //Остановить работу камеры
   void signal_connectCamber(QString ip); // Открыть login
   void sendData(QString current,QString dataTime,QString totalTime,QString stepTimeLeft,QString tempSV,QString tempPV,QString tempHeater,QString refrigeration);


   void signal_setCamberNY(); // Установить программу для камеры НУ (+27 градусов)
   void signal_setCamberHort(); // Установить программу для камеры Нагрев (+60 градусов)
   void signal_setCamberCold(); // Установить программу для камеры Охлаждение (-70 градусов)

   void signal_startTimer();

   void signal_Ok(bool good);

public slots:
    void slot_startCamberWork(); //Запустить работу камеры
    void slot_stopCamberWork(); //Остановить работу камеры
    void slot_connectCamber(QString ip); // Открыть login

    bool slot_TestConnect(); // Проверка подключенна ли камера

    void slot_StartTimer(); //Запуск таймера

    void slot_SetNY(); // Установить 27 градусов (20)

    void slot_SetHord(); // Установить +50 (+55)

    void slot_SetCold();// Установить -60 (-40)

    bool slot_connectOk();
};

#endif // CAMBERMODEL_H
