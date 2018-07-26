#include "proverkaitem.h"



proverkaItem::proverkaItem(int indexGet,N6700Model* n6700,
                           gsgModel* gsgModelGet,PortModel* port,
                           bool vnytrenGenGet, bool goodGet,QString GetName,screenCapture* screenClassGet, QObject *parent):
    QObject(parent),
    m_tok("-"),
    m_v("-"),
    m_power("-"),
    m_gpsGen("-"),
    m_glonassGen("-"),
    m_gpsFind("-"),
    m_glonassFind("-"),
    m_vnytrenGen(true),
    m_good(false),
    m_tick("Ждем запуска приемника"),
    m_indexProverki("0"),
    screenClass(screenClassGet)
{



    index = indexGet;

    n6700model = n6700;

    relizProverka = new RelizProverka(indexGet,n6700model,gsgModelGet,port,GetName,screenClass);

    relizProverkaTimer = new relizproverkaTimer(indexGet,n6700model,gsgModelGet,port,relizProverka);




    for(int i=0;i < 24;i++)
    {
        m_listSpytnik.append("0");
    }

    emit signal_listSpytnikChanged(m_listSpytnik);

    //    switch (index) {
    //    case 1:
    //        m_tok = n6700->item1->tok;
    //        m_v = n6700->item1->v;
    //        m_power = n6700->item1->p;
    //        break;
    //    case 2:
    //        m_tok = n6700->item2->tok;
    //        m_v = n6700->item2->v;
    //        m_power = n6700->item2->p;
    //        break;
    //    case 3:
    //        m_tok = n6700->item3->tok;
    //        m_v = n6700->item3->v;
    //        m_power = n6700->item3->p;
    //        break;
    //    case 4:
    //        m_tok = n6700->item4->tok;
    //        m_v = n6700->item4->v;
    //        m_power = n6700->item4->p;
    //        break;
    //    default:
    //        break;
    //    }

    //    m_gpsGen = QString::number(gsgModelGet->G);
    //    m_glonassGen = QString::number(gsgModelGet->R);
    //    m_gpsFind = QString::number(port->CountFindGPS);
    //    m_glonassFind = QString::number(port->CountFindGLONASS);
    //    m_vnytrenGen = vnytrenGenGet;
    //    m_good = goodGet;

    // QObject::connect(this,&proverkaItem::signal_StartProverka,relizProverka,&RelizProverka::Work,Qt::QueuedConnection);

    QObject::connect(gsgModelGet,&gsgModel::countSpytnik_R,this,&proverkaItem::getR,Qt::QueuedConnection);
    QObject::connect(gsgModelGet,&gsgModel::countSpytnik_G,this,&proverkaItem::getG,Qt::QueuedConnection);

    QObject::connect(n6700,&N6700Model::getMeasureCURRentSignal,this,&proverkaItem::slot_getMeasureCURRentSignal,Qt::QueuedConnection);
    QObject::connect(n6700,&N6700Model::getMeasureVoltSignal,this,&proverkaItem::slot_getMeasureVoltSignal,Qt::QueuedConnection);


    QObject::connect(port,&PortModel::UpdateCountFindQML,this,&proverkaItem::slot_getUpdateCountFindQML,Qt::QueuedConnection);

    QObject::connect(port,&PortModel::signal_setListSpytnik,this,&proverkaItem::slot_setListSpytnik,Qt::QueuedConnection);

    QObject::connect(relizProverkaTimer,&relizproverkaTimer::signal_Tick,this,&proverkaItem::slot_Tick,Qt::QueuedConnection);

    QObject::connect(relizProverka,&RelizProverka::startWork,relizProverkaTimer,&relizproverkaTimer::Work,Qt::QueuedConnection);

    QObject::connect(relizProverka,&RelizProverka::startWork_liters,relizProverkaTimer,&relizproverkaTimer::Work_liters,Qt::QueuedConnection);
    QObject::connect(relizProverka,&RelizProverka::startWork_Os,relizProverkaTimer,&relizproverkaTimer::Work_Os,Qt::QueuedConnection);

    QObject::connect(relizProverka,&RelizProverka::signal_StopGSG,gsgModelGet,&gsgModel::slot_StopTimer,Qt::QueuedConnection);

    QObject::connect(relizProverkaTimer,&relizproverkaTimer::signal_GoodQML,this,&proverkaItem::slot_Good,Qt::QueuedConnection);


    QObject::connect(relizProverka,&RelizProverka::signal_IndexProverka,this,&proverkaItem::slot_IndexProverki,Qt::QueuedConnection);

    QObject::connect(this,&proverkaItem::powerChanged,relizProverkaTimer,&relizproverkaTimer::slot_Power,Qt::QueuedConnection);


    QObject::connect(relizProverkaTimer,&relizproverkaTimer::signal_GetMrk_OT,port->PortNew,&Port::GetMrk_OT);
    QObject::connect(relizProverkaTimer,&relizproverkaTimer::signal_GetMrk_liters,port->PortNew,&Port::GetMrk_liters);
    QObject::connect(relizProverkaTimer,&relizproverkaTimer::signal_GetMrk_liters_2,port->PortNew,&Port::GetMrk_liters_2);

    QObject::connect(relizProverka,&RelizProverka::signal_StartProverkaIndex,this,&proverkaItem::signal_StartProverkaIndex);


}

/////////////////////////////////////////////////////////////////////////////////

void proverkaItem::getR(int R)
{

    m_glonassGen = QString::number(R);

    glonassGenChanged(m_glonassGen);
}

void proverkaItem::getG(int G)
{
    m_gpsGen = QString::number(G);

    gpsGenChanged(m_gpsGen);
}



void proverkaItem::slot_getMeasureCURRentSignal(QVector<QString> list)
{
    n6700model->item1->tok = list[0].split("\n").first();
    n6700model->item2->tok = list[1].split("\n").first();
    n6700model->item3->tok = list[2].split("\n").first();
    n6700model->item4->tok = list[3].split("\n").first();
}

void proverkaItem::slot_getMeasureVoltSignal(QVector<QString> list)
{
    n6700model->item1->v = list[0].split("\n").first();
    n6700model->item2->v = list[1].split("\n").first();
    n6700model->item3->v = list[2].split("\n").first();
    n6700model->item4->v = list[3].split("\n").first();

    n6700model->item1->p = QString::number(n6700model->item1->tok.toDouble()*n6700model->item1->v.toDouble());
    n6700model->item2->p = QString::number(n6700model->item2->tok.toDouble()*n6700model->item2->v.toDouble());
    n6700model->item3->p = QString::number(n6700model->item3->tok.toDouble()*n6700model->item3->v.toDouble());
    n6700model->item4->p = QString::number(n6700model->item4->tok.toDouble()*n6700model->item4->v.toDouble());


    switch (index) {
    case 1:

        m_tok = QString::number(n6700model->item1->tok.toDouble(),NULL,3);
        m_v = QString::number(n6700model->item1->v.toDouble(),NULL,3);
        m_power = QString::number(n6700model->item1->p.toDouble(),NULL,3);
        break;
    case 2:
        m_tok = QString::number(n6700model->item2->tok.toDouble(),NULL,3);
        m_v = QString::number(n6700model->item2->v.toDouble(),NULL,3);
        m_power = QString::number(n6700model->item2->p.toDouble(),NULL,3);
        break;
    case 3:
        m_tok = QString::number(n6700model->item3->tok.toDouble(),NULL,3);
        m_v = QString::number(n6700model->item3->v.toDouble(),NULL,3);
        m_power = QString::number(n6700model->item3->p.toDouble(),NULL,3);
        break;
    case 4:
        m_tok = QString::number(n6700model->item4->tok.toDouble(),NULL,3);
        m_v = QString::number(n6700model->item4->v.toDouble(),NULL,3);
        m_power = QString::number(n6700model->item4->p.toDouble(),NULL,3);
        break;
    default:
        break;
    }

    emit tokChanged(m_tok);
    emit vChanged(m_v);
    emit powerChanged(m_power);

}

void proverkaItem::slot_getUpdateCountFindQML(int GLONASS, int GPS, int ALL)
{

    m_gpsFind = QString::number(GPS);
    m_glonassFind = QString::number(GLONASS);

    gpsFindChanged(m_gpsFind);
    glonassFindChanged(m_glonassFind);
}


////////////////////////////////////////////////////////////////////////////////////
const QString &proverkaItem::tok() const
{
    return m_tok;
}

void proverkaItem::setTok(const QString &tok)
{
    if(tok != m_tok)
    {
        m_tok = tok;
        emit tokChanged(m_tok);
    }
}

///////////////////////////////////////////////////////////////////////////////////
const QString &proverkaItem::v() const
{
    return m_v;
}

void proverkaItem::setV(const QString &v)
{
    if(v != m_v)
    {
        m_v = v;
        emit vChanged(m_v);
    }
}
///////////////////////////////////////////////////////////////////////////////
const QString &proverkaItem::power() const
{
    return m_power;
}

void proverkaItem::setPower(const QString &power)
{
    if(power != m_power)
    {
        m_power = power;
        emit powerChanged(m_power);
    }
}

///////////////////////////////////////////////////////////////////////////////
const QString &proverkaItem::gpsGen() const
{
    return m_gpsGen;
}

void proverkaItem::setGpsGen(const QString &gpsGen)
{
    if(gpsGen != m_gpsGen)
    {
        m_gpsGen = gpsGen;
        emit gpsGenChanged(m_gpsGen);
    }
}

///////////////////////////////////////////////////////////////////////////////
const QString &proverkaItem::glonassGen() const
{
    return m_glonassGen;
}

void proverkaItem::setGlonassGen(const QString &glonassGen)
{
    if(glonassGen != m_glonassGen)
    {
        m_glonassGen = glonassGen;
        emit glonassGenChanged(m_glonassGen);
    }
}
///////////////////////////////////////////////////////////////////////////////
const QString &proverkaItem::gpsFind() const
{
    return m_gpsFind;
}

void proverkaItem::setGpsFind(const QString &gpsFind)
{
    if(gpsFind != m_gpsFind)
    {
        m_gpsFind = gpsFind;
        emit gpsFindChanged(m_gpsFind);
    }
}

///////////////////////////////////////////////////////////////////////////////
const QString &proverkaItem::glonassFind() const
{
    return m_glonassFind;
}

void proverkaItem::setGlonassFind(const QString &glonassFind)
{
    if(glonassFind != m_glonassFind)
    {
        m_glonassFind = glonassFind;
        emit glonassFindChanged(m_glonassFind);
    }
}

///////////////////////////////////////////////////////////////////////////////
const bool &proverkaItem::vnytrenGen() const
{
    return m_vnytrenGen;
}

void proverkaItem::setVnytrenGen(const bool &vnytrenGen)
{
    if(vnytrenGen != m_vnytrenGen)
    {
        m_vnytrenGen = vnytrenGen;
        emit vnytrenGenChanged();
    }
}

///////////////////////////////////////////////////////////////////////////////
const bool &proverkaItem::good() const
{
    return m_good;
}

void proverkaItem::setGood(const bool &good)
{
    if(good != m_good)
    {
        m_good = good;
        emit goodChanged(m_good);
    }
}
///////////////////////////////////////////////////////////////////
const QString &proverkaItem::indexProverki() const
{
    return m_indexProverki;
}

void proverkaItem::setIndexProverki(const QString &indexProverki)
{
    if(indexProverki != m_indexProverki)
    {
        m_indexProverki = indexProverki;
        emit indexProverkiChanged(m_indexProverki);

        qDebug () << "indexPr = " << m_indexProverki;
    }
}

/////////////////////////////////////////////////////////////////////
const QString &proverkaItem::timeTimer() const
{
    return m_tick;
}

void proverkaItem::setTimeTimer(const QString &timeTimer)
{
    if(timeTimer != m_tick)
    {
        m_tick = timeTimer;
        emit signal_Tick(timeTimer);
    }
}
////////////////////////////////////////////////////////////////

const QVariantList &proverkaItem::listSpytnik() const
{

    return m_listSpytnik;
}

void proverkaItem::setListSpytnik(const QVariantList &listSpytnik)
{

    if(listSpytnik != m_listSpytnik)
    {
        m_listSpytnik.clear();

        for(int i=0; i < listSpytnik.count();i++)
        {
              m_listSpytnik.append(listSpytnik.value(i));
        }

        emit signal_listSpytnikChanged(m_listSpytnik);
    }
}

const QVariantList &proverkaItem::listSpytnik_Amplitude() const
{
    return m_listSpytnik_Amplitude;
}

void proverkaItem::setlistSpytnik_Amplitude(const QVariantList &listSpytnik_Amplitude)
{
    if(listSpytnik_Amplitude != m_listSpytnik_Amplitude)
    {
        m_listSpytnik_Amplitude = listSpytnik_Amplitude;

        emit signal_listSpytnik_AmplitudeChanged(m_listSpytnik_Amplitude);
    }
}

const QVariantList &proverkaItem::listSpytnik_Name() const
{
    return m_listSpytnik_Name;
}

void proverkaItem::setlistSpytnik_Name(const QVariantList &listSpytnik_Name)
{
    if(listSpytnik_Name != m_listSpytnik_Name)
    {
        m_listSpytnik_Name = listSpytnik_Name;

        emit signal_listSpytnik_NameChanged(m_listSpytnik_Name);
    }

}

const QVariantList &proverkaItem::listSpytnik_Liters() const
{
    return m_listSpytnik_Liters;
}

void proverkaItem::setlistSpytnik_Liters(const QVariantList &listSpytnik_Liters)
{
    if(listSpytnik_Liters != m_listSpytnik_Liters)
    {
        m_listSpytnik_Liters = listSpytnik_Liters;

        emit signal_listSpytnik_LitersChanged(m_listSpytnik_Liters);
    }
}



///////////////////////////////////////////////////////////////////////////////


void proverkaItem::slot_GetSignalProverka()
{
    emit signal_StartProverka();

}

void proverkaItem::slot_Tick(QString timeTimer)
{
    // qDebug () << "LOl = " << timeTimer;

    setTimeTimer(timeTimer);

}

void proverkaItem::slot_Good(bool good)
{
    setGood(good);
}

void proverkaItem::slot_IndexProverki(QString indexPr)
{

    setIndexProverki(indexPr);
}

QVariantList list;

void proverkaItem::slot_setListSpytnik(QStringList listSp,QStringList listSp_Amplitude,QStringList listSp_Name)
{
    // qDebug() <<"listSP = " << listSp;

    list.clear();

    for(int i =0; i < listSp.count();i++)
    {
        list << listSp[i];
    }

    setListSpytnik(list);

    list.clear();

    for(int i =0; i < listSp_Amplitude.count();i++)
    {
        list << listSp_Amplitude[i];
    }


    setlistSpytnik_Amplitude(list);

    list.clear();

    for(int i =0; i < listSp_Name.count();i++)
    {
        list << listSp_Name[i];
    }

    setlistSpytnik_Name(list);

    list.clear();

}
