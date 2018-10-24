#include "report.h"
#include <QTextDocument>
#include <QFileDialog>
#include <QPrinter>

#include <bdata.h>


/*!
    \brief Конструктор класса Реализации word pdf ПСИ отчета


    В конструкторе инициализируются константные данные для генерации отчета. Таки екак литеры с -6 до +6 и GPS1 до GPS23

    \author Sergey Smoglyuk
    \version 1.0
    \date Август 2018 года
*/
Report::Report(Algorithm* _alg,QObject *parent) :
    QObject(parent),
    alg(_alg)
{

    start_proverka = "25.04.2017  09:33:58";
    end_proverka = "25.04.2017  10:58:00";

    list_id_Device_Find.append("1");
    list_id_Device_Find.append("2");
    list_id_Device_Find.append("3");
    list_id_Device_Find.append("4");
    list_id_Device_Find.append("5");
    list_id_Device_Find.append("6");
    list_id_Device_Find.append("7");
    list_id_Device_Find.append("8");

    NP.append("-6");
    NP.append("-5");
    NP.append("-4");
    NP.append("-3");
    NP.append("-2");
    NP.append("-1");
    NP.append("0");
    NP.append("1");
    NP.append("2");
    NP.append("3");
    NP.append("4");
    NP.append("5");
    NP.append("6");
    NP.append("G1");
    NP.append("G2");
    NP.append("G3");
    NP.append("G4");
    NP.append("G5");
    NP.append("G6");
    NP.append("G7");
    NP.append("G8");
    NP.append("G9");
    NP.append("G10");
    NP.append("G11");
    NP.append("G12");
    NP.append("G13");
    NP.append("G14");
    NP.append("G15");
    NP.append("G16");
    NP.append("G17");
    NP.append("G18");
    NP.append("G19");
    NP.append("G20");
    NP.append("G21");
    NP.append("G22");
    NP.append("G23");



    Select.append("");
    Select.append("");
    Select.append("");
    Select.append("");
    Select.append("");


}

/*!
    \brief Функция выбора спутника


    Функция возвращает "QStringList Select"

    \author Sergey Smoglyuk
    \version 1.0
    \date Август 2018 года
*/
const QStringList &Report::listSelect() const
{
    return Select;
}

/*!
    \brief Функция выбора спутника


    Функция возвращает "QStringList Select"

    \author Sergey Smoglyuk
    \version 1.0
    \date Август 2018 года
    \warning Данная функция не  используется в программе
*/
void Report::SetlistSelect(const QStringList &listSelect)
{
    if(listSelect != Select)
    {
        Select =  listSelect;

        emit signal_listSelectChanged(Select);
    }
}


/*!
    \brief Функция поиска информации в БД


    Функция поиска информации в БД.\n
    Генерация данных из БД для формирования отчета ПСИ.

    \author Sergey Smoglyuk
    \version 1.0
    \date Август 2018 года
    \warning Данная функция не  используется в программе
*/
void Report::slot_FindData()
{
    //поиск id существующих Проверок в Бд
    QSqlQueryModel* SQL_id;

    SQL_id = alg->BD->zaprosQueryModel("SELECT * FROM Proverki");

    list_id_Proverki_Find.clear();
    list_id_Proverki_Find_Name.clear();
    list_id_Proverki_Find_Name_Treb.clear();

    for(int i=0;i< SQL_id->rowCount();i++)
    {
        list_id_Proverki_Find.append(SQL_id->data(SQL_id->index(i,0), Qt::EditRole).toString());
        list_id_Proverki_Find_Name.append(SQL_id->data(SQL_id->index(i,1), Qt::EditRole).toString());
        list_id_Proverki_Find_Name_Treb.append(SQL_id->data(SQL_id->index(i,2), Qt::EditRole).toString());

        qDebug () << list_id_Proverki_Find_Name.value(i);
    }

    list_id_Proverki_Find_Name.append("Дата и время испытаний");
    list_id_Proverki_Find_Name.append("ФИО");



    //Регулировка в НУ
    SQL_id = alg->BD->zaprosQueryModel("SELECT DISTINCT [LinkRezId],[FIO],[Serial],[Etap],MAX(DateStart) AS DateStart,[DateEnd]"
                                       " FROM"
                                       " (SELECT DISTINCT Link_Result.Id AS LinkRezId,[FIO],[Serial],[Etap],[DateStart],[DateEnd]"
                                       " FROM Link_Result"
                                       " Join User ON Link_Result.IdUser = User.Id  "
                                       " JOIN Serial ON Link_Result.IdSerial = Serial.Id "
                                       " JOIN Etap ON Link_Result.IdEtap = Etap.Id"
                                       " JOIN DateStartEnd ON Link_Result.IdDate = DateStartEnd.Id "
                                       " WHere Serial = '"+Select[0]+"' AND IdEtap = 1 AND IdTemp = 1)");


    QString LinkRezId = SQL_id->data(SQL_id->index(0,0), Qt::EditRole).toString();
    QString FIO = SQL_id->data(SQL_id->index(0,1), Qt::EditRole).toString();
    QString DateStart = SQL_id->data(SQL_id->index(0,4), Qt::EditRole).toString();
    QString DateEnd = SQL_id->data(SQL_id->index(0,5), Qt::EditRole).toString();


    qDebug () << "LinkRezId = " << LinkRezId;
    qDebug () << "Select[0] = " << Select[0];
    qDebug () << "FIO = " << FIO;
    qDebug () << "DateStart = " << DateStart;
    qDebug () << "DateEnd = " << DateEnd;

    list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_NY.clear();

    SQL_id = alg->BD->zaprosQueryModel("SELECT DISTINCT [Sootv],[IdProverki],[DateStayEnd],[DateStayStart]"
                                       " FROM Result"
                                       " Join Link_Result ON Link_Result.Id = Result.IdLink "
                                       " WHere IdLink = '"+LinkRezId+"'");

    if(SQL_id->rowCount() < 1)
    {
        for(int i=0; i < list_id_Proverki_Find_Name.count();i++)
        {
            list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_NY.append("-");
        }
    }
    else
    {
        for(int i=0;i< SQL_id->rowCount();i++)
        {
            if(SQL_id->data(SQL_id->index(i,0), Qt::EditRole).toString() == "")
            {
                list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_NY.append("-");
            }
            else
            {
                list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_NY.append(SQL_id->data(SQL_id->index(i,0), Qt::EditRole).toString());
            }

        }

        if(SQL_id->rowCount() <  list_id_Proverki_Find_Name.count()-3)
        {

            for(int i=list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_NY.count()-1;i < list_id_Proverki_Find_Name.count()-3;i++)
            {
                list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_NY.append("-");
            }
        }
    }

    list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_NY.append(DateStart +"\n"+DateEnd);
    list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_NY.append(FIO);

    //Регулировка в Холоде
    SQL_id = alg->BD->zaprosQueryModel("SELECT DISTINCT [LinkRezId],[FIO],[Serial],[Etap],MAX(DateStart) AS DateStart,[DateEnd]"
                                       " FROM"
                                       " (SELECT DISTINCT Link_Result.Id AS LinkRezId,[FIO],[Serial],[Etap],[DateStart],[DateEnd]"
                                       " FROM Link_Result"
                                       " Join User ON Link_Result.IdUser = User.Id  "
                                       " JOIN Serial ON Link_Result.IdSerial = Serial.Id "
                                       " JOIN Etap ON Link_Result.IdEtap = Etap.Id"
                                       " JOIN DateStartEnd ON Link_Result.IdDate = DateStartEnd.Id "
                                       " WHere Serial = '"+Select[0]+"' AND IdEtap = 1 AND IdTemp = 2)");


    LinkRezId = SQL_id->data(SQL_id->index(0,0), Qt::EditRole).toString();
    FIO = SQL_id->data(SQL_id->index(0,1), Qt::EditRole).toString();
    DateStart = SQL_id->data(SQL_id->index(0,4), Qt::EditRole).toString();
    DateEnd = SQL_id->data(SQL_id->index(0,5), Qt::EditRole).toString();



    list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_ColD.clear();

    SQL_id = alg->BD->zaprosQueryModel("SELECT DISTINCT [Sootv],[IdProverki],[DateStayEnd],[DateStayStart]"
                                       " FROM Result"
                                       " Join Link_Result ON Link_Result.Id = Result.IdLink "
                                       " WHere IdLink = '"+LinkRezId+"'");

    if(SQL_id->rowCount() < 1)
    {
        for(int i=0; i < list_id_Proverki_Find_Name.count();i++)
        {
            list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_ColD.append("-");
        }
    }
    else
    {
        for(int i=0;i< SQL_id->rowCount();i++)
        {
            if(SQL_id->data(SQL_id->index(i,0), Qt::EditRole).toString() == "")
            {
                list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_ColD.append("-");
            }
            else
            {
                list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_ColD.append(SQL_id->data(SQL_id->index(i,0), Qt::EditRole).toString());
            }
        }

        if(SQL_id->rowCount() <  list_id_Proverki_Find_Name.count()-3)
        {

            for(int i=list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_ColD.count()-1;i < list_id_Proverki_Find_Name.count()-3;i++)
            {
                list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_ColD.append("-");
            }
        }
    }

    list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_ColD.append(DateStart +"\n"+DateEnd);
    list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_ColD.append(FIO);

    //Регулировка после Холода в НУ
    SQL_id = alg->BD->zaprosQueryModel("SELECT DISTINCT [LinkRezId],[FIO],[Serial],[Etap],MAX(DateStart) AS DateStart,[DateEnd]"
                                       " FROM"
                                       " (SELECT DISTINCT Link_Result.Id AS LinkRezId,[FIO],[Serial],[Etap],[DateStart],[DateEnd]"
                                       " FROM Link_Result"
                                       " Join User ON Link_Result.IdUser = User.Id  "
                                       " JOIN Serial ON Link_Result.IdSerial = Serial.Id "
                                       " JOIN Etap ON Link_Result.IdEtap = Etap.Id"
                                       " JOIN DateStartEnd ON Link_Result.IdDate = DateStartEnd.Id "
                                       " WHere Serial = '"+Select[0]+"' AND IdEtap = 1 AND IdTemp = 3)");


    LinkRezId = SQL_id->data(SQL_id->index(0,0), Qt::EditRole).toString();
    FIO = SQL_id->data(SQL_id->index(0,1), Qt::EditRole).toString();
    DateStart = SQL_id->data(SQL_id->index(0,4), Qt::EditRole).toString();
    DateEnd = SQL_id->data(SQL_id->index(0,5), Qt::EditRole).toString();



    list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_ColDNY.clear();

    SQL_id = alg->BD->zaprosQueryModel("SELECT DISTINCT [Sootv],[IdProverki],[DateStayEnd],[DateStayStart]"
                                       " FROM Result"
                                       " Join Link_Result ON Link_Result.Id = Result.IdLink "
                                       " WHere IdLink = '"+LinkRezId+"'");

    if(SQL_id->rowCount() < 1)
    {
        for(int i=0; i < list_id_Proverki_Find_Name.count();i++)
        {
            list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_ColDNY.append("-");
        }
    }
    else
    {
        for(int i=0;i< SQL_id->rowCount();i++)
        {
            if(SQL_id->data(SQL_id->index(i,0), Qt::EditRole).toString() == "")
            {
                list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_ColDNY.append("-");
            }
            else
            {
               list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_ColDNY.append(SQL_id->data(SQL_id->index(i,0), Qt::EditRole).toString());
            }

        }

        if(SQL_id->rowCount() <  list_id_Proverki_Find_Name.count()-3)
        {

            for(int i=list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_ColDNY.count()-1;i < list_id_Proverki_Find_Name.count()-3;i++)
            {
                list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_ColDNY.append("-");
            }
        }
    }

    list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_ColDNY.append(DateStart +"\n"+DateEnd);
    list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_ColDNY.append(FIO);

    //Регулировка в Тепле
    SQL_id = alg->BD->zaprosQueryModel("SELECT DISTINCT [LinkRezId],[FIO],[Serial],[Etap],MAX(DateStart) AS DateStart,[DateEnd]"
                                       " FROM"
                                       " (SELECT DISTINCT Link_Result.Id AS LinkRezId,[FIO],[Serial],[Etap],[DateStart],[DateEnd]"
                                       " FROM Link_Result"
                                       " Join User ON Link_Result.IdUser = User.Id  "
                                       " JOIN Serial ON Link_Result.IdSerial = Serial.Id "
                                       " JOIN Etap ON Link_Result.IdEtap = Etap.Id"
                                       " JOIN DateStartEnd ON Link_Result.IdDate = DateStartEnd.Id "
                                       " WHere Serial = '"+Select[0]+"' AND IdEtap = 1 AND IdTemp = 4)");


    LinkRezId = SQL_id->data(SQL_id->index(0,0), Qt::EditRole).toString();
    FIO = SQL_id->data(SQL_id->index(0,1), Qt::EditRole).toString();
    DateStart = SQL_id->data(SQL_id->index(0,4), Qt::EditRole).toString();
    DateEnd = SQL_id->data(SQL_id->index(0,5), Qt::EditRole).toString();



    list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_Hort.clear();

    SQL_id = alg->BD->zaprosQueryModel("SELECT DISTINCT [Sootv],[IdProverki],[DateStayEnd],[DateStayStart]"
                                       " FROM Result"
                                       " Join Link_Result ON Link_Result.Id = Result.IdLink "
                                       " WHere IdLink = '"+LinkRezId+"'");
    if(SQL_id->rowCount() < 1)
    {
        for(int i=0; i < list_id_Proverki_Find_Name.count();i++)
        {
            list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_Hort.append("-");
        }
    }
    else
    {
        for(int i=0;i< SQL_id->rowCount();i++)
        {
            if(SQL_id->data(SQL_id->index(i,0), Qt::EditRole).toString() == "")
            {
                list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_Hort.append("-");
            }
            else
            {
                list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_Hort.append(SQL_id->data(SQL_id->index(i,0), Qt::EditRole).toString());
            }
        }

        if(SQL_id->rowCount() <  list_id_Proverki_Find_Name.count()-3)
        {

            for(int i=list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_Hort.count()-1;i < list_id_Proverki_Find_Name.count()-3;i++)
            {
                list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_Hort.append("-");
            }
        }
    }

    list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_Hort.append(DateStart +"\n"+DateEnd);
    list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_Hort.append(FIO);

    //Регулировка после Тепла в НУ
    SQL_id = alg->BD->zaprosQueryModel("SELECT DISTINCT [LinkRezId],[FIO],[Serial],[Etap],MAX(DateStart) AS DateStart,[DateEnd]"
                                       " FROM"
                                       " (SELECT DISTINCT Link_Result.Id AS LinkRezId,[FIO],[Serial],[Etap],[DateStart],[DateEnd]"
                                       " FROM Link_Result"
                                       " Join User ON Link_Result.IdUser = User.Id  "
                                       " JOIN Serial ON Link_Result.IdSerial = Serial.Id "
                                       " JOIN Etap ON Link_Result.IdEtap = Etap.Id"
                                       " JOIN DateStartEnd ON Link_Result.IdDate = DateStartEnd.Id "
                                       " WHere Serial = '"+Select[0]+"' AND IdEtap = 1 AND IdTemp = 5)");


    LinkRezId = SQL_id->data(SQL_id->index(0,0), Qt::EditRole).toString();
    FIO = SQL_id->data(SQL_id->index(0,1), Qt::EditRole).toString();
    DateStart = SQL_id->data(SQL_id->index(0,4), Qt::EditRole).toString();
    DateEnd = SQL_id->data(SQL_id->index(0,5), Qt::EditRole).toString();


    list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_HortNY.clear();

    SQL_id = alg->BD->zaprosQueryModel("SELECT DISTINCT [Sootv],[IdProverki],[DateStayEnd],[DateStayStart]"
                                       " FROM Result"
                                       " Join Link_Result ON Link_Result.Id = Result.IdLink "
                                       " WHere IdLink = '"+LinkRezId+"'");
    if(SQL_id->rowCount() < 1)
    {
        for(int i=0; i < list_id_Proverki_Find_Name.count();i++)
        {
            list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_HortNY.append("-");
        }
    }
    else
    {
        for(int i=0;i< SQL_id->rowCount();i++)
        {
            if(SQL_id->data(SQL_id->index(i,0), Qt::EditRole).toString() == "")
            {
                list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_HortNY.append("-");
            }
            else
            {
                list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_HortNY.append(SQL_id->data(SQL_id->index(i,0), Qt::EditRole).toString());
            }
        }

        if(SQL_id->rowCount() <  list_id_Proverki_Find_Name.count()-3)
        {

            for(int i=list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_HortNY.count()-1;i < list_id_Proverki_Find_Name.count()-3;i++)
            {
                list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_HortNY.append("-");
            }
        }
    }

    list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_HortNY.append(DateStart +"\n"+DateEnd);
    list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_HortNY.append(FIO);

    qDebug () <<"list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_NY = " << list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_NY;
    qDebug () <<"list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_COLD = " << list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_ColD;
    qDebug () <<"list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_ColDNY = " << list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_ColDNY;
    qDebug () <<"list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_Hort = " << list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_Hort;
    qDebug () <<"list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_HortNY = " << list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_HortNY;



    //После тех.Тренировки в НУ
    SQL_id = alg->BD->zaprosQueryModel("SELECT DISTINCT [LinkRezId],[FIO],[Serial],[Etap],MAX(DateStart) AS DateStart,[DateEnd]"
                                       " FROM"
                                       " (SELECT DISTINCT Link_Result.Id AS LinkRezId,[FIO],[Serial],[Etap],[DateStart],[DateEnd]"
                                       " FROM Link_Result"
                                       " Join User ON Link_Result.IdUser = User.Id  "
                                       " JOIN Serial ON Link_Result.IdSerial = Serial.Id "
                                       " JOIN Etap ON Link_Result.IdEtap = Etap.Id"
                                       " JOIN DateStartEnd ON Link_Result.IdDate = DateStartEnd.Id "
                                       " WHere Serial = '"+Select[0]+"' AND IdEtap = 2 AND IdTemp = 1)");


    LinkRezId = SQL_id->data(SQL_id->index(0,0), Qt::EditRole).toString();
    FIO = SQL_id->data(SQL_id->index(0,1), Qt::EditRole).toString();
    DateStart = SQL_id->data(SQL_id->index(0,4), Qt::EditRole).toString();
    DateEnd = SQL_id->data(SQL_id->index(0,5), Qt::EditRole).toString();


    list_id_Proverki_Find_Name_Sootvetstvie_NextTexTest_NY.clear();

    SQL_id = alg->BD->zaprosQueryModel("SELECT DISTINCT [Sootv],[IdProverki],[DateStayEnd],[DateStayStart]"
                                       " FROM Result"
                                       " Join Link_Result ON Link_Result.Id = Result.IdLink "
                                       " WHere IdLink = '"+LinkRezId+"'");
    if(SQL_id->rowCount() < 1)
    {
        for(int i=0; i < list_id_Proverki_Find_Name.count();i++)
        {
            list_id_Proverki_Find_Name_Sootvetstvie_NextTexTest_NY.append("-");
        }
    }
    else
    {
        for(int i=0;i< SQL_id->rowCount();i++)
        {
            if(SQL_id->data(SQL_id->index(i,0), Qt::EditRole).toString() == "")
            {
                list_id_Proverki_Find_Name_Sootvetstvie_NextTexTest_NY.append("-");
            }
            else
            {
                list_id_Proverki_Find_Name_Sootvetstvie_NextTexTest_NY.append(SQL_id->data(SQL_id->index(i,0), Qt::EditRole).toString());
            }
        }

        if(SQL_id->rowCount() <  list_id_Proverki_Find_Name.count()-3)
        {

            for(int i=list_id_Proverki_Find_Name_Sootvetstvie_NextTexTest_NY.count()-1;i < list_id_Proverki_Find_Name.count()-3;i++)
            {
                list_id_Proverki_Find_Name_Sootvetstvie_NextTexTest_NY.append("-");
            }
        }
    }

    list_id_Proverki_Find_Name_Sootvetstvie_NextTexTest_NY.append(DateStart +"\n"+DateEnd);
    list_id_Proverki_Find_Name_Sootvetstvie_NextTexTest_NY.append(FIO);

    //////////////////////////////////////////////////////////////////////////////////////
    /// \brief slot_CreateWord
    //ПСИ

    //ПСИ в НУ
    SQL_id = alg->BD->zaprosQueryModel("SELECT DISTINCT [LinkRezId],[FIO],[Serial],[Etap],MAX(DateStart) AS DateStart,[DateEnd]"
                                       " FROM"
                                       " (SELECT DISTINCT Link_Result.Id AS LinkRezId,[FIO],[Serial],[Etap],[DateStart],[DateEnd]"
                                       " FROM Link_Result"
                                       " Join User ON Link_Result.IdUser = User.Id  "
                                       " JOIN Serial ON Link_Result.IdSerial = Serial.Id "
                                       " JOIN Etap ON Link_Result.IdEtap = Etap.Id"
                                       " JOIN DateStartEnd ON Link_Result.IdDate = DateStartEnd.Id "
                                       " WHere Serial = '"+Select[0]+"' AND IdEtap = 3 AND IdTemp = 1)");


     LinkRezId = SQL_id->data(SQL_id->index(0,0), Qt::EditRole).toString();
     FIO = SQL_id->data(SQL_id->index(0,1), Qt::EditRole).toString();
     DateStart = SQL_id->data(SQL_id->index(0,4), Qt::EditRole).toString();
     DateEnd = SQL_id->data(SQL_id->index(0,5), Qt::EditRole).toString();



    list_id_Proverki_Find_Name_Sootvetstvie_PSI_NY.clear();

    SQL_id = alg->BD->zaprosQueryModel("SELECT DISTINCT [Sootv],[IdProverki],[DateStayEnd],[DateStayStart]"
                                       " FROM Result"
                                       " Join Link_Result ON Link_Result.Id = Result.IdLink "
                                       " WHere IdLink = '"+LinkRezId+"'");

    if(SQL_id->rowCount() < 1)
    {
        for(int i=0; i < list_id_Proverki_Find_Name.count();i++)
        {
            list_id_Proverki_Find_Name_Sootvetstvie_PSI_NY.append("-");
        }
    }
    else
    {
        for(int i=0;i< SQL_id->rowCount();i++)
        {
            if(SQL_id->data(SQL_id->index(i,0), Qt::EditRole).toString() == "")
            {
                list_id_Proverki_Find_Name_Sootvetstvie_PSI_NY.append("-");
            }
            else
            {
                list_id_Proverki_Find_Name_Sootvetstvie_PSI_NY.append(SQL_id->data(SQL_id->index(i,0), Qt::EditRole).toString());
            }

        }

        if(SQL_id->rowCount() <  list_id_Proverki_Find_Name.count()-3)
        {

            for(int i=list_id_Proverki_Find_Name_Sootvetstvie_PSI_NY.count()-1;i < list_id_Proverki_Find_Name.count()-3;i++)
            {
                list_id_Proverki_Find_Name_Sootvetstvie_PSI_NY.append("-");
            }
        }
    }

    list_id_Proverki_Find_Name_Sootvetstvie_PSI_NY.append(DateStart +"\n"+DateEnd);
    list_id_Proverki_Find_Name_Sootvetstvie_PSI_NY.append(FIO);

    //ПСИ в Холоде
    SQL_id = alg->BD->zaprosQueryModel("SELECT DISTINCT [LinkRezId],[FIO],[Serial],[Etap],MAX(DateStart) AS DateStart,[DateEnd]"
                                       " FROM"
                                       " (SELECT DISTINCT Link_Result.Id AS LinkRezId,[FIO],[Serial],[Etap],[DateStart],[DateEnd]"
                                       " FROM Link_Result"
                                       " Join User ON Link_Result.IdUser = User.Id  "
                                       " JOIN Serial ON Link_Result.IdSerial = Serial.Id "
                                       " JOIN Etap ON Link_Result.IdEtap = Etap.Id"
                                       " JOIN DateStartEnd ON Link_Result.IdDate = DateStartEnd.Id "
                                       " WHere Serial = '"+Select[0]+"' AND IdEtap = 3 AND IdTemp = 2)");


    LinkRezId = SQL_id->data(SQL_id->index(0,0), Qt::EditRole).toString();
    FIO = SQL_id->data(SQL_id->index(0,1), Qt::EditRole).toString();
    DateStart = SQL_id->data(SQL_id->index(0,4), Qt::EditRole).toString();
    DateEnd = SQL_id->data(SQL_id->index(0,5), Qt::EditRole).toString();



    list_id_Proverki_Find_Name_Sootvetstvie_PSI_ColD.clear();

    SQL_id = alg->BD->zaprosQueryModel("SELECT DISTINCT [Sootv],[IdProverki],[DateStayEnd],[DateStayStart]"
                                       " FROM Result"
                                       " Join Link_Result ON Link_Result.Id = Result.IdLink "
                                       " WHere IdLink = '"+LinkRezId+"'");

    if(SQL_id->rowCount() < 1)
    {
        for(int i=0; i < list_id_Proverki_Find_Name.count();i++)
        {
            list_id_Proverki_Find_Name_Sootvetstvie_PSI_ColD.append("-");
        }
    }
    else
    {
        for(int i=0;i< SQL_id->rowCount();i++)
        {
            if(SQL_id->data(SQL_id->index(i,0), Qt::EditRole).toString() == "")
            {
                list_id_Proverki_Find_Name_Sootvetstvie_PSI_ColD.append("-");
            }
            else
            {
                list_id_Proverki_Find_Name_Sootvetstvie_PSI_ColD.append(SQL_id->data(SQL_id->index(i,0), Qt::EditRole).toString());
            }
        }

        if(SQL_id->rowCount() <  list_id_Proverki_Find_Name.count()-3)
        {

            for(int i=list_id_Proverki_Find_Name_Sootvetstvie_PSI_ColD.count()-1;i < list_id_Proverki_Find_Name.count()-3;i++)
            {
                list_id_Proverki_Find_Name_Sootvetstvie_PSI_ColD.append("-");
            }
        }
    }

    list_id_Proverki_Find_Name_Sootvetstvie_PSI_ColD.append(DateStart +"\n"+DateEnd);
    list_id_Proverki_Find_Name_Sootvetstvie_PSI_ColD.append(FIO);

    //ПСИ после Холода
    SQL_id = alg->BD->zaprosQueryModel("SELECT DISTINCT [LinkRezId],[FIO],[Serial],[Etap],MAX(DateStart) AS DateStart,[DateEnd]"
                                       " FROM"
                                       " (SELECT DISTINCT Link_Result.Id AS LinkRezId,[FIO],[Serial],[Etap],[DateStart],[DateEnd]"
                                       " FROM Link_Result"
                                       " Join User ON Link_Result.IdUser = User.Id  "
                                       " JOIN Serial ON Link_Result.IdSerial = Serial.Id "
                                       " JOIN Etap ON Link_Result.IdEtap = Etap.Id"
                                       " JOIN DateStartEnd ON Link_Result.IdDate = DateStartEnd.Id "
                                       " WHere Serial = '"+Select[0]+"' AND IdEtap = 3 AND IdTemp = 3)");


    LinkRezId = SQL_id->data(SQL_id->index(0,0), Qt::EditRole).toString();
    FIO = SQL_id->data(SQL_id->index(0,1), Qt::EditRole).toString();
    DateStart = SQL_id->data(SQL_id->index(0,4), Qt::EditRole).toString();
    DateEnd = SQL_id->data(SQL_id->index(0,5), Qt::EditRole).toString();



    list_id_Proverki_Find_Name_Sootvetstvie_PSI_ColDNY.clear();

    SQL_id = alg->BD->zaprosQueryModel("SELECT DISTINCT [Sootv],[IdProverki],[DateStayEnd],[DateStayStart]"
                                       " FROM Result"
                                       " Join Link_Result ON Link_Result.Id = Result.IdLink "
                                       " WHere IdLink = '"+LinkRezId+"'");

    if(SQL_id->rowCount() < 1)
    {
        for(int i=0; i < list_id_Proverki_Find_Name.count();i++)
        {
            list_id_Proverki_Find_Name_Sootvetstvie_PSI_ColDNY.append("-");
        }
    }
    else
    {
        for(int i=0;i< SQL_id->rowCount();i++)
        {
            if(SQL_id->data(SQL_id->index(i,0), Qt::EditRole).toString() == "")
            {
                list_id_Proverki_Find_Name_Sootvetstvie_PSI_ColDNY.append("-");
            }
            else
            {
               list_id_Proverki_Find_Name_Sootvetstvie_PSI_ColDNY.append(SQL_id->data(SQL_id->index(i,0), Qt::EditRole).toString());
            }

        }

        if(SQL_id->rowCount() <  list_id_Proverki_Find_Name.count()-3)
        {

            for(int i=list_id_Proverki_Find_Name_Sootvetstvie_PSI_ColDNY.count()-1;i < list_id_Proverki_Find_Name.count()-3;i++)
            {
                list_id_Proverki_Find_Name_Sootvetstvie_PSI_ColDNY.append("-");
            }
        }
    }

    list_id_Proverki_Find_Name_Sootvetstvie_PSI_ColDNY.append(DateStart +"\n"+DateEnd);
    list_id_Proverki_Find_Name_Sootvetstvie_PSI_ColDNY.append(FIO);

    //ПСИ в Тепле
    SQL_id = alg->BD->zaprosQueryModel("SELECT DISTINCT [LinkRezId],[FIO],[Serial],[Etap],MAX(DateStart) AS DateStart,[DateEnd]"
                                       " FROM"
                                       " (SELECT DISTINCT Link_Result.Id AS LinkRezId,[FIO],[Serial],[Etap],[DateStart],[DateEnd]"
                                       " FROM Link_Result"
                                       " Join User ON Link_Result.IdUser = User.Id  "
                                       " JOIN Serial ON Link_Result.IdSerial = Serial.Id "
                                       " JOIN Etap ON Link_Result.IdEtap = Etap.Id"
                                       " JOIN DateStartEnd ON Link_Result.IdDate = DateStartEnd.Id "
                                       " WHere Serial = '"+Select[0]+"' AND IdEtap = 3 AND IdTemp = 4)");


    LinkRezId = SQL_id->data(SQL_id->index(0,0), Qt::EditRole).toString();
    FIO = SQL_id->data(SQL_id->index(0,1), Qt::EditRole).toString();
    DateStart = SQL_id->data(SQL_id->index(0,4), Qt::EditRole).toString();
    DateEnd = SQL_id->data(SQL_id->index(0,5), Qt::EditRole).toString();



    list_id_Proverki_Find_Name_Sootvetstvie_PSI_Hort.clear();

    SQL_id = alg->BD->zaprosQueryModel("SELECT DISTINCT [Sootv],[IdProverki],[DateStayEnd],[DateStayStart]"
                                       " FROM Result"
                                       " Join Link_Result ON Link_Result.Id = Result.IdLink "
                                       " WHere IdLink = '"+LinkRezId+"'");
    if(SQL_id->rowCount() < 1)
    {
        for(int i=0; i < list_id_Proverki_Find_Name.count();i++)
        {
            list_id_Proverki_Find_Name_Sootvetstvie_PSI_Hort.append("-");
        }
    }
    else
    {
        for(int i=0;i< SQL_id->rowCount();i++)
        {
            if(SQL_id->data(SQL_id->index(i,0), Qt::EditRole).toString() == "")
            {
                list_id_Proverki_Find_Name_Sootvetstvie_PSI_Hort.append("-");
            }
            else
            {
                list_id_Proverki_Find_Name_Sootvetstvie_PSI_Hort.append(SQL_id->data(SQL_id->index(i,0), Qt::EditRole).toString());
            }
        }

        if(SQL_id->rowCount() <  list_id_Proverki_Find_Name.count()-3)
        {

            for(int i=list_id_Proverki_Find_Name_Sootvetstvie_PSI_Hort.count()-1;i < list_id_Proverki_Find_Name.count()-3;i++)
            {
                list_id_Proverki_Find_Name_Sootvetstvie_PSI_Hort.append("-");
            }
        }
    }

    list_id_Proverki_Find_Name_Sootvetstvie_PSI_Hort.append(DateStart +"\n"+DateEnd);
    list_id_Proverki_Find_Name_Sootvetstvie_PSI_Hort.append(FIO);

    //ПСИ после Тепла в НУ
    SQL_id = alg->BD->zaprosQueryModel("SELECT DISTINCT [LinkRezId],[FIO],[Serial],[Etap],MAX(DateStart) AS DateStart,[DateEnd]"
                                       " FROM"
                                       " (SELECT DISTINCT Link_Result.Id AS LinkRezId,[FIO],[Serial],[Etap],[DateStart],[DateEnd]"
                                       " FROM Link_Result"
                                       " Join User ON Link_Result.IdUser = User.Id  "
                                       " JOIN Serial ON Link_Result.IdSerial = Serial.Id "
                                       " JOIN Etap ON Link_Result.IdEtap = Etap.Id"
                                       " JOIN DateStartEnd ON Link_Result.IdDate = DateStartEnd.Id "
                                       " WHere Serial = '"+Select[0]+"' AND IdEtap = 3 AND IdTemp = 5)");


    LinkRezId = SQL_id->data(SQL_id->index(0,0), Qt::EditRole).toString();
    FIO = SQL_id->data(SQL_id->index(0,1), Qt::EditRole).toString();
    DateStart = SQL_id->data(SQL_id->index(0,4), Qt::EditRole).toString();
    DateEnd = SQL_id->data(SQL_id->index(0,5), Qt::EditRole).toString();


    list_id_Proverki_Find_Name_Sootvetstvie_PSI_HortNY.clear();

    SQL_id = alg->BD->zaprosQueryModel("SELECT DISTINCT [Sootv],[IdProverki],[DateStayEnd],[DateStayStart]"
                                       " FROM Result"
                                       " Join Link_Result ON Link_Result.Id = Result.IdLink "
                                       " WHere IdLink = '"+LinkRezId+"'");
    if(SQL_id->rowCount() < 1)
    {
        for(int i=0; i < list_id_Proverki_Find_Name.count();i++)
        {
            list_id_Proverki_Find_Name_Sootvetstvie_PSI_HortNY.append("-");
        }
    }
    else
    {
        for(int i=0;i< SQL_id->rowCount();i++)
        {
            if(SQL_id->data(SQL_id->index(i,0), Qt::EditRole).toString() == "")
            {
                list_id_Proverki_Find_Name_Sootvetstvie_PSI_HortNY.append("-");
            }
            else
            {
                list_id_Proverki_Find_Name_Sootvetstvie_PSI_HortNY.append(SQL_id->data(SQL_id->index(i,0), Qt::EditRole).toString());
            }
        }

        if(SQL_id->rowCount() <  list_id_Proverki_Find_Name.count()-3)
        {

            for(int i=list_id_Proverki_Find_Name_Sootvetstvie_PSI_HortNY.count()-1;i < list_id_Proverki_Find_Name.count()-3;i++)
            {
                list_id_Proverki_Find_Name_Sootvetstvie_PSI_HortNY.append("-");
            }
        }
    }

    list_id_Proverki_Find_Name_Sootvetstvie_PSI_HortNY.append(DateStart +"\n"+DateEnd);
    list_id_Proverki_Find_Name_Sootvetstvie_PSI_HortNY.append(FIO);


    ////////////////////////////////////////////
    /// \brief slot_CreateWord
    ///
    ///
    //После неприрывной работы
    SQL_id = alg->BD->zaprosQueryModel("SELECT DISTINCT [LinkRezId],[FIO],[Serial],[Etap],MAX(DateStart) AS DateStart,[DateEnd]"
                                       " FROM"
                                       " (SELECT DISTINCT Link_Result.Id AS LinkRezId,[FIO],[Serial],[Etap],[DateStart],[DateEnd]"
                                       " FROM Link_Result"
                                       " Join User ON Link_Result.IdUser = User.Id  "
                                       " JOIN Serial ON Link_Result.IdSerial = Serial.Id "
                                       " JOIN Etap ON Link_Result.IdEtap = Etap.Id"
                                       " JOIN DateStartEnd ON Link_Result.IdDate = DateStartEnd.Id "
                                       " WHere Serial = '"+Select[0]+"' AND IdEtap = 4 AND IdTemp = 1)");


    LinkRezId = SQL_id->data(SQL_id->index(0,0), Qt::EditRole).toString();
    FIO = SQL_id->data(SQL_id->index(0,1), Qt::EditRole).toString();
    DateStart = SQL_id->data(SQL_id->index(0,4), Qt::EditRole).toString();
    DateEnd = SQL_id->data(SQL_id->index(0,5), Qt::EditRole).toString();


    list_id_Proverki_Find_Name_Sootvetstvie_NextNeprWork_NY.clear();

    SQL_id = alg->BD->zaprosQueryModel("SELECT DISTINCT [Sootv],[IdProverki],[DateStayEnd],[DateStayStart]"
                                       " FROM Result"
                                       " Join Link_Result ON Link_Result.Id = Result.IdLink "
                                       " WHere IdLink = '"+LinkRezId+"'");
    if(SQL_id->rowCount() < 1)
    {
        for(int i=0; i < list_id_Proverki_Find_Name.count();i++)
        {
            list_id_Proverki_Find_Name_Sootvetstvie_NextNeprWork_NY.append("-");
        }
    }
    else
    {
        for(int i=0;i< SQL_id->rowCount();i++)
        {
            if(SQL_id->data(SQL_id->index(i,0), Qt::EditRole).toString() == "")
            {
                list_id_Proverki_Find_Name_Sootvetstvie_NextNeprWork_NY.append("-");
            }
            else
            {
                list_id_Proverki_Find_Name_Sootvetstvie_NextNeprWork_NY.append(SQL_id->data(SQL_id->index(i,0), Qt::EditRole).toString());
            }
        }

        if(SQL_id->rowCount() <  list_id_Proverki_Find_Name.count()-3)
        {

            for(int i=list_id_Proverki_Find_Name_Sootvetstvie_NextNeprWork_NY.count()-1;i < list_id_Proverki_Find_Name.count()-3;i++)
            {
                list_id_Proverki_Find_Name_Sootvetstvie_NextNeprWork_NY.append("-");
            }
        }
    }

    list_id_Proverki_Find_Name_Sootvetstvie_NextNeprWork_NY.append(DateStart +"\n"+DateEnd);
    list_id_Proverki_Find_Name_Sootvetstvie_NextNeprWork_NY.append(FIO);




    slot_CreateWord();

    //  slot_CreatePDF();

}


/*!
    \brief Функция формирования отчета в PDF на основе HTML


    Функция формирования PDF файла на основе HTML.\n

    \author Sergey Smoglyuk
    \version 1.0
    \date Август 2018 года
    \warning Данная функция не  используется в программе
*/
void Report::slot_CreatePDF()
{
    QTextDocument doc;
    QString table_html;
    QString table_style;

    table_style = QString("<style>"
                          "html,body {margin: 0;padding: 0;}"
                          "p {margin: 0px;}"
                          "table {word-wrap:break-word; text-align: center; vertical-align: middle; border-collapse: collapse; border: 10px solid  black; }"
                          "td,th {text-align: center;}"
                          "</style>");

    table_html =table_style;

    // "<h2>Старт: "+start_proverka+" Окончание: "+end_proverka+"</h2>"
    table_html += QString(" <center>"
                          "<h2>Электронный протокол предъявительских и приемосдаточных испытаний (ПСИ)</h2>"
                          "</center>\n");


    table_html +=  " <table   width=\"100%\" border=\"1\" cellpadding=\"0\" cellspacing=\"0\" >"; //cellpadding=\"1\"


    //Формирование шапки 1 объединенная строка
    table_html +=  " <tr >"  //bgcolor=\"#FFFFFF\"
            " <td style=\"font-weight:bold\">№</td>"
            " <td style=\"font-weight:bold\">Наименнование параметра</td>";

    for(int i=0; i < list_id_Device_Find.count();i++)
    {
        table_html +=" <td colspan=\"2\" style=\"font-weight:bold\">НП № "+QString::number(i+1)+"</td>";

    }



    table_html += " </tr>";

    for(int i=0; i < list_id_Proverki_Find.count();i++) //
    {

        table_html += " <tr >"; //строчка

        table_html +=  " <td align=\"center\">"+QString::number(i+1)+"</td>";

        table_html +=  " <td align=\"center\">"+list_id_Proverki_Find_Name.value(i)+"</td>";

        for(int j=0; j < list_id_Device_Find.count();j++)
        {
            QSqlQueryModel* SQL_id = alg->BD->zaprosQueryModel("SELECT Sootv FROM Result WHERE IdDevice = '"+list_id_Device_Find.value(j)+"'  AND DateStart = '"+start_proverka+"' AND IdProverki = '"+list_id_Proverki_Find.value(i)+"' ");

            bool flag = false;

            for(int k=0; k< SQL_id->rowCount();k++)
            {
                QString  Sootv = SQL_id->data(SQL_id->index(k,0), Qt::EditRole).toString();

                if(Sootv != "Соотв.")
                {
                    flag = true;
                    break;
                }

            }

            if(SQL_id->rowCount() <=0)
            {
                table_html +=  " <td colspan=\"2\" align=\"center\">&nbsp;Не проверялся&nbsp;</td>"; //&#215;

            }
            else
            {

                if(flag == false)
                {
                    table_html +=  " <td colspan=\"2\" align=\"center\">&nbsp;Соотв.&nbsp;</td>"; //&#10003;
                }
                else
                {
                    table_html +=  " <td colspan=\"2\" align=\"center\">&nbsp;Не соотв.&nbsp;</td>"; //&#10008;;
                }
            }


        }

        table_html += " </tr>";

    }



    table_html += " </table>";


    doc.setHtml(table_html);


    QString fileName = QFileDialog::getSaveFileName(nullptr, "Export PDF", QString(), "*.pdf"); //(QWidget* )0
    if (QFileInfo(fileName).suffix().isEmpty()) { fileName.append(".pdf"); }

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QPrinter::A4);
    printer.setOutputFileName(fileName);

    printer.setOrientation(QPrinter::Landscape);
    // printer.setOrientation(QPrinter::Portrait);



    //doc.setPageSize(printer.pageRect().size()); // This is necessary if you want to hide the page number
    doc.print(&printer);

}

/*!
    \brief Функция формирования отчета в WORD, а затем в  PDF на основе QAxObject


    Функция формирования отчета в WORD, а затем в  PDF на основе QAxObject.\n

    \author Sergey Smoglyuk
    \version 1.0
    \date Август 2018 года
*/
void Report::slot_CreateWord()
{
    QAxObject* WordApplication=new QAxObject("Word.Application"); // Создаю интерфейс к MSWord
    QAxObject* WordDocuments = WordApplication->querySubObject( "Documents()" ); // Получаю интерфейсы к его подобъекту "коллекция открытых документов":
    WordDocuments->querySubObject( "Add()" ); // Создаю новый документ
    // отключение грамматики
    QAxObject* Grammatic = WordApplication->querySubObject("Options()");
    Grammatic->setProperty("CheckSpellingAsYouType(bool)", false); // отключение грамматики
    QAxObject* ActiveDocument = WordApplication->querySubObject("ActiveDocument()");
    QAxObject* Range = ActiveDocument->querySubObject("Range()");


    QAxObject* PageSetup = ActiveDocument->querySubObject("PageSetup()");
    PageSetup->setProperty( "LeftMargin", 13);
    PageSetup->setProperty( "RightMargin", 25);
    PageSetup->setProperty( "TopMargin", 15);
    PageSetup->setProperty( "BottomMargin", 20);

    //Портретный вид документа
    PageSetup->setProperty("Orientation", 1);


    //Показать окно
    WordApplication->setProperty( "Visible", true );

    //Шрифт
    QAxObject *font = Range->querySubObject("Font");
    QAxObject *selection = WordApplication->querySubObject("Selection()"); //выделяем область

    // selection->dynamicCall("TypeParagraph()");

    font->setProperty("Size", 10); //размер заголовка
    // font->setProperty("Bold", 2); //жирный
    font->setProperty("Name", "Times New Roman"); // шрифт

    QAxObject* pswds = selection->querySubObject ("ParagraphFormat()");
    pswds->dynamicCall ("SetAlignment (WdParagraphAlignment)", 1); // по центру
    pswds->setProperty("SpaceAfter",0); // Межстрочные  интервал
    selection->setProperty("Alignment", 1);
    selection->dynamicCall("TypeText(const QString&)","Приемник, зав № МРК008\n"); //записываем текст
    selection->dynamicCall("TypeText(const QString&)","Электронный протокол предъявительских и приемосдаточных испытаний (ПСИ)\n");
   // selection->dynamicCall("TypeText(const QString&)","Регулировщик:\n");
    selection->dynamicCall("TypeParagraph()");




    QAxObject *selection2 = WordApplication->querySubObject("Selection()");
    QAxObject *font2 = selection->querySubObject("Font()");
    font2->setProperty("Size", 8);
    font2->setProperty("Bold", 0);  // не жирный
    font2->setProperty("Name", "Times New Roman");
    // QAxObject* pswds2 = selection2->querySubObject ("ParagraphFormat ()");
    //  pswds2->dynamicCall ("SetAlignment (WdParagraphAlignment)", 1); // по ширине
    // selection2->setProperty("Alignment", 1);



    // создание таблицы
    QAxObject* Tables = selection2->querySubObject("Tables()");
    QAxObject* NewTable = Tables->querySubObject("Add(Range, NumRows, NumColumns, DefaultTableBehavior, AutoFitBehavior)", selection2->dynamicCall("Range"), list_id_Proverki_Find.count()+4, 14, 2, 2); //Range(Start,End)






    /////////////////////////////

    //Этап регулировки
    QAxObject* StartCell  = NewTable->querySubObject("Cell(Row, Column)", 1, 3); // (ячейка C1)
    QAxObject* CellRange = StartCell->querySubObject("Range()");

    //Выравнивание по центру (шапка)
    // StartCell->setProperty("VerticalAlignment", 1);

    CellRange->dynamicCall("InsertAfter(Text)", "Этап регулировки");

    QAxObject* EndCell  = NewTable->querySubObject("Cell(Row, Column)", 1, 7);

    //Объединение ячеек
    StartCell ->dynamicCall("Merge(QAxObject *)",EndCell->asVariant());


    StartCell  = NewTable->querySubObject("Cell(Row, Column)", 2, 3);
    //Выравнивание по центру (шапка)
    StartCell->setProperty("VerticalAlignment", 1);
    CellRange = StartCell->querySubObject("Range()");
    CellRange->dynamicCall("InsertAfter(Text)", "НУ");

    StartCell  = NewTable->querySubObject("Cell(Row, Column)", 2, 4);
    //Выравнивание по центру (шапка)
    StartCell->setProperty("VerticalAlignment", 1);
    CellRange = StartCell->querySubObject("Range()");
    CellRange->dynamicCall("InsertAfter(Text)", "При пониженной рабочей температуре");

    StartCell  = NewTable->querySubObject("Cell(Row, Column)", 2, 5);
    //Выравнивание по центру (шапка)
    StartCell->setProperty("VerticalAlignment", 1);
    CellRange = StartCell->querySubObject("Range()");
    CellRange->dynamicCall("InsertAfter(Text)", "НУ");

    StartCell  = NewTable->querySubObject("Cell(Row, Column)", 2, 6);
    //Выравнивание по центру (шапка)
    StartCell->setProperty("VerticalAlignment", 1);
    CellRange = StartCell->querySubObject("Range()");
    CellRange->dynamicCall("InsertAfter(Text)", "При повышенной рабочей температуре");

    StartCell  = NewTable->querySubObject("Cell(Row, Column)", 2, 7);
    //Выравнивание по центру (шапка)
    StartCell->setProperty("VerticalAlignment", 1);
    CellRange = StartCell->querySubObject("Range()");
    CellRange->dynamicCall("InsertAfter(Text)", "НУ");



    /////////////////////////////
    //Наименование параметра
    StartCell = NewTable->querySubObject("Cell(Row, Column)", 1, 1);
    //Выравнивание по центру (шапка)
    StartCell->setProperty("VerticalAlignment", 1);
    CellRange = StartCell->querySubObject("Range()");
    CellRange->dynamicCall("InsertAfter(Text)", "Наименование параметра");

    EndCell  = NewTable->querySubObject("Cell(Row, Column)", 2, 1);

    //Объединение ячеек
    StartCell ->dynamicCall("Merge(QAxObject *)",EndCell->asVariant());
    /////////////////////////////

    //Требования ПМ
    StartCell = NewTable->querySubObject("Cell(Row, Column)", 1, 2);
    //Выравнивание по центру (шапка)
    StartCell->setProperty("VerticalAlignment", 1);
    CellRange = StartCell->querySubObject("Range()");
    CellRange->dynamicCall("InsertAfter(Text)", "Требования ПМ");

    EndCell  = NewTable->querySubObject("Cell(Row, Column)", 2, 2);

    //Объединение ячеек
    StartCell ->dynamicCall("Merge(QAxObject *)",EndCell->asVariant());


    ////////////////////////////////////////

    //После техн. тренировки в НУ
    StartCell = NewTable->querySubObject("Cell(Row, Column)", 1, 4);
    //Выравнивание по центру (шапка)
    StartCell->setProperty("VerticalAlignment", 1);
    CellRange = StartCell->querySubObject("Range()");
    CellRange->dynamicCall("InsertAfter(Text)", "После техн. тренировки в НУ");

    EndCell  = NewTable->querySubObject("Cell(Row, Column)", 2, 8);

    //Объединение ячеек
    StartCell ->dynamicCall("Merge(QAxObject *)",EndCell->asVariant());

    ////////////////////////////////////////

    //Предъявительские и ПСИ
    StartCell  = NewTable->querySubObject("Cell(Row, Column)", 1, 5); // (ячейка C1)
    //Выравнивание по центру (шапка)
    //StartCell->setProperty("VerticalAlignment", 1);
    CellRange = StartCell->querySubObject("Range()");

    CellRange->dynamicCall("InsertAfter(Text)", "Предъявительские и ПСИ");


    EndCell  = NewTable->querySubObject("Cell(Row, Column)", 1, 9);


    //Объединение ячеек
    StartCell ->dynamicCall("Merge(QAxObject *)",EndCell->asVariant());


    StartCell  = NewTable->querySubObject("Cell(Row, Column)", 2, 9);
    //Выравнивание по центру (шапка)
    StartCell->setProperty("VerticalAlignment", 1);
    CellRange = StartCell->querySubObject("Range()");
    CellRange->dynamicCall("InsertAfter(Text)", "НУ");

    StartCell  = NewTable->querySubObject("Cell(Row, Column)", 2, 10);
    //Выравнивание по центру (шапка)
    StartCell->setProperty("VerticalAlignment", 1);
    CellRange = StartCell->querySubObject("Range()");
    CellRange->dynamicCall("InsertAfter(Text)", "При пониженной рабочей температуре");

    StartCell  = NewTable->querySubObject("Cell(Row, Column)", 2, 11);
    //Выравнивание по центру (шапка)
    StartCell->setProperty("VerticalAlignment", 1);
    CellRange = StartCell->querySubObject("Range()");
    CellRange->dynamicCall("InsertAfter(Text)", "НУ");

    StartCell  = NewTable->querySubObject("Cell(Row, Column)", 2, 12);
    //Выравнивание по центру (шапка)
    StartCell->setProperty("VerticalAlignment", 1);
    CellRange = StartCell->querySubObject("Range()");
    CellRange->dynamicCall("InsertAfter(Text)", "При повышенной рабочей температуре");

    StartCell  = NewTable->querySubObject("Cell(Row, Column)", 2, 13);
    //Выравнивание по центру (шапка)
    StartCell->setProperty("VerticalAlignment", 1);
    CellRange = StartCell->querySubObject("Range()");
    CellRange->dynamicCall("InsertAfter(Text)", "НУ");

    // //////////////////////////////////////

    //После неприрывной работы
    StartCell = NewTable->querySubObject("Cell(Row, Column)", 1, 6);
    //Выравнивание по центру (шапка)
    StartCell->setProperty("VerticalAlignment", 1);
    CellRange = StartCell->querySubObject("Range()");
    CellRange->dynamicCall("InsertAfter(Text)", "После непрерывной работы");

    EndCell  = NewTable->querySubObject("Cell(Row, Column)", 2, 14);

    //Объединение ячеек
    StartCell ->dynamicCall("Merge(QAxObject *)",EndCell->asVariant());



    for(int i=3; i < list_id_Proverki_Find_Name.count()+3;i++)
    {
        StartCell = NewTable->querySubObject("Cell(Row, Column)", i, 1);
        StartCell->setProperty("VerticalAlignment", 0);

        CellRange = StartCell->querySubObject("Range()");
        //Выравнивание по левому краю
        CellRange->querySubObject("ParagraphFormat()")->setProperty("Alignment",0);

        CellRange->dynamicCall("InsertAfter(Text)", list_id_Proverki_Find_Name.value(i-3));

        /////////////////////////////////
        //Заполнение строчки требования ПМ
        StartCell = NewTable->querySubObject("Cell(Row, Column)", i, 2);

        StartCell->setProperty("VerticalAlignment", 1);
        CellRange = StartCell->querySubObject("Range()");
        //Выравнивание по левому краю
        CellRange->querySubObject("ParagraphFormat()")->setProperty("Alignment",1);

        CellRange->dynamicCall("InsertAfter(Text)", list_id_Proverki_Find_Name_Treb.value(i-3));

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /////////////////////////////////
        //Заполнение строчки требования ПМ
        StartCell = NewTable->querySubObject("Cell(Row, Column)", i, 3);

        StartCell->setProperty("VerticalAlignment", 1);
        CellRange = StartCell->querySubObject("Range()");
        //Выравнивание по левому краю
        CellRange->querySubObject("ParagraphFormat()")->setProperty("Alignment",1);

        CellRange->dynamicCall("InsertAfter(Text)", list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_NY.value(i-3));
        /////////////////////////////////
        //Заполнение строчки требования ПМ
        StartCell = NewTable->querySubObject("Cell(Row, Column)", i, 4);

        StartCell->setProperty("VerticalAlignment", 1);
        CellRange = StartCell->querySubObject("Range()");
        //Выравнивание по левому краю
        CellRange->querySubObject("ParagraphFormat()")->setProperty("Alignment",1);

        CellRange->dynamicCall("InsertAfter(Text)", list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_ColD.value(i-3));
        /////////////////////////////////
        //Заполнение строчки требования ПМ
        StartCell = NewTable->querySubObject("Cell(Row, Column)", i, 5);

        StartCell->setProperty("VerticalAlignment", 1);
        CellRange = StartCell->querySubObject("Range()");
        //Выравнивание по левому краю
        CellRange->querySubObject("ParagraphFormat()")->setProperty("Alignment",1);

        CellRange->dynamicCall("InsertAfter(Text)", list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_ColDNY.value(i-3));

        /////////////////////////////////
        //Заполнение строчки требования ПМ
        StartCell = NewTable->querySubObject("Cell(Row, Column)", i, 6);

        StartCell->setProperty("VerticalAlignment", 1);
        CellRange = StartCell->querySubObject("Range()");
        //Выравнивание по левому краю
        CellRange->querySubObject("ParagraphFormat()")->setProperty("Alignment",1);

        CellRange->dynamicCall("InsertAfter(Text)", list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_Hort.value(i-3));

        /////////////////////////////////
        //Заполнение строчки требования ПМ
        StartCell = NewTable->querySubObject("Cell(Row, Column)", i, 7);

        StartCell->setProperty("VerticalAlignment", 1);
        CellRange = StartCell->querySubObject("Range()");
        //Выравнивание по левому краю
        CellRange->querySubObject("ParagraphFormat()")->setProperty("Alignment",1);

        CellRange->dynamicCall("InsertAfter(Text)", list_id_Proverki_Find_Name_Sootvetstvie_Regylirovka_HortNY.value(i-3));

        /////////////////////////////////
        //Заполнение строчки требования ПМ
        StartCell = NewTable->querySubObject("Cell(Row, Column)", i, 8);

        StartCell->setProperty("VerticalAlignment", 1);
        CellRange = StartCell->querySubObject("Range()");
        //Выравнивание по левому краю
        CellRange->querySubObject("ParagraphFormat()")->setProperty("Alignment",1);

        CellRange->dynamicCall("InsertAfter(Text)", list_id_Proverki_Find_Name_Sootvetstvie_NextTexTest_NY.value(i-3));

        /////////////////////////////////
        //Заполнение строчки требования ПМ
        StartCell = NewTable->querySubObject("Cell(Row, Column)", i, 9);

        StartCell->setProperty("VerticalAlignment", 1);
        CellRange = StartCell->querySubObject("Range()");
        //Выравнивание по левому краю
        CellRange->querySubObject("ParagraphFormat()")->setProperty("Alignment",1);

        CellRange->dynamicCall("InsertAfter(Text)", list_id_Proverki_Find_Name_Sootvetstvie_PSI_NY.value(i-3));

        /////////////////////////////////
        //Заполнение строчки требования ПМ
        StartCell = NewTable->querySubObject("Cell(Row, Column)", i, 10);

        StartCell->setProperty("VerticalAlignment", 1);
        CellRange = StartCell->querySubObject("Range()");
        //Выравнивание по левому краю
        CellRange->querySubObject("ParagraphFormat()")->setProperty("Alignment",1);

        CellRange->dynamicCall("InsertAfter(Text)", list_id_Proverki_Find_Name_Sootvetstvie_PSI_ColD.value(i-3));

        /////////////////////////////////
        //Заполнение строчки требования ПМ
        StartCell = NewTable->querySubObject("Cell(Row, Column)", i, 11);

        StartCell->setProperty("VerticalAlignment", 1);
        CellRange = StartCell->querySubObject("Range()");
        //Выравнивание по левому краю
        CellRange->querySubObject("ParagraphFormat()")->setProperty("Alignment",1);

        CellRange->dynamicCall("InsertAfter(Text)", list_id_Proverki_Find_Name_Sootvetstvie_PSI_ColDNY.value(i-3));

        /////////////////////////////////
        //Заполнение строчки требования ПМ
        StartCell = NewTable->querySubObject("Cell(Row, Column)", i, 12);

        StartCell->setProperty("VerticalAlignment", 1);
        CellRange = StartCell->querySubObject("Range()");
        //Выравнивание по левому краю
        CellRange->querySubObject("ParagraphFormat()")->setProperty("Alignment",1);

        CellRange->dynamicCall("InsertAfter(Text)", list_id_Proverki_Find_Name_Sootvetstvie_PSI_Hort.value(i-3));

        /////////////////////////////////
        //Заполнение строчки требования ПМ
        StartCell = NewTable->querySubObject("Cell(Row, Column)", i, 13);

        StartCell->setProperty("VerticalAlignment", 1);
        CellRange = StartCell->querySubObject("Range()");
        //Выравнивание по левому краю
        CellRange->querySubObject("ParagraphFormat()")->setProperty("Alignment",1);

        CellRange->dynamicCall("InsertAfter(Text)", list_id_Proverki_Find_Name_Sootvetstvie_PSI_HortNY.value(i-3));

        /////////////////////////////////
        //Заполнение строчки требования ПМ
        StartCell = NewTable->querySubObject("Cell(Row, Column)", i, 14);

        StartCell->setProperty("VerticalAlignment", 1);
        CellRange = StartCell->querySubObject("Range()");
        //Выравнивание по левому краю
        CellRange->querySubObject("ParagraphFormat()")->setProperty("Alignment",1);

        CellRange->dynamicCall("InsertAfter(Text)", list_id_Proverki_Find_Name_Sootvetstvie_NextNeprWork_NY.value(i-3));

    }




    QString fileName = QFileDialog::getSaveFileName(nullptr, "Export Word", QString());//,"*.docx;;*.pdf" (QWidget* )0
    //  if (QFileInfo(fileName).suffix().isEmpty()) { fileName.append(".docx"); }

    // QString fileName=QFileDialog::getSaveFileName(0,"save file","export_table",
    //         "XML Spreadhseet(*.xlsx);;eXceL Spreadsheet(*.xls);;Comma Seperated Value(*.csv)");

    // fileName.replace("/", "\"");

    qDebug () << "file = " << fileName;



    ActiveDocument->dynamicCall("ExportAsFixedFormat (const QString&,const QString&)", fileName.split('.').first(),"17");

    ActiveDocument->dynamicCall("SaveAs (const QString&)", fileName);
    ActiveDocument->dynamicCall("SaveAs2 (const QString&)", fileName);
    ActiveDocument->dynamicCall("Close (boolean)", false);
    WordApplication->dynamicCall("Quit (void)");

    delete WordApplication;
}

/*!
    \brief Функция поиска Серийного номера в БД


    Функция поиска Серийного номера в БД.\n

    \author Sergey Smoglyuk
    \version 1.0
    \date Август 2018 года
*/
void Report::slot_GetSerial()
{
    //поиск id существующих Проверок в Бд

    list_id_Serial.clear();
    list_name_Serial.clear();

    list_id_Serial = alg->BD->report("SELECT * FROM Serial",0);
    list_name_Serial = alg->BD->report("SELECT * FROM Serial",1);

    qDebug() << list_name_Serial;

    emit signal_list(list_name_Serial);

}

/*!
    \brief Функция поиска ФИО в БД


    Функция поиска ФИО в БД.\n

    \author Sergey Smoglyuk
    \version 1.0
    \date Август 2018 года
*/
void Report::slot_GetFIO(QString serial)
{

    //    Select[0] = serial;
    //    Select[1] = "";
    //    Select[2] = "";

    //    SetlistSelect(Select);

    //поиск id существующих Проверок в Бд
    QSqlQueryModel* SQL_id;

    SQL_id = alg->BD->zaprosQueryModel("SELECT DISTINCT [IdUser],[FIO]"
                                       " FROM Link_Result"
                                       " Join User ON Link_Result.IdUser = User.Id"
                                       " JOIN Serial ON Link_Result.IdSerial = Serial.Id"
                                       " WHERE Serial = '"+serial+"'");

    list_id_FIO.clear();
    list_name_FIO.clear();



    for(int i=0;i< SQL_id->rowCount();i++)
    {
        list_id_FIO.append(SQL_id->data(SQL_id->index(i,0), Qt::EditRole).toString());
        list_name_FIO.append(SQL_id->data(SQL_id->index(i,1), Qt::EditRole).toString());
    }

    emit signal_listSerial(list_name_FIO);

}

/*!
    \brief Функция поиска Этапа в БД


    Функция поиска Этапа в БД.\n

    \author Sergey Smoglyuk
    \version 1.0
    \date Август 2018 года
*/
void Report::slot_GetEtap(QString serial,QString FIO)
{

    //поиск id существующих Проверок в Бд
    QSqlQueryModel* SQL_id;

    SQL_id = alg->BD->zaprosQueryModel("SELECT DISTINCT [IdEtap],[Etap]"
                                       " FROM Link_Result"
                                       " Join User ON Link_Result.IdUser = User.Id"
                                       " JOIN Serial ON Link_Result.IdSerial = Serial.Id"
                                       " JOIN Etap ON Link_Result.IdEtap = Etap.Id"
                                       " WHERE Serial = '"+serial+"' AND FIO = '"+FIO+"'");

    list_id_Etap.clear();
    list_name_Etap.clear();



    for(int i=0;i< SQL_id->rowCount();i++)
    {
        list_id_Etap.append(SQL_id->data(SQL_id->index(i,0), Qt::EditRole).toString());
        list_name_Etap.append(SQL_id->data(SQL_id->index(i,1), Qt::EditRole).toString());
    }



    emit signal_listSerialEtap(list_name_Etap);

}

/*!
    \brief Функция поиска информации в выбранном этапе в БД


    Функция поиска информации в выбранном этапе в БД.\n

    \author Sergey Smoglyuk
    \version 1.0
    \date Август 2018 года
*/
void Report::slot_GetEtapData(QString serial, QString FIO, QString Etap)
{
    //поиск id существующих Проверок в Бд
    QSqlQueryModel* SQL_id;

    SQL_id = alg->BD->zaprosQueryModel("SELECT DISTINCT [IdDate],[DateStart],[DateEnd]"
                                       " FROM Link_Result"
                                       " Join User ON Link_Result.IdUser = User.Id"
                                       " JOIN Serial ON Link_Result.IdSerial = Serial.Id"
                                       " JOIN Etap ON Link_Result.IdEtap = Etap.Id"
                                       " JOIN DateStartEnd ON Link_Result.IdDate = DateStartEnd.Id "
                                       " WHERE Serial = '"+serial+"' AND FIO = '"+FIO+"' AND Etap = '"+Etap+"'");


    qDebug () << "dsfadasf = " << serial << FIO << Etap;



    list_id_Date.clear();
    list_start_Date.clear();
    list_end_Date.clear();



    for(int i=0;i< SQL_id->rowCount();i++)
    {
        list_id_Date.append(SQL_id->data(SQL_id->index(i,0), Qt::EditRole).toString());
        list_start_Date.append(SQL_id->data(SQL_id->index(i,1), Qt::EditRole).toString());
        list_end_Date.append(SQL_id->data(SQL_id->index(i,2), Qt::EditRole).toString());
    }

    qDebug () << "dsfadasf = " <<list_start_Date;

    emit signal_listSerialDate(list_start_Date,list_end_Date);
}

/*!
    \brief Функция поиска информации для генерации отчета в БД


    Функция поиска информации для генерации отчета в БД.\n

    \author Sergey Smoglyuk
    \version 1.0
    \date Август 2018 года
*/
void Report::slot_GetResult(QString serial, QString FIO, QString Etap, QString Date)
{
    //поиск id существующих Проверок в Бд
    QSqlQueryModel* SQL_id;

    SQL_id = alg->BD->zaprosQueryModel("SELECT DISTINCT *"
                                       " FROM Link_Result"
                                       " Join User ON Link_Result.IdUser = User.Id"
                                       " JOIN Serial ON Link_Result.IdSerial = Serial.Id"
                                       " JOIN Etap ON Link_Result.IdEtap = Etap.Id"
                                       " JOIN DateStartEnd ON Link_Result.IdDate = DateStartEnd.Id "
                                       " WHERE Serial = '"+serial+"' AND FIO = '"+FIO+"' AND Etap = '"+Etap+"' AND DateStart = '"+Date+"'");


    qDebug () << "dsfadasf = " << serial << FIO << Etap << Date;




    IdResult = SQL_id->data(SQL_id->index(0,0), Qt::EditRole).toString();


    SQL_id = alg->BD->zaprosQueryModel("SELECT DISTINCT [IdProverki],[NameProverki],[Treb],[Sootv],[DateStayStart],[DateStayEnd]"
                                       " FROM Result"
                                       " JOIN Proverki ON Result.IdProverki = Proverki.Id"
                                       " WHere IdLink = '"+IdResult+"'");


    list_Result_IdProverki.clear();
    list_Result_NameProverki.clear();
    list_Result_Treb.clear();
    list_Result_Sootv.clear();
    list_Result_DateStayStart.clear();
    list_Result_DateStayEnd.clear();
    list_NP.clear();
    list_NP_Sootv.clear();


    for(int i=0;i< SQL_id->rowCount();i++)
    {
        list_Result_IdProverki.append(SQL_id->data(SQL_id->index(i,0), Qt::EditRole).toString());
        list_Result_NameProverki.append(SQL_id->data(SQL_id->index(i,1), Qt::EditRole).toString());

        auto treb = SQL_id->data(SQL_id->index(i,2), Qt::EditRole).toString().split('\n');
        QString strTreb = nullptr;
        for(int j =0; j < treb.count();j++)
        {
            strTreb += treb[j]+" ";
        }
        //qDebug() << strTreb;
        list_Result_Treb.append(strTreb);

        list_Result_Sootv.append(SQL_id->data(SQL_id->index(i,3), Qt::EditRole).toString());
        list_Result_DateStayStart.append(SQL_id->data(SQL_id->index(i,4), Qt::EditRole).toString());
        list_Result_DateStayEnd.append(SQL_id->data(SQL_id->index(i,5), Qt::EditRole).toString());
    }


    SQL_id = alg->BD->zaprosQueryModel("SELECT DISTINCT [NP],[Sootv]"
                                       " FROM Liter"
                                       " WHere IdLink = '"+IdResult+"'");

    for(int i=0;i< SQL_id->rowCount();i++)
    {
        list_NP.append(SQL_id->data(SQL_id->index(i,0), Qt::EditRole).toString());
        list_NP_Sootv.append(SQL_id->data(SQL_id->index(i,1), Qt::EditRole).toString());
    }



    emit signal_listResult(list_Result_IdProverki,list_Result_NameProverki,list_Result_Treb,list_Result_Sootv,list_Result_DateStayStart,list_Result_DateStayEnd,list_NP,list_NP_Sootv);
}

/*!
    \brief Функция поиска информации для генерации отчета в БД


    Функция поиска информации для генерации отчета в БД.\n

    \author Sergey Smoglyuk
    \version 1.0
    \date Август 2018 года
    \warning Данная функция используется в QML для выбора проверки и этапов.
*/
void Report::slot_setSelect(QString set, int setId)
{
    Select[setId] = set;

    emit signal_listSelectChanged(Select);
}
