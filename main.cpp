//#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QApplication>

#include <gsgmodel.h>
#include <n6700model.h>
#include <portmodel.h>
#include <osuilografmodel.h>

#include <proverka.h>

#include <tp8.h>
#include <bdata.h>
#include <report.h>


//////////////////
#include <finddevice.h>


#include <screencapture.h>

#include <QQuickWindow>


//////библиотека таймера////////////
#include "clockcircle.h"
//////библиотека Управления камерой////////////
#include "cambermodel.h"

int main(int argc, char *argv[])
{

    // Если бы передан дополнительный аргумент,
    //    if (argc > 1)
     //   {
            // то попытаемся вывести полученный аргумент
     //       qDebug() << argv[1]<<endl;

     //       return 0;
     //   }
      //  else
      //  {
            // В противном случае сообщаем, что аргументы не передавались
      //      qDebug() << "Without arguments" << endl;

       // }



    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);


    // Qt Charts uses Qt Graphics View Framework for drawing, therefore QApplication must be used.
    QApplication app(argc, argv);



    app.setApplicationVersion("1.0");
    app.setOrganizationName("RadioSvRz");


    // Всё, что требуется в данном файле - это зарегистрировать новый класс (Тип объекта) для QML слоя
    qmlRegisterType<ClockCircle>("ClockCircle",1,0,"ClockCircle");


    FindDevice* find = new FindDevice();

    //find->moveToThread(new QThread());

    // find->thread()->start();

    qDebug () << "Main thread : " << app.thread();


    qDebug () << "gsgModel* gsg : create (new)";
    gsgModel* gsg = new gsgModel(); // Создали модель класса gsg, для управления и обновления объектов в QML
    qDebug () << "gsgModel* gsg = " << sizeof(gsg);

    qDebug () << "N6700Model* n6700_1 : create (new)";
    N6700Model* n6700_1 = new N6700Model(); // Создали модель класса n6700, для управления и обновления объектов в QML
    qDebug () << "N6700Model* n6700_1 = " << sizeof(n6700_1);

    qDebug () << "N6700Model* n6700_2 : create (new)";
    N6700Model* n6700_2 = new N6700Model(); // Создали модель класса n6700, для управления и обновления объектов в QML
    qDebug () << "N6700Model* n6700_2 = " << sizeof(n6700_2);

    qDebug () << "OsuilografModel* os : create (new)";
    OsuilografModel* os = new OsuilografModel(); // Создали модель класса n6700, для управления и обновления объектов в QML
    qDebug () << "OsuilografModel* os  = " << sizeof(os);

    qDebug () << "TP8* tp : create (new)";
    TP8* tp = new TP8();
    qDebug () << "TP8* tp = " << sizeof(tp);

    qDebug () << "PortModel* port1 : create (new)";
    PortModel* port1 = new PortModel(); // Создали модель класса port, для управления и обновления объектов в QML
    qDebug () << "PortModel* port1 = " << sizeof(port1);

    qDebug () << "PortModel* port2 : create (new)";
    PortModel* port2 = new PortModel(); // Создали модель класса port, для управления и обновления объектов в QML
    qDebug () << "PortModel* port2 = " << sizeof(port2);

    qDebug () << "PortModel* port3 : create (new)";
    PortModel* port3 = new PortModel(); // Создали модель класса port, для управления и обновления объектов в QML
    qDebug () << "PortModel* port3 = " << sizeof(port3);

    qDebug () << "PortModel* port4 : create (new)";
    PortModel* port4 = new PortModel(); // Создали модель класса port, для управления и обновления объектов в QML
    qDebug () << "PortModel* port4 = " << sizeof(port4);

    qDebug () << "PortModel* port5 : create (new)";
    PortModel* port5 = new PortModel(); // Создали модель класса port, для управления и обновления объектов в QML
    qDebug () << "PortModel* port5 = " << sizeof(port5);

    qDebug () << "PortModel* port6 : create (new)";
    PortModel* port6 = new PortModel(); // Создали модель класса port, для управления и обновления объектов в QML
    qDebug () << "PortModel* port6 = " << sizeof(port6);

    qDebug () << "PortModel* port7 : create (new)";
    PortModel* port7 = new PortModel(); // Создали модель класса port, для управления и обновления объектов в QML
    qDebug () << "PortModel* port7 = " << sizeof(port7);

    qDebug () << "PortModel* port8 : create (new)";
    PortModel* port8 = new PortModel(); // Создали модель класса port, для управления и обновления объектов в QML
    qDebug () << "PortModel* port8 = " << sizeof(port8);

    //БД
    qDebug () << "BData* BD : create (new)";
    BData* BD = new BData(); // Создали класс для работы с базой данных
    qDebug () << "BData* BD = " << sizeof(BD);

    BD->moveToThread(new QThread()); //Поместили класс для работы с БД в новый поток.
    qDebug () << "Помещаем класс  << BData >> в поток: " << BD->thread();

    BD->thread()->start(); // Запускаем поток класса для работы с БД
    qDebug () << "Запускаем поток << BData >> : " << BD->thread();

    BD->connect(); // Подключаемся к БД

    gsg->SetBD(BD); // Передаем указатель на класс БД для работы с ним из класса GSG для того чтоб во время проверок записывать для каждого.
    gsg->Sed_TP_OS(os,tp); // Передаем указатель на класс Осцилографа и Пульта для использования его в проверках.


    QQmlApplicationEngine engine;

    screenCapture screenClass(&engine);



    CamberModel* camber = new CamberModel(); //Создание объекта для управления камерой






    qDebug () << "Proverka* proverka : create (new)";
    Proverka* proverka = new Proverka(camber,gsg,n6700_1,port1,port2,port3,port4,port5,port6,port7,port8,&screenClass); // Создали модель класса port, для управления и обновления объектов в QML
    qDebug () << "Proverka* proverka = " << sizeof(proverka);




    engine.rootContext()->setContextProperty("screenObject", &screenClass);

    proverka->SetBD(BD);


    Report* pdf = new Report(gsg); // Создаем класс для создания Отчетности.




    engine.rootContext()->setContextProperty("camber", camber);
    engine.rootContext()->setContextProperty("pdf", pdf);

    engine.rootContext()->setContextProperty("gsg", gsg);
    engine.rootContext()->setContextProperty("n6700_1", n6700_1);
    engine.rootContext()->setContextProperty("n6700_2", n6700_2);
    engine.rootContext()->setContextProperty("os", os);
    engine.rootContext()->setContextProperty("tp", tp);

    engine.rootContext()->setContextProperty("port1", port1);
    engine.rootContext()->setContextProperty("port2", port2);
    engine.rootContext()->setContextProperty("port3", port3);
    engine.rootContext()->setContextProperty("port4", port4);
    engine.rootContext()->setContextProperty("port5", port5);
    engine.rootContext()->setContextProperty("port6", port6);
    engine.rootContext()->setContextProperty("port7", port7);
    engine.rootContext()->setContextProperty("port8", port8);

    engine.rootContext()->setContextProperty("find", find);

    engine.rootContext()->setContextProperty("proverka", proverka);

    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    auto exit = app.exec();

    if(camber->slot_TestConnect())
    {
        camber->slot_stopCamberWork();

        Sleep(1000);
    }


    return exit;
}
