#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
#include <QThread>
#include <QSemaphore>

class Mythread : public QObject
{
    Q_OBJECT
public:
    explicit Mythread(QObject *parent = 0);


    QSemaphore sem;


signals:

public slots:
};

#endif // MYTHREAD_H
