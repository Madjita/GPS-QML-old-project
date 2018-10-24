#ifndef FINDDEVICEMODEL_H
#define FINDDEVICEMODEL_H

#include <QObject>

#include <finddevice.h>
#include <finddeviceitem.h>


class FindDeviceModel : public QObject
{
    Q_OBJECT
public:
    explicit FindDeviceModel(QObject *parent = nullptr);
    ~FindDeviceModel();

    Q_PROPERTY(QList<QObject*> listDevice READ listDevice NOTIFY ListDeviceChanged)
    const QList<QObject*> listDevice() const;

    FindDevice* find = nullptr;

    QList<QObject *> res;



signals:
    void ListDeviceChanged(QList<QObject*> );
    void signal_SendPortmapFind();

public slots:
    void addItemList(QString name, QString peerName);
    void clearRes();
    void work();

};

#endif // FINDDEVICEMODEL_H
