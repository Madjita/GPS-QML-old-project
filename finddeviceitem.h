#ifndef FINDDEVICEITEM_H
#define FINDDEVICEITEM_H

#include <QObject>

// Класс для отображения того какие устройства найденны в сети
class FindDeviceItem  : public QObject
{
    Q_OBJECT

public:
    //Коструктор
    explicit FindDeviceItem(QString content,QString peerName, QObject *parent = 0);

    //Найденные устройства названия
    Q_PROPERTY(QString content READ content NOTIFY contentChanged)
    const QString & content() const;
    void setContent(const QString & content);

    // Ip адрес найденного устрйоства
    Q_PROPERTY(QString ip READ ip NOTIFY ipChanged)
    const QString & ip() const;
    void setIp(const QString & ip);


signals:
     void contentChanged();
     void ipChanged();

private:
    QString m_content;
    QString m_ip;
    bool m_connect;
};

#endif // FINDDEVICEITEM_H
