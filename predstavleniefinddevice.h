#ifndef PREDSTAVLENIEFINDDEVICE_H
#define PREDSTAVLENIEFINDDEVICE_H

#include <QObject>


#include <finddevice.h>

class PredstavlenieFindDevice : public QObject
{
    Q_OBJECT

public:
    PredstavlenieFindDevice();

    Q_PROPERTY(QList<QObject*> listDevice READ listDevice NOTIFY ListDeviceChanged)

    const QList<QObject*> listDevice() const;

    QList<QObject*> res;


signals:
    void ListDeviceChanged(QList<QObject*>);
    void findDevice();

public slots:
    void GetListDeviceChanged(QList<QObject*>*);
    void clearList();
    void addList(QString,QString);
    void Work();


public:
  QList<QObject*> m_FindDeviceItem;
};

#endif // PREDSTAVLENIEFINDDEVICE_H
