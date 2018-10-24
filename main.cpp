//#include <QGuiApplication>

#include <QQmlApplicationEngine>


#include <QQuickWindow>

#include <QQmlContext>
#include <QApplication>


#include <gsgmodel.h>
#include <n6700model.h>
#include <portmodel.h>
#include <osuilografmodel.h>
#include <tp8model.h>
//////библиотека Управления камерой////////////
#include <cambermodel.h>
#include <proverkamodel.h>
#include <algorithmmodel.h>
#include <clockcircle.h>
#include <finddevicemodel.h>

#include <report.h>

#include <QQmlProperty>
#include <QQmlComponent>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    // Qt Charts uses Qt Graphics View Framework for drawing, therefore QApplication must be used.
    QApplication app(argc, argv);
    app.setApplicationVersion("1.0");
    app.setOrganizationName("NPP Radio");



    N6700Model* n6700_1 = nullptr;
    N6700Model* n6700_2 = nullptr;
    CamberModel* camber = nullptr;
    gsgModel* gsg = nullptr;
    OsuilografModel* os = nullptr;
    tp8Model* tp = nullptr;

    PortModel* port1 = nullptr;
    PortModel* port2 = nullptr;
    PortModel* port3 = nullptr;
    PortModel* port4 = nullptr;
    PortModel* port5 = nullptr;
    PortModel* port6 = nullptr;
    PortModel* port7 = nullptr;
    PortModel* port8 = nullptr;

    ClockCircle* clock = nullptr;
    AlgorithmModel* alg = nullptr;
    FindDeviceModel* find = nullptr;
    Report* pdf = nullptr;

    find = new FindDeviceModel;

    n6700_1 = new N6700Model;
    n6700_2 = new N6700Model;

    camber = new CamberModel;

    gsg = new gsgModel;
    os = new OsuilografModel;

    port1 = new PortModel;
    port2 = new PortModel;
    port3 = new PortModel;
    port4 = new PortModel;
    port5 = new PortModel;
    port6 = new PortModel;
    port7 = new PortModel;
    port8 = new PortModel;



   // find->work();


    // Всё, что требуется в данном файле - это зарегистрировать новый класс (Тип объекта) для QML слоя
    qmlRegisterType<ClockCircle>("ClockCircle",1,0,"ClockCircle");

    alg = new AlgorithmModel;
    pdf = new Report(alg->alg);

    tp = new tp8Model;


    QQmlApplicationEngine engine;

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

    engine.rootContext()->setContextProperty("proverka", alg);


    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    QObject *rootObject = engine.rootObjects().first();
    QObject *qmlObject = rootObject->findChild<QObject*>("MyClock");

    //qDebug() << "Property value:" << QQmlProperty::read(qmlObject, "name");


    clock = dynamic_cast<ClockCircle*>(qmlObject);



    alg->setDevice(camber,gsg,n6700_1,n6700_2,os,tp,port1,port2,port3,port4,port5,port6,port7,port8,clock);


    if (engine.rootObjects().isEmpty())
        return -1;


    auto ex = app.exec();


    delete tp;
    delete find;
    delete alg;
    delete gsg;
    delete n6700_1;
    delete n6700_2;
    delete os;

    delete port1;
    delete port2;
    delete port3;
    delete port4;
    delete port5;
    delete port6;
    delete port7;
    delete port8;

    delete camber;


    return ex;
}
