#ifndef N6700MODEL_H
#define N6700MODEL_H

#include <QObject>
#include <QtCore>
#include <QDebug>

#include <n6700.h>

struct ItemIstochnik
{
    QString tok;
    QString v;
    QString p;
};


class N6700Model : public QObject
{
    Q_OBJECT
public:
    explicit N6700Model(QObject *parent = 0);

    N6700* n6700;


    ItemIstochnik* item1;
    ItemIstochnik* item2;
    ItemIstochnik* item3;
    ItemIstochnik* item4;


    Q_PROPERTY(QString name READ name)

    const QString & name() const;


signals:
    void connectDevice(QString);
    void DisConnect();
    void endWork();
    void Work();
    void startTimer();
    void stopTimer();

    //сигналы от модели до QML
    void getMeasureCURRentSignal(QVector<QString>);
    void getMeasureVoltSignal(QVector<QString>);

    void setOut(QString,bool);
    void setVolt(QString canal, QString V);


    void signal_Ok(bool good);

public slots:
    void slot_connectDevice(QString);
    bool slot_connectOk();
    void slot_DisConnect();
    void slot_Work();
    void slot_StartTimer();
    void slot_getMeasureCURRentSignal(QVector<QString>);
    void slot_getMeasureVoltSignal(QVector<QString>);
    void slot_StopTimer();


};

#endif // N6700MODEL_H
