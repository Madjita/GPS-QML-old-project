#include "algorithmmodel.h"

AlgorithmModel::AlgorithmModel(QObject *parent):
    QObject(parent)
    //m_ListProverkaItem(new QList<QObject*>)
{

    alg = new Algorithm;

    alg->mainThread = this->thread();


    connect(this,&AlgorithmModel::signal_StartProverka,alg,&Algorithm::slot_StartProverka);
    connect(alg,&Algorithm::listProverkaItemChanged,this,&AlgorithmModel::GetListProverkaChanged);

    qRegisterMetaType<QList<QObject*>*>("QList<QObject*>*");
}

AlgorithmModel::~AlgorithmModel()
{
    delete alg;
}

const QList<QObject *> AlgorithmModel::listProverka() const
{
    return m_ListProverkaItem;
}

const QString &AlgorithmModel::modeStart() const
{
    return m_modeStart;
}

void AlgorithmModel::setModeStart(const QString &mode)
{
    if (m_modeStart != mode)
    {
        m_modeStart = mode;
        if(alg != nullptr)
        {
            alg->m_modeStart  = m_modeStart;
        }
        emit signal_modeStartChanche(m_modeStart);
    }
}

const QString &AlgorithmModel::fioStart() const
{
    return m_fioStart;
}

void AlgorithmModel::setFioStart(const QString &fio)
{
    if (m_fioStart != fio)
    {
        m_fioStart = fio;
        if(alg != nullptr)
        {
            alg->m_fio  = m_fioStart;
        }
        emit signal_fioStartChanche(m_fioStart);
    }
}

int AlgorithmModel::seTnamberProverkaStart() const
{
    return namberProverkaStart;
}

void AlgorithmModel::namberProverkaStart_write(const int &i)
{
    if(namberProverkaStart != i)
    {
        namberProverkaStart = i;
        alg->namberProverkaStart = namberProverkaStart;
        emit namberProverkaStart_write(namberProverkaStart);
    }
}

int AlgorithmModel::setNamberModeCamber() const
{
    return namberModeCamber;
}

void AlgorithmModel::getNamberModeCamber(const int &i)
{
    namberModeCamber = i;
    alg->namberModeCamber = namberModeCamber;
    emit signal_NamberModeCamberChanche(namberModeCamber);
}

bool AlgorithmModel::setautoProverka() const
{
    return autoProverka;
}

void AlgorithmModel::getautoProverka(const bool &i)
{
    autoProverka = i;
    alg->autoProverka = i;
    emit signal_autoProverkaChanche(autoProverka);
}

void AlgorithmModel::setDevice(CamberModel *_camber, gsgModel *_gsg, N6700Model *_n6700_1, N6700Model *_n6700_2,OsuilografModel* _os,tp8Model* _tp, PortModel *_port1, PortModel *_port2, PortModel *_port3, PortModel *_port4, PortModel *_port5, PortModel *_port6, PortModel *_port7, PortModel *_port8,ClockCircle *_clock)
{
    alg->setDevice(_camber,_gsg,_n6700_1,_n6700_2,_os,_tp,_port1,_port2,_port3,_port4,_port5,_port6,_port7,_port8);

    clock = _clock;

    connect(_tp,&tp8Model::signal_comandaClear_ok,this,&AlgorithmModel::clearWait);
    connect(_tp->tp,&TP8::signal_comanda_ok,alg,&Algorithm::slot_comand_ok);

    connect(alg,&Algorithm::startTimerMrk,_port1->PortNew,&Port::Work);
    connect(alg,&Algorithm::startTimerMrk,_port2->PortNew,&Port::Work);
    connect(alg,&Algorithm::startTimerMrk,_port3->PortNew,&Port::Work);
    connect(alg,&Algorithm::startTimerMrk,_port4->PortNew,&Port::Work);
    connect(alg,&Algorithm::startTimerMrk,_port5->PortNew,&Port::Work);
    connect(alg,&Algorithm::startTimerMrk,_port6->PortNew,&Port::Work);
    connect(alg,&Algorithm::startTimerMrk,_port7->PortNew,&Port::Work);
    connect(alg,&Algorithm::startTimerMrk,_port8->PortNew,&Port::Work);


    connect(_port1,&PortModel::mrkLoadChanged,this,&AlgorithmModel::clearWait_loadMRK);
    connect(_port2,&PortModel::mrkLoadChanged,this,&AlgorithmModel::clearWait_loadMRK);
    connect(_port3,&PortModel::mrkLoadChanged,this,&AlgorithmModel::clearWait_loadMRK);
    connect(_port4,&PortModel::mrkLoadChanged,this,&AlgorithmModel::clearWait_loadMRK);
    connect(_port5,&PortModel::mrkLoadChanged,this,&AlgorithmModel::clearWait_loadMRK);
    connect(_port6,&PortModel::mrkLoadChanged,this,&AlgorithmModel::clearWait_loadMRK);
    connect(_port7,&PortModel::mrkLoadChanged,this,&AlgorithmModel::clearWait_loadMRK);
    connect(_port8,&PortModel::mrkLoadChanged,this,&AlgorithmModel::clearWait_loadMRK);


    connect(_port1,&PortModel::sendNameMRK,this,&AlgorithmModel::clearWait);
    connect(_port2,&PortModel::sendNameMRK,this,&AlgorithmModel::clearWait);
    connect(_port3,&PortModel::sendNameMRK,this,&AlgorithmModel::clearWait);
    connect(_port4,&PortModel::sendNameMRK,this,&AlgorithmModel::clearWait);
    connect(_port5,&PortModel::sendNameMRK,this,&AlgorithmModel::clearWait);
    connect(_port6,&PortModel::sendNameMRK,this,&AlgorithmModel::clearWait);
    connect(_port7,&PortModel::sendNameMRK,this,&AlgorithmModel::clearWait);
    connect(_port8,&PortModel::sendNameMRK,this,&AlgorithmModel::clearWait);

    connect(_camber,&CamberModel::signal_startTimer,_clock,&ClockCircle::start);
    connect(alg,&Algorithm::signal_stopCamberWorkProverka,_camber,&CamberModel::slot_stopCamberWork);

    connect(alg,&Algorithm::setPPS,_gsg,&gsgModel::slot_setPPSOUTput);
    connect(alg,&Algorithm::setSIGNALtype,_gsg,&gsgModel::setSIGNALtype);
    connect(alg,&Algorithm::signal_StartGSG,_gsg,&gsgModel::slot_StartTimer);
    connect(_gsg,&gsgModel::signal_getCONTrolQML,this,&AlgorithmModel::clearWait);
    connect(alg,&Algorithm::signal_Gen_setSATid,_gsg,&gsgModel::slot_Gen_setSATid);
    connect(alg,&Algorithm::signal_Gen_setCONTrol,_gsg,&gsgModel::slot_Gen_setCONTrol);





    //временно пока нет камеры

    connect(alg,&Algorithm::signal_NextCamberTemprature,this,&AlgorithmModel::slot_StartProverka);

    connect(alg,&Algorithm::endOneProverka,this,&AlgorithmModel::endOneProverka);
    connect(alg,&Algorithm::signal_Finish,this,&AlgorithmModel::signal_Finish);
    connect(alg,&Algorithm::qml_text,this,&AlgorithmModel::qml_text);
    connect(alg,&Algorithm::qml_good,this,&AlgorithmModel::qml_good);
    connect(alg,&Algorithm::signal_clearView,this,&AlgorithmModel::signal_clearView);

    //    connect(alg,&Algorithm::signal_startCold,this,&AlgorithmModel::slot_StartProverka);
    //    connect(alg,&Algorithm::signal_startNY,this,&AlgorithmModel::slot_StartProverka);
    //    connect(alg,&Algorithm::signal_startHord,this,&AlgorithmModel::slot_StartProverka);



    if(_n6700_1 != nullptr)
        connect(alg,&Algorithm::startTimer,_n6700_1,&N6700Model::slot_StartTimer);

    if(_n6700_2 != nullptr)
        connect(alg,&Algorithm::startTimer,_n6700_2,&N6700Model::slot_StartTimer);


    connect(_clock,&ClockCircle::signal_Finish,this,&AlgorithmModel::clearWait);


}


void AlgorithmModel::slot_StartProverka()
{
    emit signal_StartProverka();
}

void AlgorithmModel::GetListProverkaChanged(QList<QObject *> *list)
{
    m_ListProverkaItem.clear();

    for(int i=0;i<list->count();i++)
    {
        m_ListProverkaItem.append(list->value(i));

    }
    emit ListProverkaChanged(m_ListProverkaItem);
}

void AlgorithmModel::clearWait()
{
    qDebug() << "Clear ALG : " << alg->sem.available();
    alg->sem.release();
}

void AlgorithmModel::clearWait_loadMRK(bool load)
{

    if(load)
    {
        alg->sem.release();
    }
    else
    {
        //дописать ошибку
        alg->sem.release();
    }
}
