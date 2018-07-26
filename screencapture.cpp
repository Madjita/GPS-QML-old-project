#include <QPixmap>
#include <QQuickView>
#include <QString>
#include <QDateTime>

#include "screenCapture.h"

screenCapture::screenCapture(QQmlApplicationEngine *currentView) :
    QObject(0), currentView_(currentView)
{
}


void screenCapture::capture(QString const &path) const
{
//    QImage img = currentView_->grabWindow();


//    QString data = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss");
//    QString name = "screen_result_"+data+".jpg";


//    img.save(name,"jpg");


        foreach(auto obj, currentView_->rootObjects()) {
                QImage screen = qobject_cast<QQuickWindow*>(obj)->grabWindow();
                screen.save("C:\\Users\\4230\\Videos\\Скриншоты\\Скриншоты\\" +path);
            }

}

