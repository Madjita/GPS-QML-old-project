#include "report.h"
#include <QTextDocument>
#include <QFileDialog>
#include <QPrinter>

Report::Report(gsgModel* GetGSG,QObject *parent) :
    QObject(parent),
    gsg(GetGSG)
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


}

void Report::slot_FindData()
{
    //поиск id существующих Проверок в Бд
    QSqlQueryModel* SQL_id;

   SQL_id = gsg->BD->zaprosQueryModel("SELECT * FROM Proverki");

   list_id_Proverki_Find.clear();

   for(int i=0;i< SQL_id->rowCount();i++)
   {
       list_id_Proverki_Find.append(SQL_id->data(SQL_id->index(i,0), Qt::EditRole).toString());
       list_id_Proverki_Find_Name.append(SQL_id->data(SQL_id->index(i,1), Qt::EditRole).toString());
   }


    slot_CreatePDF();

}







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

   table_html += QString(" <center>"
                         "<h1>Электронный протокол испытаний</h1>"
                         "<h2>Старт: "+start_proverka+" Окончание: "+end_proverka+"</h2>"
                         "</center>\n");

   table_html +=  " <table   width=\"100%\" border=\"1\" cellpadding=\"0\" cellspacing=\"0\" >"; //cellpadding=\"1\"


   //Формирование шапки 1 объединенная строка
   table_html +=  " <tr >"  //bgcolor=\"#FFFFFF\"
           " <td style=\"font-weight:bold\">№</td>"
           " <td style=\"font-weight:bold\">Наименование параметра</td>";

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
           QSqlQueryModel* SQL_id = gsg->BD->zaprosQueryModel("SELECT Sootv FROM Result WHERE IdDevice = '"+list_id_Device_Find.value(j)+"'  AND DateStart = '"+start_proverka+"' AND IdProverki = '"+list_id_Proverki_Find.value(i)+"' ");

           bool flag = false;

           for(int k=0; k< SQL_id->rowCount();k++)
           {
             QString  Sootv = SQL_id->data(SQL_id->index(k,0), Qt::EditRole).toString();

             if(Sootv != "Соответствует")
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
                 table_html +=  " <td colspan=\"2\" align=\"center\">&nbsp;Соответствует&nbsp;</td>"; //&#10003;
               }
               else
               {
                  table_html +=  " <td colspan=\"2\" align=\"center\">&nbsp;Не соответствует&nbsp;</td>"; //&#10008;;
               }
           }


        }

        table_html += " </tr>";

    }


     int  count_td = 0; // Количество всех колонок



     table_html += " </table>";


     doc.setHtml(table_html);


     QString fileName = QFileDialog::getSaveFileName((QWidget* )0, "Export PDF", QString(), "*.pdf");
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
