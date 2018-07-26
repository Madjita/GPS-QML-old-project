#ifndef SCREENCAPTURE_H
#define SCREENCAPTURE_H

#include <QObject>
#include <QQmlApplicationEngine>

class QString;
class QQuickView;

class screenCapture : public QObject
{
    Q_OBJECT
public:
    explicit screenCapture(QQmlApplicationEngine *parent = 0);

public slots:
    void capture(QString const &path) const;


private:
    QQmlApplicationEngine *currentView_;
};

#endif // SCREENCAPTURE_H
