#ifndef REPORT_H
#define REPORT_H

#include <QObject>
#include <gsgmodel.h>

class Report : public QObject
{
    Q_OBJECT
public:
    explicit Report(gsgModel* GetGSG,QObject *parent = 0);


    gsgModel* gsg;

    QString start_proverka;
    QString stay_proverka;
    QString stay_liter_proverka;
    QString end_proverka;

    QString Id_Device_find;
    QString Id_Proverki_find;

    QStringList list_id_Proverki_Find; // лись найденных Id проверок
    QStringList list_id_Device_Find;
    QStringList list_id_Proverki_Find_Name;
    QStringList list_id_Proverki_Find_Name_Sootvetstvie;
    QStringList list_id_Proverki_Find_Liter_Sootvetstvie;

    QStringList NP;

signals:

public slots:

     void slot_FindData();


    void slot_CreatePDF();
};

#endif // REPORT_H
