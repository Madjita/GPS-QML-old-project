#include "proverkaitem.h"



proverkaItem::proverkaItem(int indexGet,int _indexList,N6700Model* n6700,
                           gsgModel* gsgModelGet,OsuilografModel* _os, tp8Model* _tp,PortModel* port,
                           bool vnytrenGenGet, bool goodGet,QString GetName, QObject *parent):
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
    indexList(_indexList),
    m_tick("Ждем запуска приемника"),
    m_indexProverki("0")
{

    index = indexGet;
    emit indexNPChanged(QString::number(index));
    n6700model = n6700;
    os = _os;
    tp = _tp;

    relizProverka = new RelizProverka(indexGet,_indexList,n6700,gsgModelGet,os,tp,port,GetName);

    relizProverkaTimer = new relizproverkaTimer(indexGet,n6700,gsgModelGet,port,relizProverka);


    for(int i=0;i < 24;i++)
    {
        m_listSpytnik.append("0");
    }

    emit signal_listSpytnikChanged(m_listSpytnik);



    connect(gsgModelGet,&gsgModel::countSpytnik_R,this,&proverkaItem::getR);
    connect(gsgModelGet,&gsgModel::countSpytnik_G,this,&proverkaItem::getG);

    connect(n6700->n6700,&N6700::getMeasureCURRentSignal,this,&proverkaItem::slot_getMeasureCURRentSignal);
    connect(n6700->n6700,&N6700::getMeasureVoltSignal,this,&proverkaItem::slot_getMeasureVoltSignal);


    connect(port,&PortModel::UpdateCountFindQML,this,&proverkaItem::slot_getUpdateCountFindQML);

    connect(port->PortNew,&Port::signal_GoTORelizproverka,this,&proverkaItem::slot_setListSpytnik);


    connect(relizProverkaTimer,&relizproverkaTimer::signal_Tick,this,&proverkaItem::slot_Tick);

    connect(relizProverka,&RelizProverka::signal_qmlText,this,&proverkaItem::slot_Tick);

    connect(relizProverka,&RelizProverka::startWork,relizProverkaTimer,&relizproverkaTimer::Work);

    connect(relizProverka,&RelizProverka::startWork_liters,relizProverkaTimer,&relizproverkaTimer::Work_liters);
    connect(relizProverka,&RelizProverka::startWork_Os,relizProverkaTimer,&relizproverkaTimer::Work_Os);

    connect(relizProverka,&RelizProverka::signal_StopGSG,gsgModelGet,&gsgModel::slot_StopTimer);

    connect(relizProverkaTimer,&relizproverkaTimer::signal_GoodQML,this,&proverkaItem::slot_Good);


    connect(relizProverka,&RelizProverka::signal_IndexProverka,this,&proverkaItem::slot_IndexProverki);

    connect(this,&proverkaItem::powerChanged,relizProverkaTimer,&relizproverkaTimer::slot_Power);


    connect(relizProverkaTimer,&relizproverkaTimer::signal_GetMrk_OT,port->PortNew,&Port::GetMrk_OT);
    connect(relizProverkaTimer,&relizproverkaTimer::signal_GetMrk_liters,port->PortNew,&Port::GetMrk_liters);

    auto lambda = [&](const QString indexPr) {
        switch (indexPr.toInt()) {
        case 1: setProverkaName("Проверка работоспособности приемника");break;
            case 2: setProverkaName("Проверка работоспособности приемника на воздействие пониженного напряжения питания");break;
            case 3: setProverkaName("Проверка работоспособности приемника на воздействие повышенного напряжения питания");break;
            case 4: setProverkaName("Проверка мощности, потребляемой приемником от источника постоянного тока");break;
            case 5: setProverkaName("Проверка поиска, захвата и сопровождения сигнала с уровнем мощности минус 160 дБВт(-130 dBm)");break;
            case 6: setProverkaName("Проверка работы приемника от внешнего опорного генератора");break;
            case 7: setProverkaName("Проверка напряжения сигнала выходной частоты 10 МГц");break;
            case 8: setProverkaName("Проверка амплитуды выходного сигнала секундной метки");break;
            case 9: setProverkaName("Проверка напряжения питания МШУ");break;
            case 10: setProverkaName("Проверка смещения секундной метки");break;
        }
        emit signal_StartProverkaIndex(indexPr);
       };

    connect(relizProverka,&RelizProverka::signal_StartProverkaIndex,lambda);


}

proverkaItem::~proverkaItem()
{
    qDebug() << "DELETE proverkaItem";
    delete relizProverkaTimer;
    delete  relizProverka;
}

const QString &proverkaItem::indexNP()
{
    m_indexNP = QString::number(index);
    return m_indexNP;
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
    get_tok.clear();

    for(int i=0; i < list.count();i++)
    {
        get_tok.append(list[i].split("\n").first());
    }
}

void proverkaItem::slot_getMeasureVoltSignal(QVector<QString> list)
{

    get_volt.clear();
    get_p.clear();

    for(int i=0; i < list.count();i++)
    {
        get_volt.append(list[i].split("\n").first());

        get_p.append(QString::number(get_tok.value(i).toDouble() * get_volt.value(i).toDouble()));
    }


    switch (index) {
    case 1:

        m_tok = QString::number(get_tok.value(0).toDouble(),NULL,3);
        m_v = QString::number(get_volt.value(0).toDouble(),NULL,3);
        m_power = QString::number(get_p.value(0).toDouble(),NULL,3);
        break;
    case 2:
        m_tok = QString::number(get_tok.value(1).toDouble(),NULL,3);
        m_v = QString::number(get_volt.value(1).toDouble(),NULL,3);
        m_power = QString::number(get_p.value(1).toDouble(),NULL,3);
        break;
    case 3:
        m_tok = QString::number(get_tok.value(2).toDouble(),NULL,3);
        m_v = QString::number(get_volt.value(2).toDouble(),NULL,3);
        m_power = QString::number(get_p.value(2).toDouble(),NULL,3);
        break;
    case 4:
        m_tok = QString::number(get_tok.value(3).toDouble(),NULL,3);
        m_v = QString::number(get_volt.value(3).toDouble(),NULL,3);
        m_power = QString::number(get_p.value(3).toDouble(),NULL,3);
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

const QString &proverkaItem::proverkaName() const
{
    return m_proverkaName;
}

void proverkaItem::setProverkaName(const QString &proverkaName)
{

    if(proverkaName != m_proverkaName)
    {
        m_proverkaName = proverkaName;
        emit signal_proverkaNameChanged(m_proverkaName);
    }

      qDebug() << m_proverkaName;
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

int proverkaItem::slot_IndexNp()
{
    return index;
}





void proverkaItem::slot_setListSpytnik(QStringList listSp,QStringList listSp_Amplitude,QStringList listSp_Name)
{

    list.clear();

    foreach(QString s, listSp)
    {
        list << s;
    }


    setListSpytnik(list);


    list.clear();

    foreach(QString s, listSp_Amplitude)
    {
        list << s;
    }

    setlistSpytnik_Amplitude(list);

    list.clear();

    foreach(QString s, listSp_Name)
    {
        list << s;
    }

    setlistSpytnik_Name(list);

    list.clear();


}
