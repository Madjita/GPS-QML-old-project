#ifndef REPORT_H
#define REPORT_H

#include <QObject>
#include <algorithm.h>

#include <QAxObject>
#include <QAxWidget>
#include <QAxBase>

class Report : public QObject
{
    Q_OBJECT

public:
    explicit Report(Algorithm* _alg,QObject *parent = nullptr);

    //Имя спутника
    Q_PROPERTY(QStringList  listSelect READ listSelect NOTIFY signal_listSelectChanged)
    const QStringList & listSelect() const;
    void SetlistSelect(const QStringList & listSelect);


    Algorithm* alg;

    QString start_proverka;
    QString stay_proverka;
    QString stay_liter_proverka;
    QString end_proverka;

    QString Id_Device_find;
    QString Id_Proverki_find;

    QStringList list_id_Proverki_Find; // лись найденных Id проверок
    QStringList list_id_Device_Find;
    QStringList list_id_Proverki_Find_Name;
    QStringList list_id_Proverki_Find_Name_Treb; //требования
    QStringList list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_NY; //регулировка НУ
    QStringList list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_ColD; //регулировка холод
    QStringList list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_ColDNY;  //регулировка НУ после холода
    QStringList list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_Hort;  //регулировка жара
    QStringList list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_HortNY; // регулировка НУ после жары


    //Листы на После тех.Тренировки
    QStringList list_id_Proverki_Find_Name_Sootvetstvie_NextTexTest_NY; //После тех.Тренировки НУ

    //ПСИ
    QStringList list_id_Proverki_Find_Name_Sootvetstvie_PSI_NY; //регулировка НУ
    QStringList list_id_Proverki_Find_Name_Sootvetstvie_PSI_ColD; //регулировка холод
    QStringList list_id_Proverki_Find_Name_Sootvetstvie_PSI_ColDNY;  //регулировка НУ после холода
    QStringList list_id_Proverki_Find_Name_Sootvetstvie_PSI_Hort;  //регулировка жара
    QStringList list_id_Proverki_Find_Name_Sootvetstvie_PSI_HortNY; // регулировка НУ после жары

    //После  непрерывной работы
    QStringList list_id_Proverki_Find_Name_Sootvetstvie_NextNeprWork_NY; //После  непрерывной работы НУ


    //Serial
    QStringList list_id_Serial;
    QStringList list_name_Serial;

    //FIO
    QStringList list_id_FIO;
    QStringList list_name_FIO;

    //Etap
    QStringList list_id_Etap;
    QStringList list_name_Etap;

    //Date
    QStringList list_id_Date;
    QStringList list_start_Date;
    QStringList list_end_Date;


    //Result
    QStringList list_Result_IdProverki;
    QStringList list_Result_NameProverki;
    QStringList list_Result_Treb;
    QStringList list_Result_Sootv;
    QStringList list_Result_DateStayStart;
    QStringList list_Result_DateStayEnd;

    //Liters
    QStringList list_NP;
    QStringList list_NP_Sootv;




    QString IdResult;

    QStringList NP;


    QStringList Select;


signals:
    void signal_list(QStringList lol);
    void signal_listSerial(QStringList lol);
    void signal_listSerialEtap(QStringList lol);
    void signal_listSerialDate(QStringList lol,QStringList end);
    void signal_listResult(QStringList list_Result_IdProverki,QStringList list_Result_NameProverki,QStringList list_Result_Treb,QStringList list_Result_Sootv,QStringList list_Result_DateStayStart,QStringList list_Result_DateStayEnd,QStringList Np,QStringList Np_sootv);


    void signal_listSelectChanged(QStringList);

public slots:

    void slot_FindData();


    void slot_CreatePDF();

    void slot_CreateWord();


    // Запросы к БД

    void slot_GetSerial();
    void slot_GetFIO(QString serial);

    void slot_GetEtap(QString serial,QString FIO);

    void slot_GetEtapData(QString serial,QString FIO,QString Etap);

    void slot_GetResult(QString serial,QString FIO, QString Etap, QString Date);


    void slot_setSelect(QString,int);

};

#endif // REPORT_H
